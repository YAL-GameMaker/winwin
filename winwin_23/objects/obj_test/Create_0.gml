//show_message("!");
winwin_init();
var _width = room_width;
var _height = room_height;
window_set_size(_width * 2, _height * 2);
//surface_resize(application_surface, _width * 2, _height * 2);
extra = winwin_create(400, 400, _width, _height * 3, "Window 1", 2);
eyes = [];
alarm[0] = 10;