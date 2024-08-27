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

dllg ww_ptr_create winwin_create(int x, int y, int width, int height, winwin_config config) {
    auto ww = new winwin();
    //
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
    ww->buf.width = width;
    ww->buf.height = height;
    ww->buf.new_width = width;
    ww->buf.new_height = height;
    RECT rcClient = { x, y, x + width, y + height };
    AdjustWindowRectEx(&rcClient, dwStyle, false, dwExStyle);
    width = rcClient.right - rcClient.left;
    height = rcClient.bottom - rcClient.top;
    //
    ww->hwnd = CreateWindowExW(
        dwExStyle,
        winwin_class,
        ww_cc(config.caption),
        dwStyle,
        x, y, width, height,
        nullptr, nullptr, ww_base.hInstance, nullptr
    );
    ww->cursor = ww_base_cursor;

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

    if (config.show) ShowWindow(ww->hwnd, SW_SHOW);
    if (config.topmost) SetWindowPos(ww->hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    return ww;
}

double winwin_draw_end_raw();
dllg void winwin_destroy(gml_ptr_destroy<winwin> ww) {
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
    // todo: destroy swapchain
}

