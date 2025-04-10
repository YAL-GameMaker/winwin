var wx = window_get_x();
var wy = window_get_y();
var ww = window_get_width();
var wh = window_get_height();

var pixel = surface_create(scale, scale);
surface_set_target(pixel);
draw_clear(c_white);
surface_reset_target();

var draw_grid = function(pixel, w, h) /*=>*/ {
	var grid_alpha = 0.3;
	for (var i = 0; i < w; i += scale) {
		draw_surface_stretched_ext(pixel, i, 0, 1, h, c_black, grid_alpha);
	}
	for (var i = 0; i < h; i += scale) {
		draw_surface_stretched_ext(pixel, 0, i, w, 1, c_black, grid_alpha);
	}
}

var cursor_cfg = new winwin_config();
cursor_cfg.clickthrough = true;
cursor_cfg.kind = winwin_kind_borderless;
cursor_cfg.per_pixel_alpha = true;
cursor_cfg.taskbar_button = false;
cursor_cfg.topmost = true;
var cursor_width = scale * sprite_get_width(cursor_spr);
var cursor_height = scale * sprite_get_height(cursor_spr);
cursor = winwin_create(wx, wy, cursor_width, cursor_height, cursor_cfg);
winwin_draw_begin(cursor);
winwin_draw_clear(c_black, 0);
draw_sprite_ext(cursor_spr, 0, 0, 0, scale, scale, 0, c_white, 1);
gpu_set_colorwriteenable(1, 1, 1, 0);
draw_grid(pixel, cursor_width, cursor_height);
gpu_set_colorwriteenable(1, 1, 1, 1);
winwin_draw_end();

winwin_follow_mouse_start_snapped(cursor, wx, wy, scale, scale, 0, 0);

var grid_surf = surface_create(ww, wh);
surface_set_target(grid_surf);
draw_grid(pixel, ww, wh);
surface_reset_target();
grid_sprite = sprite_create_from_surface(grid_surf, 0, 0, ww, wh, 0, 0, 0, 0);

surface_free(pixel);
surface_free(grid_surf);