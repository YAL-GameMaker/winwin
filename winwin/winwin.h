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

struct ww_keybits {
    uint32_t segments[8]{};
    void clear() {
        memset(segments, 0, sizeof(segments));
    }
    void assign(ww_keybits& other) {
        memcpy(segments, other.segments, sizeof(segments));
    }
    bool get(int i) {
        if (i < 0 || i >= 256) return false;
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

struct winwin {
    HWND hwnd = NULL;
    IDXGISwapChain* swapchain = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;
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
    int sync_interval = 0;
    int8_t close_button = 0;
    //
    ww_size minSize{}, maxSize{};
};
using ww_ptr = gml_ptr<winwin>;

extern std::vector<winwin*> ww_list;
extern std::unordered_map<HWND, winwin*> ww_map;
extern ww_ptr ww_main;
extern ww_ptr ww_target;

constexpr DXGI_FORMAT ww_dxgi_format = DXGI_FORMAT_R8G8B8A8_UNORM;
constexpr LONG WW_WS_EX_CLICKTHROUGH = (WS_EX_TRANSPARENT | WS_EX_LAYERED);

#define ww_cc(str) ww_c1.proc(str)
inline LONG rect_width(RECT& r) { return r.right - r.left; }
inline LONG rect_height(RECT& r) { return r.bottom - r.top; }

///
enum class winwin_kind {
    normal,
    borderless,
    tool,
};
struct winwin_config {
    const char* caption;
    winwin_kind kind;
    bool resize;
    bool show;
    bool topmost;
    bool taskbar_button;
    bool clickthrough;
    bool noactivate;
    int vsync;
    int8_t close_button;
};

using ww_ptr_create = ww_ptr;
/**
    @dllg:type ww_ptr_create
    @gmlRead
    var _ptr = buffer_read(_buf, buffer_u64);
    var _box;
    if (_ptr != 0) {
        _ptr = ptr(_ptr);
        _box = new winwin(_ptr);
        global.__winwin_map[?_ptr] = _box;
    } else _box = undefined;
    return _box;
**/

using ww_ptr_find = ww_ptr;
/**
    @dllg:type ww_ptr_find
    @gmlRead
    var _ptr = buffer_read(_buf, buffer_u64);
    var _box;
    if (_ptr != 0) {
        _ptr = ptr(_ptr);
        _box = global.__winwin_map[?_ptr];
        if (_box == undefined) {
            _box = new winwin(_ptr);
            global.__winwin_map[?_ptr] = _box;
        }
    } else _box = undefined;
    return _box;
**/

using ww_ptr_destroy = gml_ptr_destroy<winwin>;
/**
    @dllg:type ww_ptr_destroy
    @gmlWrite
    var _box_0 = $value;
    if (instanceof(_box_0) != "winwin") { show_error("Expected a winwin, got " + string(_box_0), true); exit }
    var _ptr_0 = _box_0.__ptr__;
    if (_ptr_0 == pointer_null) { show_error("This winwin is already destroyed.", true); exit; }
    _box_0.__ptr__ = pointer_null;
    ds_map_delete(global.__winwin_map, _ptr_0);
    buffer_write(_buf, buffer_u64, int64(_ptr_0));
**/
