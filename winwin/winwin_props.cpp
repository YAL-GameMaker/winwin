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
    wchar_t* ws = ww_c1.wget(len + 1);
    memset(ws, 0, sizeof(wchar_t) * (len + 1));
    GetWindowTextW(ww->hwnd, ws, len + 1);
    return ww_cc(ws);
}
dllg bool winwin_set_caption(ww_ptr ww, const char* caption) {
    return SetWindowTextW(ww->hwnd, ww_cc(caption));
}