#include "stdafx.h"
#include "winwin.h"

dllg bool winwin_has_focus(ww_ptr ww) {
	return GetFocus() == ww->hwnd;
}

dllg ww_ptr_find winwin_get_active() {
	auto hwnd = GetActiveWindow();
	auto pair = ww_map.find(hwnd);
	if (pair != ww_map.end()) {
		return pair->second;
	} else return nullptr;
}
dllg bool winwin_set_active(ww_ptr ww) {
	return SetActiveWindow(ww->hwnd);
}

dllg ww_ptr_find winwin_get_foreground() {
	auto hwnd = GetForegroundWindow();
	auto pair = ww_map.find(hwnd);
	if (pair != ww_map.end()) {
		return pair->second;
	} else return nullptr;
}
dllg bool winwin_set_foreground(ww_ptr ww) {
	return SetForegroundWindow(ww->hwnd);
}