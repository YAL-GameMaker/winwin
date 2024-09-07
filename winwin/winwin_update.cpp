#include "stdafx.h"
#include "winwin.h"

bool winwin_resize_buffer(ww_ptr ww, int width, int height);
dllg void winwin_update() {
    for (auto ww : ww_list) {
        ww->mt.enter();
        if (ww->buf.resize_in > 0 && --ww->buf.resize_in <= 0) {
            winwin_resize_buffer(ww, ww->buf.new_width, ww->buf.new_height);
        }
        //
        ww->keys.down.assign(ww->keys_next.down);
        ww->keys.pressed.assign(ww->keys_next.pressed);
        ww->keys_next.pressed.clear();
        ww->keys.released.assign(ww->keys_next.released);
        ww->keys_next.released.clear();
        //
        ww->mouse.down.assign(ww->mouse_next.down);
        ww->mouse.pressed.assign(ww->mouse_next.pressed);
        ww->mouse_next.pressed.clear();
        ww->mouse.released.assign(ww->mouse_next.released);
        ww->mouse_next.released.clear();
        ww->mouse.wheel = ww->mouse_next.wheel;
        ww->mouse_next.wheel = 0;
        ww->mouse.hwheel = ww->mouse_next.hwheel;
        ww->mouse_next.hwheel = 0;
        //
        ww->mt.leave();
    }
}