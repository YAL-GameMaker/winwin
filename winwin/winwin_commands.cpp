#include "stdafx.h"
#include "winwin.h"

//
dllg bool winwin_get_topmost(ww_ptr ww) {
    auto hwnd = ww->hwnd;
    return (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;
}
dllg bool winwin_set_topmost(ww_ptr ww, bool enable) {
    auto hwnd = ww->hwnd;
    SetWindowPos(hwnd, enable ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    return true;
}

// todo: https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nf-shobjidl_core-itaskbarlist-addtab
dllg bool winwin_get_taskbar_button_visible(ww_ptr ww) {
    return (GetWindowLong(ww->hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) == 0;
}
dllg bool winwin_set_taskbar_button_visible(ww_ptr ww, bool show_button) {
    auto hwnd = ww->hwnd;
    auto exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (show_button) {
        exStyle &= ~WS_EX_TOOLWINDOW;
    } else exStyle |= WS_EX_TOOLWINDOW;
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
    return true;
}

// https://stackoverflow.com/a/50245502
constexpr LONG MY_GWL_CLICKTHROUGH = (WS_EX_TRANSPARENT | WS_EX_LAYERED);
dllg bool winwin_get_clickthrough(ww_ptr ww) {
    auto hwnd = ww->hwnd;
    return (GetWindowLong(hwnd, GWL_EXSTYLE) & MY_GWL_CLICKTHROUGH) == MY_GWL_CLICKTHROUGH;
}
dllg bool winwin_set_clickthrough(ww_ptr ww, bool enable_clickthrough) {
    auto hwnd = ww->hwnd;
    auto exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (enable_clickthrough) {
        exStyle |= MY_GWL_CLICKTHROUGH;
    } else exStyle &= ~MY_GWL_CLICKTHROUGH;
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
    return true;
}

//
dllg bool winwin_get_noactivate(ww_ptr ww) {
    auto hwnd = ww->hwnd;
    return (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_NOACTIVATE) == WS_EX_NOACTIVATE;
}
dllg bool winwin_set_noactivate(ww_ptr ww, bool disable_activation) {
    auto hwnd = ww->hwnd;
    auto exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (disable_activation) {
        exStyle |= WS_EX_NOACTIVATE;
    } else exStyle &= ~WS_EX_NOACTIVATE;
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
    return true;
}

dllg bool winwin_set_visible(ww_ptr ww, bool visible) {
    auto hwnd = ww->hwnd;
    ShowWindow(hwnd, visible ? SW_SHOW : SW_HIDE);
    return true;
}