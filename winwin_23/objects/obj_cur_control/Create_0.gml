// you could make a fake cursor out of a window and a pixel grid out of another window
// https://astropulse.itch.io/familiar-pixel-cursors
var w = room_width;
var h = room_height;
scale = 6;
surface_resize(application_surface, w, h);
window_set_size(w * scale, h * scale);
display_set_gui_size(w * scale, h * scale);
alarm[0] = 10;

cursor = undefined;
cursor_spr = spr_cur_cursor;
grid_sprite = -1 /*#as sprite*/;

window_set_cursor(cr_none);