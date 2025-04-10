#include "stdafx.h"
#include "winwin.h"
#include "winwin_follow_mouse.h"

bool winwin_set_position(ww_ptr ww, int x, int y);
bool winwin_get_position(ww_ptr ww, POINT* p) {
    RECT r{};
    if (!GetClientRect(ww->hwnd, &r)) return false;
    p->x = r.left;
    p->y = r.top;
    return ClientToScreen(ww->hwnd, p);
}
bool winwin_set_position_smart(ww_ptr ww, int x, int y) {
    POINT p;
    if (!winwin_get_position(ww, &p)) return false;
    if (p.x != x || p.y != y) {
        return winwin_set_position(ww, x, y);
    } else return true;
}

DWORD WINAPI winwin_follow_mouse_thread(void* _init) {
    auto init = (winwin_follow_mouse_params*)_init;
    auto ww = init->ww;
    while (IsWindow(ww->hwnd)) {
        if (IsWindowVisible(ww->hwnd)) {
            POINT mousePos;
            if (GetCursorPos(&mousePos)) {
                winwin_set_position_smart(ww, mousePos.x - init->xoffset, mousePos.y - init->yoffset);
            }
        }
        Sleep(init->sleep_time);
    }
    return 0;
}
dllg int winwin_follow_mouse_start(ww_ptr ww, int xoffset, int yoffset, int sleep_time = 4) {
    winwin_follow_mouse_params* init;
    auto tweak = ww->follow_mouse.kind == 1;
    if (tweak) {
        init = (winwin_follow_mouse_params*)ww->follow_mouse.param;
    } else {
        ww->follow_mouse.stop();
        init = new winwin_follow_mouse_params();
    }
    init->ww = ww;
    init->xoffset = xoffset;
    init->yoffset = yoffset;
    init->sleep_time = sleep_time;
    if (tweak) return 0;
    DWORD thread_id;
    auto thread = CreateThread(NULL, 0, winwin_follow_mouse_thread, init, 0, &thread_id);
    if (!thread) {
        delete init;
        return -1;
    } else {
        ww->follow_mouse.thread = thread;
        ww->follow_mouse.kind = 1;
        ww->follow_mouse.param = init;
        return 1;
    }
}

DWORD WINAPI winwin_follow_mouse_snapped_thread(void* _init) {
    auto init = (winwin_follow_mouse_snapped_params*)_init;
    auto ww = init->ww;
    while (IsWindow(ww->hwnd)) {
        if (IsWindowVisible(ww->hwnd)) {
            POINT mousePos;
            if (GetCursorPos(&mousePos)) {
                int mx = (int)(init->xstart + floor((mousePos.x - init->xstart) / init->xsnap) * init->xsnap);
                int my = (int)(init->ystart + floor((mousePos.y - init->ystart) / init->ysnap) * init->ysnap);
                winwin_set_position_smart(ww, mx - init->xoffset, my - init->yoffset);
            }
        }
        Sleep(init->sleep_time);
    }
    return 0;
}
dllg int winwin_follow_mouse_start_snapped(ww_ptr ww,
    int xstart, int ystart, double xsnap, double ysnap,
    int xoffset, int yoffset, int sleep_time = 4
) {
    winwin_follow_mouse_snapped_params* init;
    auto tweak = ww->follow_mouse.kind == 2;
    if (tweak) {
        init = (winwin_follow_mouse_snapped_params*)ww->follow_mouse.param;
    } else {
        ww->follow_mouse.stop();
        init = new winwin_follow_mouse_snapped_params();
    }
    init->ww = ww;
    init->xoffset = xoffset;
    init->yoffset = yoffset;
    init->xstart = xstart;
    init->ystart = ystart;
    init->xsnap = xsnap;
    init->ysnap = ysnap;
    init->sleep_time = sleep_time;
    if (tweak) return 0;
    DWORD thread_id;
    auto thread = CreateThread(NULL, 0, winwin_follow_mouse_snapped_thread, init, 0, &thread_id);
    if (!thread) {
        delete init;
        return -1;
    } else {
        ww->follow_mouse.thread = thread;
        ww->follow_mouse.kind = 2;
        ww->follow_mouse.param = init;
        return 1;
    }
}

dllg bool winwin_follow_mouse_end(ww_ptr ww) {
    return ww->follow_mouse.stop();
}