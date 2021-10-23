#include "CppDLL.h"
#include "pch.h"

#define Expo extern "C" __declspec(dllexport)

Expo void tangensMilion()
{
	double a = 0;
	for (int i = 0; i < 1000000; i++)
		a += i;
	return;
}

Expo BYTE** Sobel(BYTE** PixelArray, int arraySize, int numerOfThreads, int imageHeight, int imageWidth)
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

	int** SobelArray = new int*[imageWidth];
	for (int i = 0; i < imageWidth; i++)	SobelArray[i] = new int[imageHeight];

	for (int i = 0; i < imageWidth; i++)
		for (int j = 0; j < imageHeight; j++)
			SobelArray[i][j] = 0;

	int currentByte = 0;
	int additionalPixel = imageWidth % 4;

	//Gx
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			if (x == 0) //Left wall
			{
				SobelArray[x][y] += PixelArray[x + 1][y] * GxMatrix[5];
				if (y != 0) SobelArray[x][y] += PixelArray[x + 1][y - 1] * GxMatrix[2];						//Not upper corner
				if(y!= (imageHeight -1)) SobelArray[x][y] += PixelArray[x + 1][y + 1] * GxMatrix[8];		//Not bottom corner
			}
			else if (x == (imageWidth - 1)) //Right wall
			{
				SobelArray[x][y] += PixelArray[x - 1][y] * GxMatrix[3];
				if (y != 0) SobelArray[x][y] += PixelArray[x - 1][y - 1] * GxMatrix[0];						//Not upper corner
				if (y != (imageHeight - 1)) SobelArray[x][y] += PixelArray[x - 1][y + 1] * GxMatrix[6];		//Not bottom corner
			}
			else //Center
			{
				SobelArray[x][y] +=
					PixelArray[x - 1][y - 1] * GxMatrix[0] + PixelArray[x + 1][y - 1] * GxMatrix[2] + 
					PixelArray[x - 1][y] * GxMatrix[3] + PixelArray[x + 1][y] * GxMatrix[5] + 
					PixelArray[x + 1][y + 1] * GxMatrix[8] + PixelArray[x - 1][y + 1] * GxMatrix[6];
			}
			currentByte += 4;
		}
		currentByte += additionalPixel;
	}

	currentByte = 0;
	//Gy
	for (int y = 0; y < imageHeight; y++)
	{
		for (int x = 0; x < imageWidth; x++)
		{
			if (y == 0) //Upper wall
			{
				SobelArray[x][y] += PixelArray[x][y + 1] * GyMatrix[7];
				if (x != 0) SobelArray[x][y] += PixelArray[x - 1][y + 1] * GyMatrix[6];					//Not right corner
				if (x != (imageWidth - 1))SobelArray[x][y] += PixelArray[x + 1][y + 1] * GyMatrix[8];	//Not left corner
			}
			if (y == (imageHeight - 1)) //Bottom wall
			{
				SobelArray[x][y] += PixelArray[x][y - 1] * GyMatrix[1];
				if (x != 0) SobelArray[x][y] += PixelArray[x - 1][y - 1] * GyMatrix[0];					//Not right corner
				if (x != (imageWidth - 1))SobelArray[x][y] += PixelArray[x + 1][y - 1] * GyMatrix[2];	//Not left corner
			}
			else //Center
			{
				SobelArray[x][y] +=
					PixelArray[x - 1][y - 1] * GyMatrix[0] + PixelArray[x][y - 1] * GyMatrix[1] + PixelArray[x + 1][y - 1] * GyMatrix[2] +
					PixelArray[x - 1][y + 1] * GyMatrix[6] + PixelArray[x][y + 1] * GyMatrix[7] + PixelArray[x + 1][y + 1] * GyMatrix[8];
			}
			currentByte += 4;
		}
		currentByte += additionalPixel;
	}


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


	//Normalisation of SobelArray to fit [0...255]

	
	if (minimum != 0) maximum -= minimum;

	int divider;
	maximum > 255 ? divider = maximum / 255 : divider = 255 / maximum;

	for (int y = 0; y < imageHeight; y++)
		for (int x = 0; x < imageWidth; x++)
		{
			if (minimum != 0) SobelArray[x][y] -= minimum;
			SobelArray[x][y] /= divider;
		}

	return (BYTE**)SobelArray;

}