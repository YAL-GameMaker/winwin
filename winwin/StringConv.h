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
    void init() {
        cbuf = nullptr;
        cbuf_size = 0;
        wbuf = nullptr;
        wbuf_size = 0;
    }
    LPWSTR wget(size_t size) {
        if (wbuf_size < size) {
            if (wbuf != nullptr) {
                wbuf = realloc_arr(wbuf, size);
            } else wbuf = malloc_arr<wchar_t>(size);
            wbuf_size = size;
        }
        return wbuf;
    }
    LPCWSTR proc(const char* src, int cp = CP_UTF8) {
        auto size = MultiByteToWideChar(cp, 0, src, -1, NULL, 0);
        LPWSTR buf = wget((size_t)size);
        MultiByteToWideChar(cp, 0, src, -1, wbuf, size);
        return wbuf;
    }
    char* get(size_t size) {
        if (cbuf_size < size) {
            if (cbuf != nullptr) {
                cbuf = realloc_arr(cbuf, size);
            } else cbuf = malloc_arr<char>(size);
            cbuf_size = size;
        }
        return cbuf;
    }
    char* proc(LPCWSTR src, int cp = CP_UTF8) {
        auto size = WideCharToMultiByte(cp, 0, src, -1, NULL, 0, NULL, NULL);
        char* buf = get((size_t)size);
        WideCharToMultiByte(cp, 0, src, -1, buf, size, NULL, NULL);
        return buf;
    }
};
extern StringConv ww_c1, ww_c2;
