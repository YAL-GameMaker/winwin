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
    static thread = false;
    static vsync = 0;
    static close_button = 1;
}

