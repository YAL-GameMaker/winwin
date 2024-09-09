maze = -1;
var config = new winwin_config();
config.caption = "Debug";
config.kind = winwin_kind_tool;
window = winwin_create(
	window_get_x() + window_get_width(), window_get_y(),
	320, 320, config,
);
winwin_set_owner(window, winwin_main);
//winwin_set_topmost(winwin_main, true);
alarm[0] = 2;