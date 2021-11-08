#pragma once
#include "pch.h"

#ifdef CPPDLL_EXPORTS
#define CPPDLL_API __declspec(dllexport)
#else
#define CPPDLL_API __declspec(dllimport)
#endif


int* Vertical(BYTE* Array, int imageHeight, int imageWidth, int bytesToCalculate, int start);

int* Horizontal(BYTE* Array, int imageHeight, int imageWidth, int bytesToCalculate, int start);

extern "C" CPPDLL_API BYTE* Sobel(BYTE* PixelArray, int imageHeight, int imageWidth, int bytesToCalculate, int start, int* calculatedArray);

extern "C" CPPDLL_API void Normalize(int * source, int minimum, int maximum, int bytesToCalculate, int start, BYTE * &calculatedArray);