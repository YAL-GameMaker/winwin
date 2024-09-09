if (!winwin_exists(window)) exit;

var pad = 16;
var w_width = winwin_get_width(window);
var w_height = winwin_get_height(window);
if (point_in_rectangle(x, y, -pad, -pad, w_width + pad, w_height + pad)) exit;

// try to wrap on both 
var w_left = winwin_get_x(window);
var w_top = winwin_get_y(window);
if (try_wrap(w_left, w_top, x, y, pad)) exit;
if (try_wrap(w_left, w_top, xprevious, y, pad)) exit;
if (try_wrap(w_left, w_top, x, yprevious, pad)) exit;

x = clamp(x, -pad, pad + w_width);
y = clamp(y, -pad, pad + w_height);