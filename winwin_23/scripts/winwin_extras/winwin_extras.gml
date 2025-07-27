function winwin(_ptr) constructor {
    __ptr__ = _ptr;
}
function winwin_config() constructor {
    static caption = "Window";
    static kind = winwin_kind_normal;
    static resize = false;
    static show = true;
    static topmost = false;
    static taskbar_button = true; // can only disable for borderless!
    static clickthrough = false;
    static noactivate = false;
    static per_pixel_alpha = false;
    static thread = false;
    static vsync = 0;
    static close_button = 1;
    static owner = undefined;
}
// https://github.com/YoYoGames/GameMaker-Bugs/issues/10141
function winwin_buffer_write_string_u32(_buf, _string) {
    with ({ _buf: _buf }) string_foreach(_string, function(_char, _pos) /*=>*/ {
        buffer_write(_buf, buffer_u32, ord(_char));
    });
}