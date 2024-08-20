#include "stdafx.h"
#include "winwin.h"
#define __RELFILE__ "winwin_props"

dllg bool winwin_exists(ww_ptr ww) {
    return IsWindow(ww->hwnd);
}

// size:
dllg std::optional<int> winwin_get_x(ww_ptr ww) {
    RECT r{};
    if (!GetClientRect(ww->hwnd, &r)) return {};
    POINT p = { r.left, r.top };
    if (!ClientToScreen(ww->hwnd, &p)) return {};
    return p.x;
}
dllg std::optional<int> winwin_get_y(ww_ptr ww) {
    RECT r{};
    if (!GetClientRect(ww->hwnd, &r)) return {};
    POINT p = { r.left, r.top };
    if (!ClientToScreen(ww->hwnd, &p)) return {};
    return p.y;
}
dllg std::optional<int> winwin_get_width(ww_ptr ww) {
    RECT r{};
    if (GetClientRect(ww->hwnd, &r)) return r.right - r.left;
    return {};
}
dllg std::optional<int> winwin_get_height(ww_ptr ww) {
    RECT r{};
    if (GetClientRect(ww->hwnd, &r)) return r.bottom - r.top;
    return {};
}