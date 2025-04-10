#pragma once
#include "stdafx.h"
#include "winwin.h"
#include <atomic>

struct winwin_follow_mouse_params {
    ww_ptr ww;
    std::atomic<int> xoffset, yoffset, sleep_time;
};
struct winwin_follow_mouse_snapped_params {
    ww_ptr ww;
    std::atomic<int> xoffset, yoffset, sleep_time, xstart, ystart;
    std::atomic<double> xsnap, ysnap;
};