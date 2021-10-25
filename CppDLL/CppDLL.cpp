#include "CppDLL.h"
#include "pch.h"
#include <iostream>

#define Expo extern "C" __declspec(dllexport)

Expo void tangensMilion()
{
	double a = 0;
	for (int i = 0; i < 1000000; i++)
		a += i;
	return;
}

Expo BYTE** Sobel(BYTE** PixelArray, int arraySize, int imageHeight, int imageWidth)
{
	int GxMatrix[9] = { 1,0,-1,2,0,-2,1,0,-2 };
	int GyMatrix[9] = { 1,2,1,0,0,0,-1,-2,-1 };

	//[0][1][2]
	//[3][4][5]
	//[6][7][8]

	//GXMATRIX:
	//1  0 -1
	//2  0 -2
	//1  0 -1

	//GYMATRIX:	
	// 1  2  1
	// 0  0  0
	//-1 -2 -1

	int rowSize = imageWidth * 3 + imageWidth % 4;

	int** SobelArrayGX = new int*[rowSize];
	for (int i = 0; i < rowSize; i++)	SobelArrayGX[i] = new int[imageHeight];

	for (int i = 0; i < rowSize; i++)
		for (int j = 0; j < imageHeight; j++)
			SobelArrayGX[i][j] = 0;


	int** SobelArrayGY = new int* [rowSize];
	for (int i = 0; i < rowSize; i++)	SobelArrayGY[i] = new int[imageHeight];

	for (int i = 0; i < rowSize; i++)
		for (int j = 0; j < imageHeight; j++)
			SobelArrayGY[i][j] = 0;

	int currentByte = 0;
	int additionalPixel = imageWidth % 4;

	//Gx
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < rowSize - additionalPixel; x++)
		{
			if (x == 0) //Left wall
			{
				SobelArrayGX[x][y] += PixelArray[x + 1][y] * GxMatrix[5];
				if (y != 0) SobelArrayGX[x][y] += PixelArray[x + 1][y - 1] * GxMatrix[2];						//Not upper corner
				if(y!= (imageHeight -1)) SobelArrayGX[x][y] += PixelArray[x + 1][y + 1] * GxMatrix[8];		//Not bottom corner
			}
			else if (x == (imageWidth - 1)) //Right wall
			{
				SobelArrayGX[x][y] += PixelArray[x - 1][y] * GxMatrix[3];
				if (y != 0) SobelArrayGX[x][y] += PixelArray[x - 1][y - 1] * GxMatrix[0];						//Not upper corner
				if (y != (imageHeight - 1)) SobelArrayGX[x][y] += PixelArray[x - 1][y + 1] * GxMatrix[6];		//Not bottom corner
			}
			else //Center
			{
				if (y != 0) SobelArrayGX[x][y] += PixelArray[x - 1][y - 1] * GxMatrix[0] + PixelArray[x + 1][y - 1] * GxMatrix[2];					//Not top wall

				SobelArrayGX[x][y] += PixelArray[x - 1][y] * GxMatrix[3] + PixelArray[x + 1][y] * GxMatrix[5];										//center

				if (y != (imageHeight - 1)) SobelArrayGX[x][y] += PixelArray[x + 1][y + 1] * GxMatrix[8] + PixelArray[x - 1][y + 1] * GxMatrix[6];	//Not bottom wall
			}
			currentByte += 4;
		}
		currentByte += additionalPixel;
	}

	currentByte = 0;
	//Gy
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < rowSize-additionalPixel; x++)
		{
			if (y == 0) //Upper wall
			{
				SobelArrayGY[x][y] += PixelArray[x][y + 1] * GyMatrix[7];
				if (x != 0) SobelArrayGY[x][y] += PixelArray[x - 1][y + 1] * GyMatrix[6];					//Not right corner
				if (x != (imageWidth - 1))SobelArrayGY[x][y] += PixelArray[x + 1][y + 1] * GyMatrix[8];	//Not left corner
			}
			else if (y == (imageHeight - 1)) //Bottom wall
			{
				SobelArrayGY[x][y] += PixelArray[x][y - 1] * GyMatrix[1];
				if (x != 0) SobelArrayGY[x][y] += PixelArray[x - 1][y - 1] * GyMatrix[0];					//Not right corner
				if (x != (imageWidth - 1))SobelArrayGY[x][y] += PixelArray[x + 1][y - 1] * GyMatrix[2];	//Not left corner
			}
			else //Center
			{
				if (x != 0) SobelArrayGY[x][y] += PixelArray[x - 1][y - 1] * GyMatrix[0] + PixelArray[x - 1][y + 1] * GyMatrix[6];					//Not right wall
				SobelArrayGY[x][y] += PixelArray[x][y - 1] * GyMatrix[1] + PixelArray[x][y + 1] * GyMatrix[7];
				if (x != (imageWidth - 1)) SobelArrayGY[x][y] += +PixelArray[x + 1][y + 1] * GyMatrix[8] + PixelArray[x + 1][y - 1] * GyMatrix[2];	//Not left wall
			}
			currentByte += 4;
		}
		currentByte += additionalPixel;
	}

	int** SobelArray = new int* [rowSize];
	for (int i = 0; i < rowSize; i++)	SobelArray[i] = new int[imageHeight];

	for (int i = 0; i < rowSize; i++)
		for (int j = 0; j < imageHeight; j++)
			SobelArray[i][j] = sqrt(SobelArrayGX[i][j] * SobelArrayGX[i][j] + SobelArrayGY[i][j]* SobelArrayGY[i][j]);



	//Finding min and max
	int maximum, minimum;

	maximum = SobelArray[0][0];
	minimum = SobelArray[0][0];

	for (int y = 0; y < imageHeight; y++)
		for (int x = 0; x < imageWidth; x++)
		{
			if (SobelArray[x][y] > maximum) maximum = SobelArray[x][y];
			if (SobelArray[x][y] < minimum) minimum = SobelArray[x][y];
		}

	for (int y = 0; y < imageHeight; y++)
		for (int x = 0; x < imageWidth; x++)
		{
			std::cout << SobelArray[x][y]<<"\t";
		}



	//Normalisation of SobelArray to fit [0...255]

	if (minimum != 0) maximum -= minimum;

	float divider;
	maximum > 255 ? divider = maximum / 255 + 1 : divider = 255 / maximum + 1;

	for (int y = 0; y < imageHeight; y++)
		for (int x = 0; x < imageWidth; x++)
		{
			if (minimum != 0) SobelArray[x][y] -= minimum;
			SobelArray[x][y] /= divider;
		}

	return (BYTE**)SobelArray;

}