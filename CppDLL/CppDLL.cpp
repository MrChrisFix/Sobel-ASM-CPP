#include "CppDLL.h"
#include "pch.h"
#include <iostream>
#include <cmath>

#define Expo extern "C" __declspec(dllexport)

Expo void Sobel(BYTE* grayArray, int imageHeight, int imageWidth, int bytesToCalculate, int start, int* calculatedArray)
{
	//arraySize = imageHeight * imageWidth;

	int GX, GY;

	for (int i = start; i < start + bytesToCalculate; i++)
	{
		int row = (i - i % imageWidth) / imageWidth;	//safe row calculation

		if (i % imageWidth == 0 || i % imageWidth == imageWidth - 1 || row == 0) continue;
		else if (row == imageHeight - 1) break;
		GX = grayArray[i + 1] * -2 + grayArray[i - 1] * 2 +
			grayArray[i + 1 - imageWidth] * -1 + grayArray[i - 1 - imageWidth] * 1 +
			grayArray[i + 1 + imageWidth] * -1 + grayArray[i - 1 + imageWidth] * 1;

		GY = grayArray[i + imageWidth] * -2 + grayArray[i - imageWidth] * 2 +
			grayArray[i - 1 + imageWidth] * -1 + grayArray[i - 1 - imageWidth] * 1 +
			grayArray[i + 1 + imageWidth] * -1 + grayArray[i + 1 - imageWidth] * 1;

		//power of 2
		GX *= GX;
		GY *= GY;
		calculatedArray[i] = std::sqrt(GX + GY);
	}
	return;
}

Expo void Normalize(int* source, int minimum, int maximum, int bytesToCalculate, int start, BYTE*& calculatedArray)
{
	int difference = maximum - minimum;

	if (difference == 0) difference = 1; //preventing division by 0

	for (int i = start; i < start+bytesToCalculate; i++)
	{
		calculatedArray[i] = (source[i] - minimum) / (difference * 1.0) * 255;
	}

}
