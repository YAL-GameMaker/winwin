var peeker_count = 1;
var config = new winwin_config();
config.caption = "👀";
config.show = false;
config.kind = winwin_kind_borderless;
config.topmost = true;
config.taskbar_button = false;
for (var i = 0; i < peeker_count; i++) {
	var ww = winwin_create(window_get_x() + i * 150, window_get_y() - 150, 128, 128, config);
	ww.dragging = false;
	ww.drag_x = 0;
	ww.drag_y = 0;
	winwin_enable_per_pixel_alpha(ww);
	winwin_set_visible(ww, true);
	array_push(eyes, ww);
}