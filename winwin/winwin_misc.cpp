#include "stdafx.h"
#include "winwin.h"

void ProcessMessages() {
    tagMSG m;
    while (true) {
        if (PeekMessageW(&m, NULL, 0, 0, PM_REMOVE)) {
            if (m.message != WM_QUIT) {
                TranslateMessage(&m);
                DispatchMessageW(&m);
            } else break;
        } else break;
    }
}
dllg void winwin_sleep(int ms, bool process_messages = true) {
    if (!process_messages) {
        Sleep(ms);
        return;
    }
    constexpr int pm_step = 100;
    while (ms > 0) {
        int step;
        if (ms > pm_step) {
            step = pm_step;
            ms -= pm_step;
        } else {
            step = ms;
            ms = 0;
        }
        Sleep(step);
        ProcessMessages();
    }
}

dllg void winwin_game_end(int exit_code = 0) {
    exit(exit_code);
}