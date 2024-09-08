window = winwin_main;
spr_idle = spr_wwd_wrap_idle;
spr_walk = spr_wwd_wrap_walk;
image_xscale = 4;
image_yscale = 4;
friction = 1.8;
accel = 6;
maxspeed = 8;
check = function(_key) {
	// take input from any window, really
	var _windows = obj_wwd_wrap_control.windows;
	var n = array_length(_windows);
	for (var i = 0; i < n; i++) {
		var _window = _windows[i];
		if (winwin_keyboard_check(_windows[i], _key)) return true;
	}
	return false;
}
try_wrap = function(wx, wy, nx, ny, pad) {
	// figure out which window can fit the character at nx,ny
	var gx = wx + nx;
	var gy = wy + ny;
	var _windows = obj_wwd_wrap_control.windows;
	var n = array_length(_windows);
	for (var i = 0; i < n; i++) {
		var _window = _windows[i];
		if (_window == window) continue; // (same window!)
		if (!winwin_exists(_window)) continue;
		
		var iw_left = winwin_get_x(_window);
		var iw_top = winwin_get_y(_window);
		var iw_right = iw_left + winwin_get_width(_window);
		var iw_bottom = iw_top + winwin_get_height(_window);
		if (point_in_rectangle(gx, gy, iw_left - pad, iw_top - pad, iw_right + pad, iw_bottom + pad)) {
			window = _window;
			x = gx - iw_left;
			y = gy - iw_top;
			return true;
		}
	}
}