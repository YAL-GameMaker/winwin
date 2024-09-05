#include "stdafx.h"
#include "winwin.h"
#define __RELFILE__ "winwin_props"

dllx double winwin_exists_raw(winwin* ww) {
    return IsWindow(ww->hwnd);
}
dllg uintptr_t winwin_get_handle(ww_ptr ww) {
    return (uintptr_t)ww->hwnd;
}

dllg const char* winwin_get_caption(ww_ptr ww) {
    auto len = GetWindowTextLengthW(ww->hwnd);
    if (len <= 0) return "";
    auto ws = ww_c1.wget(len + 1);
    memset(ws, 0, sizeof(wchar_t) * (len + 1));
    GetWindowTextW(ww->hwnd, ws, len + 1);
    return ww_cc(ws);
}
dllg bool winwin_set_caption(ww_ptr ww, const char* caption) {
    return SetWindowTextW(ww->hwnd, ww_cc(caption));
}

dllg int8_t winwin_get_close_button(ww_ptr ww) {
    return ww->close_button;
}
dllg bool winwin_set_close_button(ww_ptr ww, int8_t close_button_state) {
    auto hwnd = ww->hwnd;
    if (!hwnd) return false;
    auto curr = ww->close_button;
    if (curr == close_button_state) return true;
    ww->close_button = close_button_state;
    if ((curr == 0) != (close_button_state == 0)) {
        auto menu = GetSystemMenu(hwnd, false);
        return EnableMenuItem(menu, SC_CLOSE, MF_BYCOMMAND | (close_button_state == 0 ? MF_GRAYED : MF_ENABLED));
    }
    return true;
}

dllg int8_t winwin_get_vsync(ww_ptr ww) {
    return ww->sync_interval;
}
dllg void winwin_set_vsync(ww_ptr ww, int sync_interval) {
    ww->sync_interval = sync_interval;
}