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
dllg const char* winwin_keyboard_string_get(ww_ptr ww) {
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

dllg bool winwin_mouse_wheel_up(ww_ptr ww) {
	return ww->mouse.wheel < 0;
}
dllg bool winwin_mouse_wheel_down(ww_ptr ww) {
	return ww->mouse.wheel > 0;
}
dllg bool winwin_mouse_wheel_get_delta_x(ww_ptr ww) {
	return ww->mouse.hwheel;
}
dllg bool winwin_mouse_wheel_get_delta_y(ww_ptr ww) {
	return ww->mouse.wheel;
}