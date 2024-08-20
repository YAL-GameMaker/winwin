#include "stdafx.h"
#include "winwin.h"

LRESULT winwin_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    auto pair = ww_map.find(hwnd);
    auto ww = pair != ww_map.end() ? pair->second : nullptr;
    if (ww) switch (msg) {
        case WM_SETCURSOR: {
            if (LOWORD(lparam) != HTCLIENT) break;
            SetCursor(pair->second->cursor);
            break;
        };
        case WM_SIZE: {
            if (wparam == SIZE_MINIMIZED) {
                ww->buf.resize_in = 0;
            } else {
                if (wparam == SIZE_RESTORED) {
                    ww->buf.resize_in = 1;
                } else ww->buf.resize_in = 5;
                ww->buf.new_width = LOWORD(lparam);
                ww->buf.new_height = HIWORD(lparam);
            }
        };
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}