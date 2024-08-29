
var n = array_length(eyes);
for (var i = 0; i < n; i++) {
	var ww = eyes[i];
	if (!winwin_exists(ww)) continue;
	
	if (winwin_mouse_check_button_pressed(ww, mb_left)) {
		ww.dragging = true;
		ww.drag_x = display_mouse_get_x() - winwin_get_x(ww);
		ww.drag_y = display_mouse_get_y() - winwin_get_y(ww);
	}
	if (ww.dragging) {
		if (winwin_mouse_check_button(ww, mb_left)) {
			winwin_set_position(ww,
				display_mouse_get_x() - ww.drag_x,
				display_mouse_get_y() - ww.drag_y,
			)
		} else {
			ww.dragging = false;
		}
	}
	
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
		dd = (200 / dd) / 15;
		dx *= dd;
		dy *= dd;
		draw_sprite_ext(spr_eyes, 1, 64 + max(0, k) * 64 + dx, 64 + dy, 1, 1, 0, c_white, 1);
	}
	
	winwin_draw_end();
}

if (winwin_exists(extra)) {
	var ks = winwin_keyboard_string_get(extra);
	if (ks != extra_caption) {
		winwin_set_caption(extra, ks);
		extra_caption = ks;
	}
	
	
	winwin_draw_start(extra);
	
	draw_set_color(#405070);
	var _width = winwin_get_draw_width();
	var _height = winwin_get_draw_height();
	draw_rectangle(0, 0, _width, _height, false);
	
	draw_set_color(c_white);
	draw_rectangle(2, 2, _width - 3, _height - 3, 1);
	
	var s = sfmt("Size: %x%", _width, _height);
	
	//s += "\nkeyboard_string: " + winwin_keyboard_string_get(extra);
	for (var k = 0; k < 256; k++) {
		if (winwin_keyboard_check(extra, k)) s += "\nHolding key " + string(k);
		if (winwin_keyboard_check_pressed(extra, k)) {
			ds_list_insert(extra.key_list, 0, k);
			if (ds_list_size(extra.key_list) >= 8) {
				ds_list_delete(extra.key_list, 7);
			}
		}
	}
	draw_text(7, 7, s);
	
	draw_set_halign(fa_right);
	s = "Keys:"
	for (var i = 0; i < ds_list_size(extra.key_list); i++) {
		s += "\n" + string(extra.key_list[|i]);
	}
	draw_text(_width - 7, 7, s);
	draw_set_halign(fa_left);
	
	draw_circle(_width / 2, _height / 2, 20, false);
	
	if (winwin_mouse_check_button(extra, mb_left)) {
		draw_circle(winwin_mouse_get_x(extra), winwin_mouse_get_y(extra), 10, true);
	}
	
	winwin_draw_end();
	
	if (keyboard_check_pressed(vk_space)) {
		for (var i = 0; i < 10; i++) {
			winwin_draw_start(winwin_main);
			draw_text(57, 57 + 20 * i, string(i));
			winwin_draw_end();
			var _till = current_time + 500;
			while (current_time < _till) {}
		}
	}
	
	draw_text(5, 5, "Hello! " + string(current_time div 1000));
} else {
	draw_text(5, 5, "No window!");
}