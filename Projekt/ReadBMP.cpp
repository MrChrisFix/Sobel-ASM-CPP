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

		this->byteFile = new unsigned char[size];
	
		this->File.read(reinterpret_cast<char*>(byteFile), size);	

		this->File.close();
	}
	this->distributeByteFile();
}

ReadBMP::~ReadBMP()
{
	delete[] this->byteFile;
	delete[] this->PixelArray;

	if (this->File.is_open()) this->File.close();
}

bool ReadBMP::checkIfGray()
{
	int addtionalPixels = this->BMPInfoHeader.biWidth % 4;
	size_t currentByte = 0;

	for (int y = this->BMPInfoHeader.biHeight - 1; y >= 0; y--)
	{
		for (int x = 0; x < this->BMPInfoHeader.biWidth; x++)
		{
			if (this->PixelArray[currentByte + 2] != this->PixelArray[currentByte + 1] || this->PixelArray[currentByte + 1] != this->PixelArray[currentByte]) return false;
			currentByte += 3;
		}
		currentByte += addtionalPixels;
	}

	return true;
}

void ReadBMP::changeToGrayScale()
{
	int addtionalPixels = this->BMPInfoHeader.biWidth % 4;

	size_t currentByte = 0;
	for (int y = this->BMPInfoHeader.biHeight - 1; y >= 0; y--)
	{
		for (int x = 0; x < this->BMPInfoHeader.biWidth; x++)
		{
			//unsigned char sum = this->PixelArray[currentByte] + this->PixelArray[currentByte + 1] + this->PixelArray[currentByte + 2];

			float blue = this->PixelArray[currentByte + 2]/255.0f;
			float green = this->PixelArray[currentByte + 1]/255.0f;
			float red = this->PixelArray[currentByte]/255.0f;

			float waged = red * 0.2126 + green * 0.7152 + blue * 0.0722; //Good waged grayscale transformation

			for (int i = 0; i < 3; i++)
				this->PixelArray[currentByte + i] = waged * 255.0f;
			currentByte += 3;
		}
		currentByte += addtionalPixels;
	}

	//if (checkIfGray()) std::cout << "Szare"; else std::cout << "Nie jest szare";
}

System::Drawing::Bitmap^ ReadBMP::getBitmap()
{
	System::Drawing::Bitmap^ greyImage = gcnew System::Drawing::Bitmap(this->BMPInfoHeader.biWidth, this->BMPInfoHeader.biHeight);

	size_t currentByte = 0;
	int addtionalPixels = greyImage->Width % 4;

	for (int y = greyImage->Height - 1; y >= 0; y--)
	{
		for (int x = 0; x < greyImage->Width; x++)
		{
			greyImage->SetPixel(x, y, System::Drawing::Color::FromArgb(
				this->PixelArray[currentByte + 2], this->PixelArray[currentByte + 1], this->PixelArray[currentByte] //all rgb in gray are the same
			));
			currentByte += 3;
			if (currentByte > this->BMPInfoHeader.biSizeImage)
				return greyImage;
		}
		currentByte += addtionalPixels;
	}

	return greyImage;
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

	this->extractPixelData();

	if (!checkIfGray()) changeToGrayScale();

}

void ReadBMP::extractPixelData()
{
	this->PixelArray = new unsigned char[this->BMPInfoHeader.biSizeImage];

	for(size_t i = 0; i< this->BMPInfoHeader.biSizeImage; i++)
		this->PixelArray[i] = this->byteFile[this->fileHeader.Offset+i];
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
