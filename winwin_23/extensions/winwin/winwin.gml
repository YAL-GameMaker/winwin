#define winwin_init
/// ()
var _inf = os_get_info();
var _result = winwin_init_raw(
	window_handle(),
	_inf[?"video_d3d11_device"],
	_inf[?"video_d3d11_context"],
	_inf[?"video_d3d11_swapchain"],
);
ds_map_destroy(_inf);
return _result;

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

#define winwin_draw_start
/// (window)->
var _win = argument0;
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