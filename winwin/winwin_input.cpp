#include "stdafx.h"
#include "winwin.h"

/** @dllg:gmlheader if (argument0 == winwin_main) return keyboard_check(argument1); */
dllg bool winwin_keyboard_check(ww_ptr ww, int key) {
	return ww->keys.down.get(key);
}
/** @dllg:gmlheader if (argument0 == winwin_main) return keyboard_check_pressed(argument1); */
dllg bool winwin_keyboard_check_pressed(ww_ptr ww, int key) {
	return ww->keys.pressed.get(key);
}
/** @dllg:gmlheader if (argument0 == winwin_main) return keyboard_check_released(argument1); */
dllg bool winwin_keyboard_check_released(ww_ptr ww, int key) {
	return ww->keys.released.get(key);
}

// // // keyboard string
void winwin_keyboard_string_proc(ww_ptr ww, uint32_t c) {
	auto& wks = ww->keyboard_string;
	if (wks.size >= wks.capacity) {
		wks.size = 0;
	}
	if (c == 8) {
		if (wks.size > 0) wks.size -= 1;
	} else if (c >= 32) {
		wks.data[wks.size++] = c;
	}
}
/** @dllg:gmlheader if (argument0 == winwin_main) return keyboard_string; */
dllg const char* winwin_keyboard_get_string(ww_ptr ww) {
	static std::vector<uint8_t> result{};
	auto& wks = ww->keyboard_string;
	auto size = wks.size;
	auto chars = wks.data;
	result.resize(size * 4 + 1);
	auto r = result.data();
	for (int i = 0; i < size; i++) {
		auto c = chars[i];
		if (c < 128) {
			r[0] = (char)c;
			r += 1;
		} else if (c < 2048) {
			r[0] = 0xC0 + ((c >> 6) & 0x1F);
			r[1] = 0x80 + ( c       & 0x3F);
			r += 2;
		} else if (c < 65536) {
			r[0] = 0xE0 + ((c >> 12) & 0x0F);
			r[1] = 0x80 + ((c >>  6) & 0x3F);
			r[2] = 0x80 + ( c        & 0x3F);
			r += 3;
		} else {
			r[0] = 0xF0 + ((c >> 18) & 0x07);
			r[1] = 0x80 + ((c >> 12) & 0x3F);
			r[2] = 0x80 + ((c >>  6) & 0x3F);
			r[3] = 0x80 + ( c        & 0x3F);
			r += 4;
		}
	}
	r[0] = 0;
	return (const char*)result.data();
}
/// ~
dllg int winwin_keyboard_set_string_raw(ww_ptr ww, gml_buffer buf) {
	auto& wks = ww->keyboard_string;
	auto n = buf.tell() >> 2;
	if (n > wks.capacity) n = wks.capacity;
	memcpy_arr(wks.data, (uint32_t*)buf.data(), n);
	return n;
}

// // // mouse

dllg bool winwin_mouse_is_over(ww_ptr ww) {
	return ww->mouse_over;
}

/** @dllg:gmlheader if (argument0 == winwin_main) return window_mouse_get_x(); */
dllg int winwin_mouse_get_x(ww_ptr ww) {
	return ww->mouse_x;
}
/** @dllg:gmlheader if (argument0 == winwin_main) return window_mouse_get_y(); */
dllg int winwin_mouse_get_y(ww_ptr ww) {
	return ww->mouse_y;
}

/** @dllg:gmlheader if (argument0 == winwin_main) return mouse_check_button(argument1); */
dllg bool winwin_mouse_check_button(ww_ptr ww, int button) {
	return ww->mouse.down.get(button);
}
/** @dllg:gmlheader if (argument0 == winwin_main) return mouse_check_button_pressed(argument1); */
dllg bool winwin_mouse_check_button_pressed(ww_ptr ww, int button) {
	return ww->mouse.pressed.get(button);
}
/** @dllg:gmlheader if (argument0 == winwin_main) return mouse_check_button_released(argument1); */
dllg bool winwin_mouse_check_button_released(ww_ptr ww, int button) {
	return ww->mouse.released.get(button);
}

/** @dllg:gmlheader if (argument0 == winwin_main) return mouse_wheel_up(); */
dllg bool winwin_mouse_wheel_up(ww_ptr ww) {
	return ww->mouse.wheel < 0;
}
/** @dllg:gmlheader if (argument0 == winwin_main) return mouse_wheel_down(); */
dllg bool winwin_mouse_wheel_down(ww_ptr ww) {
	return ww->mouse.wheel > 0;
}
/** @dllg:gmlheader if (argument0 == winwin_main) return 0; */
dllg int winwin_mouse_wheel_get_delta_x(ww_ptr ww) {
	return ww->mouse.hwheel;
}
/** @dllg:gmlheader if (argument0 == winwin_main) return mouse_wheel_down() - mouse_wheel_up(); */
dllg int winwin_mouse_wheel_get_delta_y(ww_ptr ww) {
	return ww->mouse.wheel;
}

dllg void winwin_keyboard_clear(ww_ptr ww, int key) {
	ww->keys.down.set(key, false);
	ww->keys.pressed.set(key, false);
	ww->keys.released.set(key, false);
	//
	ww->keys_next.down.set(key, false);
	ww->keys_next.pressed.set(key, false);
	ww->keys_next.released.set(key, false);
}
dllg void winwin_mouse_clear(ww_ptr ww, int button) {
	ww->mouse.down.set(button, false);
	ww->mouse.pressed.set(button, false);
	ww->mouse.released.set(button, false);
	//
	ww->mouse_next.down.set(button, false);
	ww->mouse_next.pressed.set(button, false);
	ww->mouse_next.released.set(button, false);
}
dllg void winwin_io_clear(ww_ptr ww) {
	ww->keys.down.clear();
	ww->keys.pressed.clear();
	ww->keys.released.clear();
	//
	ww->keys_next.down.clear();
	ww->keys_next.pressed.clear();
	ww->keys_next.released.clear();
	//
	ww->mouse.down.clear();
	ww->mouse.pressed.clear();
	ww->mouse.released.clear();
	//
	ww->mouse_next.down.clear();
	ww->mouse_next.pressed.clear();
	ww->mouse_next.released.clear();
}