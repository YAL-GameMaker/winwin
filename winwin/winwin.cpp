/// @author YellowAfterlife

#include "stdafx.h"
#include "winwin.h"

std::vector<winwin*> ww_list{};
std::unordered_map<HWND, winwin*> ww_map{};
wm_base_t ww_base{};
StringConv ww_c1, ww_c2;
HCURSOR ww_base_cursor;

///
dllx double winwin_is_available() {
    return 1;
}

LRESULT winwin_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

constexpr wchar_t winwin_class[] = L"winwin_class";
HCURSOR winwin_cursor_init();
dllx double winwin_init_raw(void* hwnd, void* device, void* context, void* swapchain) {
    //
    ww_base.main_hwnd = (HWND)hwnd;
    ww_base.device = (ID3D11Device*)device;
    ww_base.context = (ID3D11DeviceContext*)context;
    ww_base.main_swapchain = (IDXGISwapChain*)swapchain;
    ww_base.hInstance = (HINSTANCE)GetWindowLongPtr(ww_base.main_hwnd, GWLP_HINSTANCE); // yeah dunno
    //
    WNDCLASS wndc{};
    wndc.hInstance = ww_base.hInstance;
    wndc.lpszClassName = winwin_class;
    wndc.lpfnWndProc = winwin_wndproc;
    wndc.hIcon = (HICON)GetClassLongPtr(ww_base.main_hwnd, GCLP_HICON);
    RegisterClass(&wndc);
    //
    auto ww_main = new winwin();
    ww_main->hwnd = ww_base.main_hwnd;
    ww_main->swapchain = ww_base.main_swapchain;
    ww_base.ref = ww_main;
    //
    ww_base_cursor = winwin_cursor_init();
    //
    return 1;
}
///~
dllg ww_ptr_create winwin_init_2() {
    return ww_base.ref;
}

