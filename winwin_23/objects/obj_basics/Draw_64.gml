draw_set_font(fnt_test);
draw_set_color(c_black);
draw_set_alpha(0.25);
draw_rectangle(-3, -3, room_width + 3, room_height + 3, false);
draw_set_alpha(1);
draw_set_color(c_white);

if (winwin_exists(extra)) {
	var ks = winwin_keyboard_string_get(extra);
	if (ks != extra.last_caption) {
		winwin_set_caption(extra, ks);
		extra.last_caption = ks;
	}
	
	
	winwin_draw_begin(extra);
	
	draw_set_color(#405070);
	var _width = winwin_get_draw_width();
	var _height = winwin_get_draw_height();
	winwin_draw_clear(#405070);
	
	draw_set_color(c_white);
	draw_rectangle(2, 2, _width - 3, _height - 3, 1);
	
	// info:
	var s = sfmt("Size: %x%", _width, _height);
	s += "\nTry keyboard, mouse here";
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
	
	//
	var wheel_x = winwin_mouse_wheel_get_delta_x(extra) div 10;
	var wheel_y = winwin_mouse_wheel_get_delta_y(extra) div 10;
	if (wheel_x != 0 || wheel_y != 0) {
		winwin_set_position(extra, winwin_get_x(extra) + wheel_x, winwin_get_y(extra) + wheel_y);
	}
	
	// close button state:
	for (var k = 0; k < 3; k++) {
		if (winwin_keyboard_check_pressed(extra, vk_f1 + k)) {
			winwin_set_close_button(extra, k);
		}
	}
	
	// key history:
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
	
	draw_text(5, 5, "Hello! " + string(current_time div 1000)
		+ "\nFPS: " + string(fps) + "/" + string(room_speed));
} else {
	draw_text(5, 5, "No window!");
}
