#include "stdafx.h"
#include "winwin.h"

/*
Consideration: 
struct {
    winwin* target = nullptr;
    double target_halign = 0, target_valign = 0;
    double self_halign = 0, self_valign = 0;
    std::vector<winwin*> attachments{};
} attach;
But would it be better if window automatically remembered its offset relative to parent?
Kind of complicated
*/