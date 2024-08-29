#include "stdafx.h"
#include "winwin.h"
#define __RELFILE__ "winwin_draw"

ww_ptr ww_target = nullptr;

constexpr UINT ww_max_rtvs = 4;
struct {
    ID3D11RenderTargetView* rtvs[ww_max_rtvs]{};
    ID3D11DepthStencilView* stencil{};

    CD3D11_VIEWPORT viewports[4]{};
    UINT viewportCount = 0;

    D3D11_RECT scissorRects[4]{};
    UINT scissorCount = 0;
} ww_last;

void winwin_draw_store() {
    auto ctx = ww_base.context;
    ctx->OMGetRenderTargets(ww_max_rtvs, ww_last.rtvs, &ww_last.stencil);

    ww_last.viewportCount = (UINT)std::size(ww_last.viewports);
    ctx->RSGetViewports(&ww_last.viewportCount, ww_last.viewports);

    ww_last.scissorCount = (UINT)std::size(ww_last.scissorRects);
    ctx->RSGetScissorRects(&ww_last.scissorCount, ww_last.scissorRects);
}
void winwin_draw_restore() {
    auto ctx = ww_base.context;
    ctx->OMSetRenderTargets(ww_max_rtvs, ww_last.rtvs, ww_last.stencil);
    ctx->RSSetViewports(ww_last.viewportCount, ww_last.viewports);
    ctx->RSSetScissorRects(ww_last.scissorCount, ww_last.scissorRects);
}

struct {
    int width, height;
} ww_draw;
dllx double winwin_get_draw_width() {
    return ww_draw.width;
}
dllx double winwin_get_draw_height() {
    return ww_draw.height;
}

dllg bool winwin_resize_buffer(ww_ptr ww, int width, int height) {
    if (ww_target == ww) {
        trace("[winwin_resize_buffer] Can't resize buffer while drawing to it");
        return false;
    }
    // same size!
    if (width == ww->buf.width && height == ww->buf.height) return true;

    ww->rtv->Release();
    ww->rtv = nullptr;
    auto hr = ww->swapchain->ResizeBuffers(1, width, height, ww_dxgi_format, 0);
    if (hr != S_OK) {
        trace("[winwin_resize_buffer] ResizeBuffers failed, hr=0x%x", hr);
        return false;
    }

    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = ww->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    if (pBackBuffer == nullptr) {
        trace("[winwin_resize_buffer] GetBuffer failed, hr=0x%x", hr);
        return false;
    }

    ww_base.device->CreateRenderTargetView(pBackBuffer, nullptr, &ww->rtv);
    pBackBuffer->Release();
    ww->buf.width = width;
    ww->buf.height = height;
    ww->buf.resize_in = 0;

    return hr == S_OK;
}

/// ~
dllg bool winwin_draw_start_raw(ww_ptr ww) {
    if (ww_target != nullptr) {
        trace("[winwin_draw_start] Already drawing to a window!");
        return false;
    }
    ww_target = ww;
    auto ctx = ww_base.context;

    // store state:
    winwin_draw_store();

    // get current size:
    #if 0
    RECT rcClient{};
    if (!GetClientRect(ww->hwnd, &rcClient)) return false;
    auto rcWidth = rcClient.right - rcClient.left;
    auto rcHeight = rcClient.bottom - rcClient.top;
    rcWidth = ww->buf.width;
    rcHeight = ww->buf.height;
    ww_draw.width = rcWidth;
    ww_draw.height = rcHeight;
    #endif
    ww_draw.width = ww->buf.width;
    ww_draw.height = ww->buf.height;

    // set targets:
    ID3D11RenderTargetView* targets[ww_max_rtvs]{};
    targets[0] = ww->rtv;
    ctx->OMSetRenderTargets((UINT)std::size(targets), targets, nullptr);

    CD3D11_VIEWPORT vp(0.0f, 0.0f, (float)ww_draw.width, (float)ww_draw.height);
    ctx->RSSetViewports(1, &vp);

    D3D11_RECT scissorRect = { 0, 0, ww_draw.width, ww_draw.height };
    ctx->RSSetScissorRects(1, &scissorRect);

    return true;
}
///~
dllg double winwin_draw_end_raw() {
    auto ww = ww_target;
    if (ww == nullptr) {
        trace("[winwin_draw_end] Not drawing to a window!");
        return false;
    }
    ww_target = nullptr;
    auto ctx = ww_base.context;

    winwin_draw_restore();

    auto hr = ww->swapchain->Present(1, 0);
    if (hr != S_OK) trace("hr %d", hr);
    return hr == S_OK;
}