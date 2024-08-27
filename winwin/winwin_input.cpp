#include "stdafx.h"
#include "winwin.h"

dllg bool winwin_keyboard_check(ww_ptr ww, int key) {
	return ww->keys.down.get(key);
}
dllg bool winwin_keyboard_check_pressed(ww_ptr ww, int key) {
	return ww->keys.pressed.get(key);
}
dllg bool winwin_keyboard_check_released(ww_ptr ww, int key) {
	return ww->keys.released.get(key);
}

dllg int winwin_mouse_get_x(ww_ptr ww) {
	return ww->mouse_x;
}
dllg int winwin_mouse_get_y(ww_ptr ww) {
	return ww->mouse_y;
}

dllg bool winwin_mouse_check_button(ww_ptr ww, int button) {
	return ww->mouse.down.get(button);
}
dllg bool winwin_mouse_check_button_pressed(ww_ptr ww, int button) {
	return ww->mouse.pressed.get(button);
}
dllg bool winwin_mouse_check_button_released(ww_ptr ww, int button) {
	return ww->mouse.released.get(button);
}