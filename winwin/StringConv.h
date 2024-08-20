#pragma once
#include "stdafx.h"

class StringConv {
public:
    char* cbuf = NULL;
    size_t cbuf_size = 0;
    WCHAR* wbuf = NULL;
    size_t wbuf_size = 0;
    StringConv() {

    }
    LPWSTR wget(size_t size) {
        if (wbuf_size < size) {
            wbuf = (wchar_t*)realloc(wbuf, size * sizeof(char));
            wbuf_size = size;
        }
        return wbuf;
    }
    LPCWSTR proc(const char* src, int cp = CP_UTF8) {
        size_t size = MultiByteToWideChar(cp, 0, src, -1, NULL, 0);
        LPWSTR buf = wget(size);
        MultiByteToWideChar(cp, 0, src, -1, wbuf, (int)size);
        return wbuf;
    }
    char* get(size_t size) {
        if (cbuf_size < size) {
            cbuf = (char*)realloc(cbuf, size * sizeof(char));
            cbuf_size = size;
        }
        return cbuf;
    }
    const char* proc(LPCWSTR src, int cp = CP_UTF8) {
        size_t size = WideCharToMultiByte(cp, 0, src, -1, NULL, 0, NULL, NULL);
        char* buf = get(size);
        WideCharToMultiByte(cp, 0, src, -1, buf, (int)size, NULL, NULL);
        return buf;
    }
};
extern StringConv ww_c1, ww_c2;
