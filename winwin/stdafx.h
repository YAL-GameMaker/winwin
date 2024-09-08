// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _WINDOWS
	#include "targetver.h"
	
	#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
	#include <windows.h>
	#include <d3d11.h>
#endif

#if defined(WIN32)
#define dllx extern "C" __declspec(dllexport)
#elif defined(GNUC)
#define dllx extern "C" __attribute__ ((visibility("default"))) 
#else
#define dllx extern "C"
#endif

#define trace(...) { printf("[" __FUNCTION__ "] ", __LINE__); printf(__VA_ARGS__); printf("\n"); fflush(stdout); }

template<typename T> T* malloc_arr(size_t count) {
	return (T*)malloc(sizeof(T) * count);
}
template<typename T> T* realloc_arr(T* arr, size_t count) {
	return (T*)realloc(arr, sizeof(T) * count);
}
template<typename T> T* memcpy_arr(T* dst, const T* src, size_t count) {
	return (T*)memcpy(dst, src, sizeof(T) * count);
}

#include "gml_ext.h"

// TODO: reference additional headers your program requires here