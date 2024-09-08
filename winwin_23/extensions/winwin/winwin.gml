#define winwin_preinit
//#init winwin_preinit
//#global winwin_main
var _inf = os_get_info();
var _ok = winwin_init_raw(
	window_handle(),
	_inf[?"video_d3d11_device"],
	_inf[?"video_d3d11_context"],
	_inf[?"video_d3d11_swapchain"],
);
ds_map_destroy(_inf);
global.__winwin_map = ds_map_create();
winwin_main = winwin_init_2();
return _ok;

#define winwin_prepare_buffer
/// (size:int)->buffer~
var _size = argument0;
gml_pragma("global", "global.__winwin_buffer = undefined");
var _buf = global.__winwin_buffer;
if (_buf == undefined) {
    _buf = buffer_create(_size, buffer_grow, 1);
    global.__winwin_buffer = _buf;
} else if (buffer_get_size(_buf) < _size) {
    buffer_resize(_buf, _size);
}
buffer_seek(_buf, buffer_seek_start, 0);
return _buf;

#define winwin_exists
/// (ww)->
var _ww = argument0;
if (_ww == undefined) return false;
if (instanceof(_ww) != "winwin") return false;
var _ptr = _ww.__ptr__;
if (_ptr == pointer_null) return false;
return winwin_exists_raw(_ptr);

#define winwin_from_handle
/// (handle)->
var _ptr = argument0;
if (!is_ptr(_ptr)) _ptr = ptr(_ptr);
return global.__winwin_map[?_ptr];

#define winwin_draw_start
/// (window)->
var _win = argument0;
draw_flush();
if (!winwin_draw_start_raw(_win)) return false;
global.__winwin_last_view = matrix_get(matrix_view);
global.__winwin_last_proj = matrix_get(matrix_projection);
var _dw = winwin_get_draw_width();
var _dh = winwin_get_draw_height();
//trace(_dw, _dh);
matrix_set(matrix_view, matrix_build_lookat(
    _dw / 2, _dh / 2, -16000,
    _dw / 2, _dh / 2, 0,
    0, -1, 0
));
matrix_set(matrix_projection, matrix_build_projection_ortho(
   -_dw, _dh, 1, 32000 
));
return true;

#define winwin_draw_end
/// ()->
draw_flush();
if (!winwin_draw_end_raw()) return false;
matrix_set(matrix_view, global.__winwin_last_view);
matrix_set(matrix_projection, global.__winwin_last_proj);
return true;

#define winwin_draw_clear
/// (color, alpha = 1)
var _color = argument[0];
var _alpha = argument_count > 1 ? argument[1] : 1;
var _width = winwin_get_draw_width();
var _height = winwin_get_draw_height();
var _prev_color = draw_get_color();
var _prev_alpha = draw_get_alpha();
var _prev_blend = gpu_get_blendmode();
var _pad = 2;
if (_alpha < 1) {
	gpu_set_blendmode(bm_subtract);
	draw_set_color(c_white);
	draw_set_alpha(1);
	draw_rectangle(-_pad, -_pad, _width + _pad, _height + _pad, false);
}
//
if (_alpha > 0) {
	if (_alpha < 1) {
		gpu_set_blendmode(bm_add);
	} else gpu_set_blendmode(bm_normal);
	draw_set_color(_color);
	draw_set_alpha(_alpha);
	draw_rectangle(-_pad, -_pad, _width + _pad, _height + _pad, false);
}
//
gpu_set_blendmode(_prev_blend);
draw_set_color(_prev_color);
draw_set_alpha(_prev_alpha);
//

#define winwin_keyboard_set_string
/// (ww, str)->
if (argument0 == winwin_main) {
	keyboard_string = argument1;
	return true;
}
var _buf = winwin_prepare_buffer(4);
buffer_seek(_buf, buffer_seek_start, 0);
string_foreach(argument1, function(_char, _pos) /*=>*/ {
	buffer_write(_buf, buffer_u32, ord(_char));
});
return winwin_keyboard_set_string_raw(argument0, _buf);