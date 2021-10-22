#include "pch.h"
#include "ReadBMP.h"
#include <vector>

ReadBMP::ReadBMP(std::string fileLocation)
{
	this->File.open(fileLocation, std::ios::in | std::ios::binary | std::ios::ate);

	if (this->File.is_open())
	{
		size_t size = this->File.tellg();
		this->File.seekg(0, std::ios::beg);

		this->byteFile = new char[size];

		
		this->File.read(byteFile, size);	

		//std::cout << byteFile;

		this->File.close();
	}
	this->distributeByteFile();
}

ReadBMP::~ReadBMP()
{
	/*for (int i = 0; i < this->BMPInfoHeader.biWidth; i++)
		delete[] this->RGBPixelMatrix[i];
	delete[] this->RGBPixelMatrix;*/

	if (this->File.is_open()) this->File.close();
}

bool ReadBMP::checkIfGray()
{
	/*for (int i = 0; i < this->BMPInfoHeader.biWidth; i++)
		for (int j = 0; j < this->BMPInfoHeader.biHeight; j++)
			if (this->RGBPixelMatrix[i][j].Blue != this->RGBPixelMatrix[i][j].Green || this->RGBPixelMatrix[i][j].Green != this->RGBPixelMatrix[i][j].Red)
				return false;*/

	size_t currentByte = 0;
	while (currentByte < this->BMPInfoHeader.biWidth*this->BMPInfoHeader.biHeight)
	{
		if (this->byteFile[this->fileHeader.Offset + currentByte] != this->byteFile[this->fileHeader.Offset + currentByte + 1]) return false;
		else currentByte += 4;
	}

	return true;
}

void ReadBMP::changeToGrayScale()
{
	//for (int i = 0; i < this->BMPInfoHeader.biWidth; i++)
	//	for (int j = 0; j < this->BMPInfoHeader.biHeight; j++)
	//	{
	//		short average = this->RGBPixelMatrix[i][j].Blue/3 + this->RGBPixelMatrix[i][j].Green/3 + this->RGBPixelMatrix[i][j].Red/3;
	//		//average > 255 ? average = 255 : average = average;
	//		//average < 0 ? average = 0 : average = average;
	//		this->RGBPixelMatrix[i][j].Blue = average;
	//		this->RGBPixelMatrix[i][j].Green = average;
	//		this->RGBPixelMatrix[i][j].Red = average;
	//	}

	size_t currentByte = 0;
	while (currentByte < this->BMPInfoHeader.biWidth * this->BMPInfoHeader.biHeight)
	{
		unsigned char sum =this->byteFile[this->fileHeader.Offset + currentByte] + this->byteFile[this->fileHeader.Offset + currentByte + 1] + this->byteFile[this->fileHeader.Offset + currentByte + 2];
		unsigned char average = sum / 3;
		this->byteFile[this->fileHeader.Offset + currentByte] = average;
		this->byteFile[this->fileHeader.Offset + currentByte+1] = average;
		this->byteFile[this->fileHeader.Offset + currentByte+2] = average;
		currentByte += 4;
	}

	//if (checkIfGray()) std::cout << "Szare"; else std::cout << "Nie jest szare";
}

void ReadBMP::getBitmap(System::Drawing::Bitmap^ grayImage)
{
	grayImage = gcnew System::Drawing::Bitmap(this->BMPInfoHeader.biWidth, this->BMPInfoHeader.biHeight);

	//grayImage->SetResolution(this->BMPInfoHeader.biWidth, this->BMPInfoHeader.biHeight);
	
	size_t currentByte = 0;
	for(int x =0; x<grayImage->Width; x++)
		for (int y = 0; y < grayImage->Height; y++)
		{
			grayImage->SetPixel(x, y, System::Drawing::Color::FromArgb(
				this->byteFile[this->fileHeader.Offset + currentByte + 2],
				this->byteFile[this->fileHeader.Offset + currentByte + 1],
				this->byteFile[this->fileHeader.Offset + currentByte]
			));
			currentByte += 4;
		}
	//R
	//this->byteFile[this->fileHeader.Offset+currentByte+2]
	//G
	//this->byteFile[this->fileHeader.Offset + currentByte + 1]
	//B
	//this->byteFile[this->fileHeader.Offset + currentByte]
}



void ReadBMP::distributeByteFile()
{
	int currentByte = 0;

	//Set the file header
	this->fileHeader.fType = read16(currentByte);
	this->fileHeader.fSize = read32(currentByte);
	this->fileHeader.reserved1 = read16(currentByte);
	this->fileHeader.reserved2 = read16(currentByte);
	this->fileHeader.Offset = read32(currentByte);


	//Set the BMP info header
	this->BMPInfoHeader.biSize = read32(currentByte);
	this->BMPInfoHeader.biWidth = read32(currentByte);
	this->BMPInfoHeader.biHeight = read32(currentByte);
	this->BMPInfoHeader.biPlanes = read16(currentByte);
	this->BMPInfoHeader.biBitCount = read16(currentByte);
	this->BMPInfoHeader.biCompression = read32(currentByte);
	this->BMPInfoHeader.biSizeImage = read32(currentByte);
	this->BMPInfoHeader.biXPelsPreMeter = read32(currentByte);
	this->BMPInfoHeader.biYPelsPreMeter = read32(currentByte);
	this->BMPInfoHeader.biClrUsed = read32(currentByte);
	this->BMPInfoHeader.biClrImportent = read8(currentByte);
	this->BMPInfoHeader.biClrRotation = read8(currentByte);
	this->BMPInfoHeader.biReserved = read16(currentByte);

	if (!checkIfGray()) changeToGrayScale();

	/*this->RGBPixelMatrix = new RGBColour * [this->BMPInfoHeader.biWidth];
	for (int i = 0; i < this->BMPInfoHeader.biWidth; i++)
		this->RGBPixelMatrix[i] = new RGBColour[this->BMPInfoHeader.biWidth];

	for (int i = 0; i < this->BMPInfoHeader.biWidth; i++)
		for (int j = 0; j < this->BMPInfoHeader.biHeight; j++)
		{
			this->RGBPixelMatrix[i][j].Blue = this->File.get();
			this->RGBPixelMatrix[i][j].Green = this->File.get();
			this->RGBPixelMatrix[i][j].Red = this->File.get();
		}*/

}

unsigned __int8 ReadBMP::read8(int& byte) //1 Byte
{
	unsigned __int8 b0;
	b0 = this->byteFile[byte++];
	return b0;
}

unsigned __int16 ReadBMP::read16(int& byte) //2 Bytes
{
	unsigned __int8 b0, b1;

	b0 = this->byteFile[byte++];
	b1 = this->byteFile[byte++];

	return ((b1<<8)|b0);
}

unsigned __int32 ReadBMP::read32(int& byte) //4 Bytes
{
	unsigned __int8 b0, b1, b2, b3;

	b0 = this->byteFile[byte++];
	b1 = this->byteFile[byte++];
	b2 = this->byteFile[byte++];
	b3 = this->byteFile[byte++];

	return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}
