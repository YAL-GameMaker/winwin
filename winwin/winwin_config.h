#pragma once
#include "winwin.h"

///
enum class winwin_kind {
    normal,
    borderless,
    tool,
};
struct winwin_config {
    const char* caption;
    winwin_kind kind;
    bool resize;
    bool show;
    bool topmost;
    bool taskbar_button;
    bool clickthrough;
    bool noactivate;
    bool per_pixel_alpha;
    bool thread;
    int8_t vsync;
    int8_t close_button;
    std::optional<ww_ptr> owner;
};