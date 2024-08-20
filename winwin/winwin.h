#pragma once
#include "stdafx.h"
#include <vector>
#include <unordered_map>
#include "StringConv.h"

struct wm_base_t {
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    HWND main_hwnd;
    IDXGISwapChain* main_swapchain;
    HINSTANCE hInstance;
};
extern wm_base_t ww_base;

struct winwin {
    HWND hwnd = NULL;
    IDXGISwapChain* swapchain = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;
    HCURSOR cursor = NULL;
    int mouse_x = 0, mouse_y = 0;
    struct {
        int width, height;
        int new_width, new_height;
        int resize_in = 0;
    } buf;
};
using ww_ptr = gml_ptr<winwin>;

extern std::vector<winwin*> ww_list;
extern std::unordered_map<HWND, winwin*> ww_map;
extern ww_ptr ww_target;

constexpr DXGI_FORMAT ww_dxgi_format = DXGI_FORMAT_R8G8B8A8_UNORM;

#define ww_cc(str) ww_c1.proc(str)
