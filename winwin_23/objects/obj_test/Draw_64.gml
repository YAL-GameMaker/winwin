
var n = array_length(eyes);
for (var i = 0; i < n; i++) {
	var ww = eyes[i];
	if (!winwin_exists(ww)) continue;
	
	winwin_draw_start(ww);
	draw_set_color(c_white);
	gpu_set_blendmode(bm_subtract);
	draw_rectangle(-2, -2, 130, 130, false);
	gpu_set_blendmode(bm_normal);
	
	draw_sprite(spr_eyes, 0, 64, 64);
	var wx = winwin_get_x(ww);
	var wy = winwin_get_y(ww);
	for (var k = -1; k <= 1; k += 2) {
		var ex = wx + 64 + 32 * k;
		var ey = wy + 64;
		var dx = display_mouse_get_x() - ex;
		var dy = display_mouse_get_y() - ey;
		var dd = point_distance(0, 0, dx, dy);
		//dd = (dd > 200 ? 200 / dd : 1) / 15;
		dd = (200 / dd) / 15;
		dx *= dd;
		dy *= dd;
		//draw_text(64, 64, string(dx) + "\n" + string(dy));
		draw_sprite_ext(spr_eyes, 1, 64 + max(0, k) * 64 + dx, 64 + dy, 1, 1, 0, c_white, 1);
	}
	
	winwin_draw_end();
}

if (winwin_exists(extra)) {
	winwin_draw_start(extra);
	
	draw_set_color(/*#*/0x705040);
	var _width = winwin_get_draw_width();
	var _height = winwin_get_draw_height();
	draw_rectangle(0, 0, _width, _height, false);
	
	draw_set_color(c_white);
	draw_rectangle(2, 2, _width - 3, _height - 3, 1);
	draw_text(7, 7, sfmt("Hi! Size: %x%", _width, _height));
	draw_circle(_width / 2, _height / 2, 20, false);
	
	winwin_draw_end();
	
	draw_text(5, 5, "Hello! " + string(current_time div 1000));
} else {
	draw_text(5, 5, "No window!");
}