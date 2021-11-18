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
	int* GX = new int[bytesToCalculate];

	for (int i = 0; i < bytesToCalculate; i++) GX[i] = 0; //Init GX wih zeros


	for (int i = start; i < start + bytesToCalculate; i++)
	{
		int row = (i - i % imageWidth)/imageWidth; //safe row
		if (i % imageWidth == 0) //left wall
		{
			GX[i-start] += Array[i + 1] * -2 +Array[i] * 2;
			if (row != 0) GX[i - start] += Array[i + 1 - imageWidth] * -1 +Array[i] * 1; //Not top
			if (row != imageHeight - 1) GX[i - start] += Array[i + 1 + imageWidth] * -1 +Array[i] * 1; //Not bottom

		}
		else if (i % imageWidth == imageWidth - 1) //right wall
		{
			GX[i - start] += Array[i - 1] * 2 +Array[i-1] * -2;
			if (row != 0) GX[i - start] += Array[i - 1 - imageWidth] * 1 + Array[i - 1 - imageWidth] * -1; //Not top
			if (row != imageHeight - 1) GX[i - start] += Array[i - 1 + imageWidth] * 1 + Array[i - 1 + imageWidth] * -1; //Not bottom
		}
		else //center
		{
			GX[i - start] += Array[i + 1] * -2 + Array[i - 1] * 2;
			if (row != 0) GX[i - start] += Array[i + 1 - imageWidth] * -1 + Array[i - 1 - imageWidth] * 1;
			if (row != imageHeight - 1) GX[i - start] += Array[i + 1 + imageWidth] * -1 + Array[i - 1 + imageWidth] * 1;
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
	int* GY = new int[bytesToCalculate];

	for (int i = 0; i < bytesToCalculate; i++) GY[i] = 0; //Init GY wih zeros

	for (int i = start; i < start + bytesToCalculate; i++)
	{
		int row = (i - i % imageWidth) / imageWidth;
		if (row == 0) //Top wall
		{
			GY[i - start] += Array[i + imageWidth] * -2 + Array[i]*2;
			if (i % imageWidth != 0) GY[i - start] += Array[i - 1 + imageWidth] *-1 + Array[i]*1;
			if (i % imageWidth != imageWidth - 1) GY[i - start] += Array[i + 1 + imageWidth]*-1 + Array[i]*1; //+ Array[i] is a correction
		}
		else if (row == imageHeight - 1) //Bottom wall
		{
			GY[i - start] += Array[i - imageWidth] * 2 + Array[i] * -2;
			if (i % imageWidth != 0) GY[i - start] += Array[i - 1 - imageWidth] *1 + Array[i]*-1;
			if (i % imageWidth != imageWidth - 1) GY[i - start] += Array[i + 1 - imageWidth]*1 + Array[i]*-1;
		}
		else //Center
		{
			GY[i - start] += Array[i + imageWidth] * -2 + Array[i - imageWidth] * 2;
			if (i % imageWidth != 0) GY[i - start] += Array[i - 1 + imageWidth] * -1 + Array[i - 1 - imageWidth] * 1;
			if (i % imageWidth != imageWidth - 1) GY[i - start] += Array[i + 1 + imageWidth] * -1 + Array[i + 1 - imageWidth] * 1;
		}
	}

	return GY;
}

Expo BYTE* Sobel(BYTE* grayArray, int imageHeight, int imageWidth, int bytesToCalculate, int start, int* calculatedArray)
{
	int arraySize = imageHeight * imageWidth;

	int* Sobel_GX = Vertical(grayArray, imageHeight, imageWidth, bytesToCalculate, start);
	int* Sobel_GY = Horizontal(grayArray, imageHeight, imageWidth, bytesToCalculate, start);

	int* SobelMagnitude = new int[bytesToCalculate];

	for (int i = 0; i < bytesToCalculate; i++)
	{
		SobelMagnitude[i] = std::sqrt(pow(Sobel_GX[i], 2) + pow(Sobel_GY[i], 2));
	}

	delete[] Sobel_GX;
	delete[] Sobel_GY;


	for (int i = start; i < start + bytesToCalculate; i++)
	{
		calculatedArray[i] = SobelMagnitude[i - start];
	}

	delete[] SobelMagnitude;
		
	return nullptr;
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
