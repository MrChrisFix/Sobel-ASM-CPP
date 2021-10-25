#pragma once
#include "pch.h"

#ifdef CPPDLL_EXPORTS
#define CPPDLL_API __declspec(dllexport)
#else
#define CPPDLL_API __declspec(dllimport)
#endif

extern "C" CPPDLL_API void tangensMilion(); //TODO: delete this

extern "C" CPPDLL_API BYTE** Sobel(BYTE** PixelArray, int arraySize, int imageHeight, int imageWidth);