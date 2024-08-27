var wwc := new winwin_config();
wwc.caption = "👀";
wwc.show = false;
wwc.kind = winwin_kind_borderless;
wwc.topmost = true;
wwc.taskbar_button = false;
for (var i = 0; i < 0; i++) {
	var ww = winwin_create(window_get_x() + i * 150, window_get_y(), 128, 128, wwc);
	ww.dragging = false;
	ww.drag_x = 0;
	ww.drag_y = 0;
	winwin_enable_per_pixel_alpha(ww);
	winwin_set_visible(ww, true);
	array_push(eyes, ww);
}