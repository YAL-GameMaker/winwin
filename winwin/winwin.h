#pragma once
#include "stdafx.h"
#include <vector>
#include <unordered_map>
#include "StringConv.h"

struct winwin;
struct wm_base_t {
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    HWND main_hwnd;
    IDXGISwapChain* main_swapchain;
    HINSTANCE hInstance;
    winwin* ref;
};
extern wm_base_t ww_base;

///
enum class winwin_kind {
    normal,
    borderless,
    tool,
};

struct ww_keybits {
    uint32_t segments[8]{};
    void clear() {
        memset(segments, 0, sizeof(segments));
    }
    void assign(ww_keybits& other) {
        memcpy(segments, other.segments, sizeof(segments));
    }
    bool getAny() {
        for (auto i = 0u; i < std::size(segments); i++) {
            if (segments[i] != 0) return true;
        }
        return false;
    }
    bool get(int i) {
        if (i < 0 || i >= 256) return false;
        if (i == 0) return !getAny();
        if (i == 1) return getAny();
        return ((segments[i >> 5] >> (i & 31)) & 1) != 0;
    }
    bool set(int i, bool val) {
        if (i < 0 || i >= 256) return false;
        if (val) {
            segments[i >> 5] |= 1 << (i & 31);
        } else {
            segments[i >> 5] &= ~(1 << (i & 31));
        }
        return true;
    }
};
struct ww_keybits_tri {
    ww_keybits down{};
    ww_keybits pressed{};
    ww_keybits released{};
};
struct ww_keyboard_string {
    int size = 0;
    int capacity = 0;
    uint32_t* data = nullptr;
    ww_keyboard_string() {
        capacity = 128;
        data = malloc_arr<uint32_t>(capacity);
    }
    ~ww_keyboard_string() {
        free(data);
    }
};

struct ww_mousebits {
    uint8_t bits = 0;
    void clear() {
        bits = 0;
    }
    void assign(ww_mousebits& other) {
        bits = other.bits;
    }
    bool get(int i) {
        if (i < 0 || i >= 8) return false;
        return (bits & (1 << i)) != 0;
    }
    bool set(int i, bool val) {
        if (i < 0 || i >= 8) return false;
        if (val) {
            bits |= (1 << i);
        } else {
            bits &= ~(1 << i);
        }
        return true;
    }
};
struct ww_mousebits_tri {
    ww_mousebits down{}, pressed{}, released{};
    int wheel = 0, hwheel = 0;
};
struct ww_size {
    std::optional<int> width = {}, height = {};
};

struct winwin_config;
struct winwin {
    HWND hwnd = NULL;
    IDXGISwapChain* swapchain = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;
    //
    winwin_kind kind = winwin_kind::normal;
    bool has_shadow = true;
    //
    struct {
        int width, height;
        int new_width, new_height;
        int resize_in = 0;
    } buf;
    //
    ww_keybits_tri keys{}, keys_next{};
    ww_keyboard_string keyboard_string{};
    //
    HCURSOR cursor = NULL;
    int mouse_x = 0, mouse_y = 0;
    ww_mousebits_tri mouse{}, mouse_next{};
    bool mouse_tracking = false;
    bool mouse_over = false;
    //
    ww_size minSize{}, maxSize{};
    //
    int8_t sync_interval = 0;
    int8_t close_button = 0;
    // threading
    struct {
        winwin_config* config = nullptr;
        struct { int x, y, width, height; } rect;
        bool ok = false;
    } init;
    struct {
        HANDLE thread = NULL;
        DWORD thread_id = 0;
        CRITICAL_SECTION* section = nullptr;
        HANDLE ready = NULL;
        void enter() {
            if (section) EnterCriticalSection(section);
        }
        void leave() {
            if (section) LeaveCriticalSection(section);
        }
    } mt;
    //
    ~winwin();
};
using ww_ptr = gml_ptr<winwin>;

extern std::vector<winwin*> ww_list;
extern std::unordered_map<HWND, winwin*> ww_map;
inline winwin* ww_find(HWND hwnd) {
    if (hwnd == NULL) return nullptr;
    auto pair = ww_map.find(hwnd);
    return pair != ww_map.end() ? pair->second : nullptr;
}
extern ww_ptr ww_main;
extern ww_ptr ww_target;

constexpr DXGI_FORMAT ww_dxgi_format = DXGI_FORMAT_R8G8B8A8_UNORM;
constexpr LONG WW_WS_EX_CLICKTHROUGH = (WS_EX_TRANSPARENT | WS_EX_LAYERED);

#define ww_cc(str) ww_c1.proc(str)
inline LONG rect_width(RECT& r) { return r.right - r.left; }
inline LONG rect_height(RECT& r) { return r.bottom - r.top; }

using ww_ptr_create = ww_ptr;
using ww_ptr_find = ww_ptr;
using ww_ptr_destroy = gml_ptr_destroy<winwin>;
