#include "stdafx.h"
#include "winwin.h"

dllg bool winwin_has_focus(ww_ptr ww) {
	return GetFocus() == ww->hwnd;
}
dllg ww_ptr_find winwin_get_focus() {
	return ww_find(GetFocus());
}
dllg bool winwin_set_focus(ww_ptr ww) {
	return SetFocus(ww->hwnd) != NULL;
}
