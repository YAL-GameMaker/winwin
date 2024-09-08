winwin_update();
if (winwin_exists(extra) && !winwin_get_visible(extra)) {
	if (extra.show_in == 0) {
		extra.show_in = 30;
	} else if (--extra.show_in <= 0) {
		winwin_set_visible(extra, true);
		winwin_set_caption(extra, "I'm back :)");
	}
}
if keyboard_check_pressed(vk_escape) winwin_destroy(extra)

#region eyes
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
	winwin_draw_clear(c_black, 0);
	
	draw_sprite(spr_wwd_basics_eyes, 0, 64, 64);
	var wx = winwin_get_x(ww);
	var wy = winwin_get_y(ww);
	for (var k = -1; k <= 1; k += 2) {
		var ex = wx + 64 + 32 * k;
		var ey = wy + 64;
		var dx = display_mouse_get_x() - ex;
		var dy = display_mouse_get_y() - ey;
		var dd = point_distance(0, 0, dx, dy);
		if (dd > 15) {
			dd = (200 / dd) / 15;
			dx *= dd;
			dy *= dd;
		}
		draw_sprite_ext(spr_wwd_basics_eyes, 1, 64 + max(0, k) * 64 + dx, 64 + dy, 1, 1, 0, c_white, 1);
	}
	
	winwin_draw_end();
}
#endregion
