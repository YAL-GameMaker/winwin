var _width = room_width;
var _height = room_height;
//window_set_size(_width * 2, _height * 2);
//surface_resize(application_surface, _width * 2, _height * 2);

var config = new winwin_config();
config.caption = "Extra!";
config.resize = true;
config.close_button = 2;
config.thread = true;
//config.kind = winwin_kind_tool;
extra = winwin_create(window_get_x() - 350, window_get_y(), 350, 600, config);
extra.key_list = ds_list_create();
extra.show_in = 0;
extra_caption = "";

eyes = [];
alarm[0] = 10;