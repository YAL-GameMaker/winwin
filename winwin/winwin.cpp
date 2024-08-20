/// @author YellowAfterlife

#include "stdafx.h"
#include "winwin.h"

std::vector<winwin*> ww_list{};
std::unordered_map<HWND, winwin*> ww_map{};
wm_base_t ww_base{};
StringConv ww_c1, ww_c2;
HCURSOR ww_base_cursor;

LRESULT winwin_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

constexpr wchar_t winwin_class[] = L"winwin_class";
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
    RegisterClass(&wndc);
    //
    ww_base_cursor = LoadCursor(NULL, IDC_ARROW);
    //
    return 1;
}

dllg ww_ptr winwin_create(int x, int y, int width, int height, const char* caption, int border) {
    auto ww = new winwin();
    DWORD exStyle = 0;
    DWORD dwStyle;
    if (border <= 0) {
        dwStyle = WS_POPUP;
    } else {
        dwStyle = WS_OVERLAPPEDWINDOW;
        if (border < 2) {
            dwStyle &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
        }
    }
    //
    ww->buf.width = width;
    ww->buf.height = height;
    ww->buf.new_width = width;
    ww->buf.new_height = height;
    RECT rcClient = { x, y, x + width, y + height };
    AdjustWindowRectEx(&rcClient, dwStyle, false, exStyle);
    width = rcClient.right - rcClient.left;
    height = rcClient.bottom - rcClient.top;
    //
    ww->hwnd = CreateWindowExW(
        exStyle,
        winwin_class,
        ww_cc(caption),
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

    ShowWindow(ww->hwnd, SW_SHOW);
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

bool winwin_resize_buffer(ww_ptr ww, int width, int height);
dllg void winwin_update() {
    for (auto ww : ww_list) {
        if (ww->buf.resize_in > 0 && --ww->buf.resize_in <= 0) {
            winwin_resize_buffer(ww, ww->buf.new_width, ww->buf.new_height);
        }
    }
}