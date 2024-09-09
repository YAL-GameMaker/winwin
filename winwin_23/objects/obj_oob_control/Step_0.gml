winwin_update();
if (!winwin_exists(game)) {
	// Quit if the "game window" has closed.
	game_end();
	exit;
}
// player:
with (obj_oob_player) {
	var scale = abs(image_xscale);
	var _xo = sprite_get_xoffset(sprite_index) * scale;
	var _yo = sprite_get_yoffset(sprite_index) * scale;
	winwin_set_position(other.dino,
		winwin_get_x(other.game) + x - _xo,
		winwin_get_y(other.game) + y - _yo,
	);
	winwin_draw_begin(other.dino);
	winwin_draw_clear(c_black, 0);
	draw_sprite_ext(sprite_index, image_index, _xo, _yo, image_xscale, image_yscale, 0, image_blend, image_alpha);
	winwin_draw_end();
}
// game:
winwin_draw_begin(game);
winwin_draw_clear(c_yal_blue);
winwin_draw_end();
// grid:
if (grid_start > 0) {
	if (winwin_has_focus(game) || window_has_focus()) {
		if (!winwin_get_visible(grid)) {
			winwin_set_visible(grid, true);
			winwin_set_focus(grid);
			winwin_set_focus(game);
		}
		//
		var r = winwin_get_width(grid) div 2;
		if (!surface_exists(grid_surf)) {
			grid_surf = surface_create(r * 2, r * 2);
		}
		// honestly, this is WAY too much effort for a cool effect
		surface_set_target(grid_surf);
		draw_clear_alpha(c_black, 0);
		gpu_set_blendmode(bm_add);
		var u = shader_get_uniform(sh_oob_clip, "u_circle");
		var t = (current_time - grid_start) / 1500;
		shader_set(sh_oob_clip);
		shader_set_uniform_f(u, r, r, min(t, 1) * r);
		
		draw_sprite_tiled(spr_oob_grid, 0, t * 16 % 16, -t/2 * 16 % 16);
		
		shader_reset();
		gpu_set_blendmode(bm_normal);
		surface_reset_target();
		if (keyboard_check_pressed(vk_space)) surface_save(grid_surf, "temp.png");
		//
		winwin_draw_begin(grid);
		winwin_draw_clear(c_black, 0);
		draw_surface(grid_surf, 0, 0);
		winwin_draw_end();
	} else {
		if (winwin_get_visible(grid)) winwin_set_visible(grid, false);
	}
} else {
	winwin_draw_begin(grid);
	winwin_draw_clear(c_black, 0.0);
	winwin_draw_end();
	if (!point_in_rectangle(obj_oob_player.x, obj_oob_player.y, 0, 0, room_width, room_height)) {
		grid_start = current_time;
		winwin_set_position(grid,
			winwin_get_x(dino) - (winwin_get_width(grid) - winwin_get_width(dino)) div 2,
			winwin_get_y(dino) - (winwin_get_height(grid) - winwin_get_height(dino)) div 2,
		);
	}
}
