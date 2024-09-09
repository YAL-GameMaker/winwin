// grid underlay
var grid_config = new winwin_config();
grid_config.per_pixel_alpha = true;
grid_config.kind = winwin_kind_borderless;
grid_config.clickthrough = true;
grid_config.noactivate = true;
grid = winwin_create(window_get_x() + 100, window_get_y(), 720, 720, grid_config);
grid_start = 0;
grid_surf = -1;

// "game window"
var game_config = new winwin_config();
game_config.caption = window_get_caption();
//game_config.close_button = 2;
game_config.thread = true;
game = winwin_create(
	window_get_x(),
	window_get_y(),
	window_get_width(),
	window_get_height(),
	game_config,
);

// the dinosaur
var dino_inst = instance_create_layer(room_width / 2, room_height / 2, layer, obj_oob_player);
var dino_sprite = dino_inst.sprite_index;
var dino_scale = dino_inst.image_xscale;
var dino_config = new winwin_config();
dino_config.caption = "Dino";
dino_config.kind = winwin_kind_borderless;
dino_config.noactivate = true;
dino_config.clickthrough = true;
dino_config.owner = game;
dino_config.per_pixel_alpha = true;
dino = winwin_create(
	window_get_x() + dino_inst.x - sprite_get_xoffset(dino_sprite) * dino_scale,
	window_get_y() + dino_inst.y - sprite_get_yoffset(dino_sprite) * dino_scale,
	sprite_get_width(dino_sprite) * dino_scale,
	sprite_get_height(dino_sprite) * dino_scale,
	dino_config,
);
call_later(2, time_source_units_frames, function() /*=>*/ {
	// if I just disable the draw event, the game sometimes hangs on exit (??)
	// NB! If you hide the active window, the app minimizes
	winwin_set_focus(game);
	winwin_set_visible(winwin_main, false);
});