#include "CppDLL.h"
#include "pch.h"
#include <iostream>
#include <cmath>

#define Expo extern "C" __declspec(dllexport)


int* Vertical(BYTE* Array, int imageHeight, int imageWidth, int bytesToCalculate, int start)
{
	//GXMATRIX:
	//1  0 -1
	//2  0 -2
	//1  0 -1

	int arraySize = imageHeight * imageWidth;
	int* GX = new int[arraySize];

	for (int i = 0; i < arraySize; i++) GX[i] = 0; //Init GX wih zeros


	for (int i = start; i < start + bytesToCalculate; i++)
	{
		int row = (i - i % imageWidth)/imageWidth; //safe row
		if (i % imageWidth == 0) //left wall
		{
			GX[i] += Array[i + 1] * -2 +Array[i] * 2;
			if (row != 0) GX[i] += Array[i + 1 - imageWidth] * -1 +Array[i] * 1; //Not top
			if (row != imageHeight - 1) GX[i] += Array[i + 1 + imageWidth] * -1 +Array[i] * 1; //Not bottom

		}
		else if (i % imageWidth == imageWidth - 1) //right wall
		{
			GX[i] += Array[i - 1] * 2 +Array[i-1] * -2;
			if (row != 0) GX[i] += Array[i - 1 - imageWidth] * 1 + Array[i - 1 - imageWidth] * -1; //Not top
			if (row != imageHeight - 1) GX[i] += Array[i - 1 + imageWidth] * 1 + Array[i - 1 + imageWidth] * -1; //Not bottom
		}
		else //center
		{
			GX[i] += Array[i + 1] * -2 + Array[i - 1] * 2;
			if (row != 0) GX[i] += Array[i + 1 - imageWidth] * -1 + Array[i - 1 - imageWidth] * 1;
			if (row != imageHeight - 1) GX[i] += Array[i + 1 + imageWidth] * -1 + Array[i - 1 + imageWidth] * 1;
		}
	}

	return GX;
}

int* Horizontal(BYTE* Array, int imageHeight, int imageWidth, int bytesToCalculate, int start)
{
	//GYMATRIX:	
	// 1  2  1
	// 0  0  0
	//-1 -2 -1

	int arraySize = imageHeight * imageWidth;
	int* GY = new int[arraySize];

	for (int i = 0; i < arraySize; i++) GY[i] = 0; //Init GY wih zeros

	for (int i = start; i < start + bytesToCalculate; i++)
	{
		int row = (i - i % imageWidth) / imageWidth;
		if (row == 0) //Top wall
		{
			GY[i] += Array[i + imageWidth] * -2 + Array[i]*2;
			if (i % imageWidth != 0) GY[i] += Array[i - 1 + imageWidth] *-1 + Array[i]*1;
			if (i % imageWidth != imageWidth - 1) GY[i] += Array[i + 1 + imageWidth]*-1 + Array[i]*1; //+ Array[i] is a correction
		}
		else if (row == imageHeight - 1) //Bottom wall
		{
			GY[i] += Array[i - imageWidth] * 2 + Array[i] * -2;
			if (i % imageWidth != 0) GY[i] += Array[i - 1 - imageWidth] *1 + Array[i]*-1;
			if (i % imageWidth != imageWidth - 1) GY[i] += Array[i + 1 - imageWidth]*1 + Array[i]*-1;
		}
		else //Center
		{
			GY[i] += Array[i + imageWidth] * -2 + Array[i - imageWidth] * 2;
			if (i % imageWidth != 0) GY[i] += Array[i - 1 + imageWidth] * -1 + Array[i - 1 - imageWidth] * 1;
			if (i % imageWidth != imageWidth - 1) GY[i] += Array[i + 1 + imageWidth] * -1 + Array[i + 1 - imageWidth] * 1;
		}
	}

	return GY;
}

Expo BYTE* Sobel(BYTE* grayArray, int imageHeight, int imageWidth, int bytesToCalculate, int start)
{
	int arraySize = imageHeight * imageWidth;

	int* Sobel_GX = Vertical(grayArray, imageHeight, imageWidth, bytesToCalculate, start);
	int* Sobel_GY = Horizontal(grayArray, imageHeight, imageWidth, bytesToCalculate, start);

	int* SobelMagnitude = new int[arraySize];

	for (int i = 0; i < arraySize; i++)
	{
		SobelMagnitude[i] = std::sqrt(pow(Sobel_GX[i], 2) + pow(Sobel_GY[i], 2));
	}

	delete[] Sobel_GX;
	delete[] Sobel_GY;


	//Normalization
	int minimum, maximum;
	minimum = maximum = SobelMagnitude[0];
	for (int i = 0; i < arraySize; i++)
	{
		if (SobelMagnitude[i] < minimum) minimum = SobelMagnitude[i];
		if (SobelMagnitude[i] > maximum) maximum = SobelMagnitude[i];
	}

	int difference = maximum - minimum;

	if (difference == 0) difference = 1; //preventing division by 0

	BYTE* result = new BYTE[arraySize];
	for (int i = 0; i < arraySize; i++)
	{
		result[i] = (SobelMagnitude[i] - minimum) / (difference * 1.0) * 255;
	}

	delete[] SobelMagnitude;

	return result;
}
