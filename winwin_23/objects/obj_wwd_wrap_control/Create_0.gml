windows = [winwin_main];

var config = new winwin_config();
config.resize = true;
config.thread = true;
config.show = false;
var wx = window_get_x() + window_get_width();
var wy = window_get_y();
var wh = window_get_height();
for (var i = 0; i < 3; i++) {
	var ww = 200 + i * 50;
	config.caption = "Window " + string(i);
	var window = winwin_create(wx, wy, ww, wh, config);
	wx += ww;
	array_push(windows, window);
	with (window) call_later(2, time_source_units_frames, function() /*=>*/ {
		winwin_set_visible(self, true);
	})
}