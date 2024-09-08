maze = -1;
var config = new winwin_config();
config.caption = "Debug";
window = winwin_create(
	window_get_x() + window_get_width(), window_get_y(),
	320, 320, config,
);
alarm[0] = 2;