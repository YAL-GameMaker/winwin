image_xscale = 4;
image_yscale = 4;
friction = 1.8;
accel = 6;
maxspeed = 8;
check = function(_key) {
	// take input from any window, really
	var n = ds_list_size(winwin_list);
	for (var i = 0; i < n; i++) {
		if (winwin_keyboard_check(winwin_list[|i], _key)) return true;
	}
	return false;
}