bool winwin_create_impl(winwin* ww, winwin_config& config, int x, int y, int width, int height) {
    DWORD dwExStyle = 0;
    DWORD dwStyle;
    if (config.kind == winwin_kind::borderless) {
        dwStyle = WS_POPUP;
        if (!config.taskbar_button) {
            dwExStyle |= WS_EX_TOOLWINDOW;
        }
    } else {
        dwStyle = WS_OVERLAPPEDWINDOW;
        if (!config.resize) dwStyle &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
        if (config.kind == winwin_kind::tool) {
            dwExStyle |= WS_EX_TOOLWINDOW;
        }
    }
    if (config.noactivate) dwExStyle |= WS_EX_NOACTIVATE;
    if (config.clickthrough) dwExStyle |= WW_WS_EX_CLICKTHROUGH;
    //
    RECT rcClient = { x, y, x + width, y + height };
    AdjustWindowRectEx(&rcClient, dwStyle, false, dwExStyle);
    //
    auto hwnd = CreateWindowExW(
        dwExStyle,
        winwin_class,
        ww_cc(config.caption),
        dwStyle,
        rcClient.left, rcClient.top, rect_width(rcClient), rect_height(rcClient),
        nullptr, nullptr, ww_base.hInstance, nullptr
    );
    if (hwnd == nullptr) return false;
    //
    if (config.close_button == 0) {
        auto menu = GetSystemMenu(hwnd, false);
        EnableMenuItem(menu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
    }
    //
    ww->hwnd = hwnd;
    ww->close_button = config.close_button;
    ww->buf.width = width;
    ww->buf.height = height;
    ww->buf.new_width = width;
    ww->buf.new_height = height;
    ww->cursor = ww_base_cursor;
    ww->sync_interval = config.vsync;

    ID3D11Device* device = ww_base.device;

    // swapchain:
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = ww_dxgi_format;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = ww->hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    IDXGIDevice* dxgiDevice = nullptr;
    device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

    IDXGIAdapter* adapter = nullptr;
    dxgiDevice->GetAdapter(&adapter);

    IDXGIFactory* factory = nullptr;
    adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory);

    factory->CreateSwapChain(device, &scd, &ww->swapchain);

    dxgiDevice->Release();
    adapter->Release();
    factory->Release();

    // render target view:
    ID3D11Texture2D* pBackBuffer = nullptr;
    ww->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    if (pBackBuffer == nullptr) return false;

    device->CreateRenderTargetView(pBackBuffer, nullptr, &ww->rtv);
    pBackBuffer->Release();

    ww_list.push_back(ww);
    ww_map[ww->hwnd] = ww;

    if (config.show) ShowWindow(ww->hwnd, SW_SHOWNOACTIVATE);
    if (config.topmost) SetWindowPos(ww->hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    return true;
}
DWORD WINAPI winwin_thread(void* param) {
    auto ww = (winwin*)param;
    auto cohr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    IsGUIThread(true);

    auto& cr = ww->init.rect;
    auto ok = winwin_create_impl(ww, *ww->init.config, cr.x, cr.y, cr.width, cr.height);

    if (ok) {
        ww->mt.section = new CRITICAL_SECTION();
        InitializeCriticalSection(ww->mt.section);
    }
    ww->init.ok = ok;
    SetEvent(ww->mt.ready);
    if (!ok) {
        CoUninitialize();
        return 0;
    }

    MSG msg{};
    while (GetMessage(&msg, ww->hwnd, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();
    return static_cast<DWORD>(msg.wParam);
}
dllg ww_ptr_create winwin_create(int x, int y, int width, int height, winwin_config config) {
    auto ww = new winwin();
    bool ok;
    if (config.thread) {
        //
        auto &ir = ww->init.rect;
        ir.x = x;
        ir.y = y;
        ir.width = width;
        ir.height = height;
        //
        ww->init.config = new winwin_config();
        static_assert(std::is_trivially_copyable_v<winwin_config>, "winwin_config must be trivially copyable");
        memcpy_arr(ww->init.config, &config, 1);
        //
        ww->mt.ready = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        CreateThread(nullptr, 0, winwin_thread, ww, 0, &ww->mt.thread_id);
        WaitForSingleObject(ww->mt.ready, INFINITE);
        ok = ww->init.ok;
    } else {
        ok = winwin_create_impl(ww, config, x, y, width, height);
    }
    if (!ok) {
        delete ww;
        return nullptr;
    }
    return ww;
}
winwin::~winwin() {
    this->mt.enter();
    if (this->hwnd) {
        if (this->mt.section) {
            // just tell it to close, I guess? We don't own the window
            this->close_button = 1;
            PostMessage(this->hwnd, WM_CLOSE, 0, 0);
        } else {
            DestroyWindow(this->hwnd);
        }
        this->hwnd = nullptr;
    }
    //
    if (this->init.config) {
        delete this->init.config;
        this->init.config = nullptr;
    }
    //
    auto& mt = this->mt;
    if (mt.ready) {
        CloseHandle(mt.ready);
        mt.ready = NULL;
    }
    if (mt.thread) {
        CloseHandle(mt.thread);
        mt.thread = NULL;
    }
    //
    this->rtv->Release();
    this->swapchain->Release();
    //
    auto section = this->mt.section;
    if (section) {
        this->mt.section = nullptr;
        LeaveCriticalSection(section);
        DeleteCriticalSection(section);
    }
}

dllx double winwin_draw_end_raw();
dllg void winwin_destroy(ww_ptr_destroy ww) {
    if (ww_target == ww) winwin_draw_end_raw();
    auto n = ww_list.size();
    for (auto i = 0u; i < n; i++) {
        if (ww_list[i] == ww) {
            ww_list.erase(ww_list.begin() + i);
            break;
        }
    }
    ww_map.erase(ww->hwnd);
    DestroyWindow(ww->hwnd);
    delete ww;
}

void ProcessMessages() {
    tagMSG m;
    while (true) {
        if (PeekMessageW(&m, NULL, 0, 0, PM_REMOVE)) {
            if (m.message != WM_QUIT) {
                TranslateMessage(&m);
                DispatchMessageW(&m);
            } else break;
        } else break;
    }
}
dllg void winwin_sleep(int ms, bool process_messages = true) {
    if (!process_messages) {
        Sleep(ms);
        return;
    }
    constexpr int pm_step = 100;
    while (ms > 0) {
        int step;
        if (ms > pm_step) {
            step = pm_step;
            ms -= pm_step;
        } else {
            step = ms;
            ms = 0;
        }
        Sleep(step);
        ProcessMessages();
    }
}