#include "stdafx.h"
#include "winwin.h"
#include <windowsx.h>
#define __RELFILE__ "winwin_wndproc"

LRESULT winwin_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    auto ww = ww_find(hwnd);
    if (!ww) { // ..?
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    switch (msg) {
        case WM_SETCURSOR: {
            if (LOWORD(lparam) != HTCLIENT) break;
            SetCursor(ww->cursor);
        }; break;

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
        }; break;
        case WM_GETMINMAXINFO: {
            RECT rc{};
            GetClientRect(hwnd, &rc);
            if (rect_width(rc) > 0) {
                RECT rw{};
                GetWindowRect(hwnd, &rw);
                auto dx = rect_width(rw) - rc.left;
                auto dy = rect_height(rw) - rc.top;

                auto inf = (MINMAXINFO*)lparam;
                if (ww->minSize.width)  inf->ptMinTrackSize.x = dx + *ww->minSize.width;
                if (ww->minSize.height) inf->ptMinTrackSize.y = dy + *ww->minSize.height;
                if (ww->maxSize.width)  inf->ptMaxTrackSize.x = dx + *ww->maxSize.width;
                if (ww->maxSize.height) inf->ptMaxTrackSize.y = dy + *ww->maxSize.height;
            }
        }; break;

        case WM_SYSCOMMAND: {
            if (wparam == SC_CLOSE && ww->close_button == 2) {
                ShowWindow(hwnd, SW_HIDE);
                return TRUE;
            }
        }; break;
        
        // mouse:
        case WM_MOUSEMOVE: {
            ww->mouse_x = GET_X_LPARAM(lparam);
            ww->mouse_y = GET_Y_LPARAM(lparam);
            if (!ww->mouse_tracking) {
                TRACKMOUSEEVENT tme{};
                tme.cbSize = sizeof(tme);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hwnd;
                ww->mouse_tracking = TrackMouseEvent(&tme);
                //trace("Enter");
                if (ww->mouse_tracking) {
                    ww->mouse_over = true;
                }
            }
        }; break;
        case WM_MOUSELEAVE: {
            ww->mouse_tracking = false;
            //trace("Leave");
            ww->mouse_over = false;
        }; break;
        case WM_LBUTTONDOWN: case WM_RBUTTONDOWN: case WM_MBUTTONDOWN: {
            uint8_t flag = 0;
            switch (msg) {
                case WM_LBUTTONDOWN: flag = 1; break;
                case WM_RBUTTONDOWN: flag = 2; break;
                case WM_MBUTTONDOWN: flag = 3; break;
            }
            if (!ww->mouse_next.down.get(flag)) {
                ww->mouse_next.down.set(flag, true);
                ww->mouse_next.pressed.set(flag, true);
            }
        }; break;
        case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP:
        case WM_NCLBUTTONUP: case WM_NCRBUTTONUP: case WM_NCMBUTTONUP: {
            uint8_t flag = 0;
            switch (msg) {
                case WM_LBUTTONUP: case WM_NCLBUTTONUP: flag = 1; break;
                case WM_RBUTTONUP: case WM_NCRBUTTONUP: flag = 2; break;
                case WM_MBUTTONUP: case WM_NCMBUTTONUP: flag = 3; break;
            }
            if (ww->mouse_next.down.get(flag)) {
                ww->mouse_next.down.set(flag, false);
                ww->mouse_next.released.set(flag, true);
            }
        }; break;
        case WM_MOUSEWHEEL: {
            ww->mouse_next.wheel += HIWORD(wparam);
        }; break;
        case WM_MOUSEHWHEEL: {
            ww->mouse_next.hwheel += HIWORD(wparam);
        }; break;

        // keyboard:
        case WM_KEYDOWN: case WM_SYSKEYDOWN: {
            auto key = (int)wparam;
            if (key < 256 && !ww->keys_next.down.get(key)) {
                ww->keys_next.down.set(key, true);
                ww->keys_next.pressed.set(key, true);
            }
        }; break;
        case WM_KEYUP: case WM_SYSKEYUP: {
            auto key = (int)wparam;
            if (key < 256 && ww->keys_next.down.get(key)) {
                ww->keys_next.down.set(key, false);
                ww->keys_next.released.set(key, true);
            }
        }; break;
        case WM_CHAR: {
            void winwin_keyboard_string_proc(ww_ptr ww, uint32_t c);
            winwin_keyboard_string_proc(ww, (uint32_t)wparam);
        }; break;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}