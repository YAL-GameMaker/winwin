#include "stdafx.h"
#include "winwin.h"

// getters:
#define winwin_def_rect_ret {}
dllg std::optional<int> winwin_get_x(ww_ptr ww) {
    RECT r{};
    if (!GetClientRect(ww->hwnd, &r)) return winwin_def_rect_ret;
    POINT p = { r.left, r.top };
    if (!ClientToScreen(ww->hwnd, &p)) return winwin_def_rect_ret;
    return p.x;
}
dllg std::optional<int> winwin_get_y(ww_ptr ww) {
    RECT r{};
    if (!GetClientRect(ww->hwnd, &r)) return winwin_def_rect_ret;
    POINT p = { r.left, r.top };
    if (!ClientToScreen(ww->hwnd, &p)) return winwin_def_rect_ret;
    return p.y;
}
dllg std::optional<int> winwin_get_width(ww_ptr ww) {
    RECT r{};
    if (GetClientRect(ww->hwnd, &r)) return rect_width(r);
    return winwin_def_rect_ret;
}
dllg std::optional<int> winwin_get_height(ww_ptr ww) {
    RECT r{};
    if (GetClientRect(ww->hwnd, &r)) return rect_height(r);
    return winwin_def_rect_ret;
}

// setters:
dllg bool winwin_set_position(ww_ptr ww, int x, int y) {
    RECT r{};
    if (!GetClientRect(ww->hwnd, &r)) return false;
    POINT p = { r.left, r.top };
    if (!ClientToScreen(ww->hwnd, &p)) return false;
    //
    if (!GetWindowRect(ww->hwnd, &r)) return false;
    int nx = r.left + (x - p.x);
    int ny = r.top + (y - p.y);
    //
    return SetWindowPos(ww->hwnd, 0, nx, ny, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

dllg bool winwin_set_size(ww_ptr ww, int width, int height) {
    auto hwnd = ww->hwnd;

    auto dwStyle = GetWindowLong(hwnd, GWL_STYLE);
    auto dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    auto hasMenu = GetMenu(hwnd) != NULL;
    RECT r = { 0, 0, width, height };
    if (!AdjustWindowRectEx(&r, dwStyle, hasMenu, dwExStyle)) return false;
    auto adjWidth = rect_width(r);
    auto adjHeight = rect_height(r);

    return SetWindowPos(hwnd, NULL, 0, 0, adjWidth, adjHeight, SWP_NOZORDER | SWP_NOMOVE);
}

dllg bool winwin_set_rectangle(ww_ptr ww, int x, int y, int width, int height) {
    auto hwnd = ww->hwnd;

    auto dwStyle = GetWindowLong(hwnd, GWL_STYLE);
    auto dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    auto hasMenu = GetMenu(hwnd) != NULL;
    RECT r = { x, y, width, height };
    if (!AdjustWindowRectEx(&r, dwStyle, hasMenu, dwExStyle)) return false;
    auto adjWidth = rect_width(r);
    auto adjHeight = rect_height(r);

    return SetWindowPos(hwnd, NULL, r.left, r.top, adjWidth, adjHeight, SWP_NOZORDER | SWP_NOMOVE);
}

// min/max:
dllg std::optional<int> window_get_min_width(ww_ptr ww) {
    return ww->minSize.width;
}
dllg std::optional<int> window_get_min_height(ww_ptr ww) {
    return ww->minSize.height;
}
dllg std::optional<int> window_get_max_width(ww_ptr ww) {
    return ww->maxSize.width;
}
dllg std::optional<int> window_get_max_height(ww_ptr ww) {
    return ww->maxSize.height;
}

dllg bool window_set_min_width(ww_ptr ww, std::optional<int> min_width = {}) {
    ww->minSize.width = min_width;
    return true;
}
dllg bool window_set_min_height(ww_ptr ww, std::optional<int> min_height = {}) {
    ww->minSize.height = min_height;
    return true;
}
dllg bool window_set_max_width(ww_ptr ww, std::optional<int> max_width = {}) {
    ww->maxSize.width = max_width;
    return true;
}
dllg bool window_set_max_height(ww_ptr ww, std::optional<int> max_height = {}) {
    ww->maxSize.height = max_height;
    return true;
}