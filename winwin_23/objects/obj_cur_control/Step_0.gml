var wx = window_get_x();
var wy = window_get_y();
var ww = window_get_width();
var wh = window_get_height();
var show = !winwin_is_minimized(winwin_main);

if (cursor != undefined) {
	var cx = display_mouse_get_x();
	var cy = display_mouse_get_y();
	winwin_set_visible(cursor, show && cx >= wx && cy >= wy && cx < wx + ww && cy < wy + wh);
	winwin_follow_mouse_start_snapped(cursor, wx, wy, scale, scale, 0, 0);
	//winwin_follow_mouse_start(cursor, 0, 0);
}