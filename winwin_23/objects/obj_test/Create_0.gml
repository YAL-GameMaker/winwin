//show_message("!");
winwin_init();
var _width = room_width;
var _height = room_height;
//window_set_size(_width * 2, _height * 2);
//surface_resize(application_surface, _width * 2, _height * 2);

var wwc := new winwin_config();
wwc.caption = "Extra!";
wwc.resize = true;
//wwc.kind = winwin_kind_tool;
extra = winwin_create(window_get_x() - 400, window_get_y(), 350, 600, wwc);
extra.key_list = ds_list_create();
extra_caption = "";

eyes = [];
alarm[0] = 10;