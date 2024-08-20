for (var i = 0; i < 5; i++) {
	var ww = winwin_create(window_get_x() + i * 150, window_get_y(), 128, 128, "👀", 0);
	winwin_enable_per_pixel_alpha(ww);
	winwin_set_topmost(ww, true);
	winwin_set_taskbar_button_visible(ww, false);
	array_push(eyes, ww);
}