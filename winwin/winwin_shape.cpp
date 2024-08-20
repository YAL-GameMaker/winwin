#include "stdafx.h"
#include "winwin.h"
#include <dwmapi.h>

// this largely mirrors my own window_shape

static LONG GetWindowExStyle(HWND hwnd) {
	return GetWindowLong(hwnd, GWL_EXSTYLE);
}
static void SetWindowExStyle(HWND hwnd, LONG flags) {
	SetWindowLong(hwnd, GWL_EXSTYLE, (flags));
}
static bool GetWindowLayered(HWND hwnd) {
	return GetWindowExStyle(hwnd) & WS_EX_LAYERED;
}
static void SetWindowLayered(HWND hwnd, bool layered) {
	auto flags = GetWindowExStyle(hwnd);
	if (layered) {
		if ((flags & WS_EX_LAYERED) == 0) {
			SetWindowExStyle(hwnd, flags | WS_EX_LAYERED);
		}
	} else {
		if ((flags & WS_EX_LAYERED) != 0) {
			SetWindowExStyle(hwnd, flags & ~WS_EX_LAYERED);
		}
	}
}

dllg double winwin_get_alpha(ww_ptr ww) {
	auto hwnd = ww->hwnd;
	if (!GetWindowLayered(hwnd)) return 1;
	BYTE alpha = 0;
	DWORD flags = 0;
	GetLayeredWindowAttributes(hwnd, NULL, &alpha, &flags);
	if ((flags & LWA_ALPHA) == 0) return 1;
	return (double)alpha / 255;
}
dllg bool winwin_set_alpha(ww_ptr ww, double alpha) {
	auto hwnd = ww->hwnd;
	bool set = alpha < 1;
	if (set) {
		SetWindowLayered(hwnd, true);
	} else {
		if (!GetWindowLayered(hwnd)) return true;
	}
	//
	BYTE bAlpha = 0;
	COLORREF crKey = {};
	DWORD flags = 0;
	GetLayeredWindowAttributes(hwnd, &crKey, &bAlpha, &flags);
	//
	if (set) {
		flags |= LWA_ALPHA;
		if (alpha < 0) alpha = 0;
		bAlpha = (BYTE)(alpha * 255);
		SetLayeredWindowAttributes(hwnd, crKey, bAlpha, flags);
	} else {
		flags &= ~LWA_ALPHA;
		SetLayeredWindowAttributes(hwnd, crKey, 255, flags);
		if (flags == 0) SetWindowLayered(hwnd, false);
	}
	return true;
}

dllg int winwin_get_chromakey(ww_ptr ww) {
	auto hwnd = ww->hwnd;
	if (!GetWindowLayered(hwnd)) return -1;
	COLORREF crKey;
	DWORD flags = 0;
	GetLayeredWindowAttributes(hwnd, &crKey, NULL, &flags);
	if ((flags & LWA_COLORKEY) == 0) return -1;
	return crKey;
}
dllg bool winwinset_chromakey(ww_ptr ww, double color) {
	auto hwnd = ww->hwnd;
	bool set = color >= 0;
	if (set) {
		SetWindowLayered(hwnd, true);
	} else {
		if (!GetWindowLayered(hwnd)) return true;
	}
	//
	BYTE bAlpha = 0;
	COLORREF crKey = {};
	DWORD flags = 0;
	GetLayeredWindowAttributes(hwnd, &crKey, &bAlpha, &flags);
	//
	if (set) {
		flags |= LWA_COLORKEY;
		crKey = (DWORD)color;
		SetLayeredWindowAttributes(hwnd, crKey, bAlpha, flags);
	} else {
		flags &= ~LWA_COLORKEY;
		crKey = 0xFF00FF;
		SetLayeredWindowAttributes(hwnd, crKey, bAlpha, flags);
		if (flags == 0) SetWindowLayered(hwnd, false);
	}
	return true;
}

///
dllg bool winwin_enable_per_pixel_alpha(ww_ptr ww) {
	DWM_BLURBEHIND bb = { 0 };
	bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	bb.hRgnBlur = CreateRectRgn(0, 0, -1, -1);
	bb.fEnable = TRUE;
	DwmEnableBlurBehindWindow(ww->hwnd, &bb);
	return 1;
}