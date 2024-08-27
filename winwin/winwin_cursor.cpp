#include "stdafx.h"
#include "winwin.h"

static HCURSOR winwin_cursors[23]{};

HCURSOR winwin_cursor_init() {
	#define X(c) LoadCursor(NULL, c)
	auto& cr = winwin_cursors;
	auto arrow = X(IDC_ARROW);
	auto dont = X(IDC_NO);
	cr[0] = arrow; // cr_default 0
	cr[1] = NULL; // cr_none -1
	cr[2] = arrow; // cr_arrow -2
	cr[3] = X(IDC_CROSS); // cr_cross -3
	cr[4] = X(IDC_IBEAM); // cr_beam -4
	cr[6] = X(IDC_SIZENESW); // cr_size_nesw -6
	cr[7] = X(IDC_SIZENS); // cr_size_ns -7
	cr[8] = X(IDC_SIZENWSE); // cr_size_nwse -8
	cr[9] = X(IDC_SIZEWE); // cr_size_we -9
	cr[10] = X(IDC_UPARROW); // cr_uparrow -10
	cr[11] = X(IDC_WAIT); // cr_hourglass -11
	cr[12] = X(IDC_HAND); // cr_drag -12
	for (int i = 13; i <= 18; i++) cr[i] = dont;
	cr[19] = X(IDC_APPSTARTING); // cr_appstart -19
	cr[21] = X(IDC_HELP); // cr_handpoint -21
	cr[22] = X(IDC_SIZEALL); // cr_size_all -22
	#undef X
	return arrow;
}

dllg int winwin_get_cursor(ww_ptr ww) {
	auto cr = ww->cursor;
	for (int i = 0; i < std::size(winwin_cursors); i++) {
		if (winwin_cursors[i] == cr) return -i;
	}
	return -1;
}
dllg bool winwin_set_cursor(ww_ptr ww, int cursor) {
	cursor = -cursor;
	if (cursor < 0 || cursor >= std::size(winwin_cursors)) return false;
	ww->cursor = winwin_cursors[cursor];
	if (ww->mouse_over) SetCursor(ww->cursor);
	return true;
}

dllg uintptr_t winwin_get_cursor_handle(ww_ptr ww) {
	return (uintptr_t)ww->cursor;
}
dllg bool winwin_set_cursor_handle(ww_ptr ww, uintptr_t hcursor) {
	ww->cursor = (HCURSOR)hcursor;
	return true;
}
