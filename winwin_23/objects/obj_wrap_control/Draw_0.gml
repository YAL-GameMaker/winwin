draw_set_font(fnt_test);
draw_text(7, 7, "Dino sprite from:"
	+ "\n" + "https://arks.itch.io/dino-characters"
);
var n = array_length(windows);
for (var i = 0; i < n; i++) {
	var _window = windows[i];
	if (!winwin_exists(_window)) continue;
	var _main = _window == winwin_main;
	if (!_main) {
		// (if it's the game window, we're already drawing there)
		winwin_draw_start(_window);
		winwin_draw_clear(#405070);
	}
	//
	var wx = winwin_get_x(_window);
	var wy = winwin_get_y(_window);
	var ww = winwin_get_width(_window);
	var wh = winwin_get_height(_window);
	// draw the player in the active window and ones that are overlapping:
	with (obj_wrap_player) {
		if (!winwin_exists(window)) continue;
		var pwx = winwin_get_x(window);
		var pwy = winwin_get_y(window);
		if (window == _window) {
			var pad = 5;
			draw_rectangle(pad, pad, ww - pad, wh - pad, true);
		}
		if (window == _window || rectangle_in_rectangle(
			pwx + bbox_left,
			pwy + bbox_top,
			pwx + bbox_right,
			pwy + bbox_bottom,
			wx, wy, wx + ww, wy + wh,
		)) {
			var px = x - (wx - pwx);
			var py = y - (wy - pwy);
			draw_sprite_ext(sprite_index, image_index, px, py, image_xscale, image_yscale, image_angle, image_blend, image_alpha);
		}
	}
	if (!_main) winwin_draw_end();
}
