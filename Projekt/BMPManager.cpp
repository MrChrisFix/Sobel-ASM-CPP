#include "pch.h"
#include "BMPManager.h"
#include <vector>

BMPManager::BMPManager(std::string fileLocation)
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

BMPManager::~BMPManager()
{
	delete[] this->byteFile;
	delete[] this->PixelArray;

	if (this->File.is_open()) this->File.close();
}


System::Drawing::Bitmap^ BMPManager::getBitmap() //Not used anymore
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

System::Drawing::Bitmap^ BMPManager::getBitmap2()
{
	System::Drawing::Bitmap^ grayImage = gcnew System::Drawing::Bitmap(this->BMPInfoHeader.biWidth, this->BMPInfoHeader.biHeight);

	System::Drawing::Rectangle rect = System::Drawing::Rectangle(0, 0, grayImage->Width, grayImage->Height);

	System::Drawing::Imaging::BitmapData^ imageData = grayImage->LockBits(rect, System::Drawing::Imaging::ImageLockMode::WriteOnly, System::Drawing::Imaging::PixelFormat::Format24bppRgb);


	int length = imageData->Stride * imageData->Height;

	array<BYTE>^ bytes = gcnew array<BYTE>(length);

	int row = 4, column = 0;;
	for (int i = 0; i < length; i++, column++)
	{
		if (i != 0 && i % imageData->Width == 0) { row--, column = 0; };
		bytes[i] = this->PixelArray[row*(imageData->Width)+column];
	}

	System::Runtime::InteropServices::Marshal::Copy( bytes, 0, imageData->Scan0, length);

	grayImage->UnlockBits(imageData);

	return grayImage;
}



System::Drawing::Bitmap^ BMPManager::createBitmap(BYTE** PixelArray)
{
	System::Drawing::Bitmap^ Image = gcnew System::Drawing::Bitmap(this->BMPInfoHeader.biWidth, this->BMPInfoHeader.biHeight);

	System::Drawing::Rectangle rect = System::Drawing::Rectangle(0, 0, Image->Width, Image->Height);

	System::Drawing::Imaging::BitmapData^ imageData = Image->LockBits(rect, System::Drawing::Imaging::ImageLockMode::WriteOnly, System::Drawing::Imaging::PixelFormat::Format24bppRgb);


	int length = imageData->Stride * imageData->Height;

	array<BYTE>^ bytes = gcnew array<BYTE>(length);

	size_t currentByte = 0;
	int addtionalPixels = Image->Width % 4;
	for (int y = imageData->Height - 1; y >= 0; y--)
	{
		for (int x = 0; x < imageData->Stride; x++)
		{
			bytes[currentByte] = this->PixelArray2D[x][y];
			currentByte++;
		}
		//currentByte += addtionalPixels;
	}

	System::Runtime::InteropServices::Marshal::Copy(bytes, 0, imageData->Scan0, length);

	Image->UnlockBits(imageData);

	return Image;

}

unsigned char* BMPManager::getPixelArray()
{
	return this->PixelArray;
}

unsigned char** BMPManager::getPixelArray2D()
{
	return this->PixelArray2D;
}

int BMPManager::getWidth()
{
	return this->BMPInfoHeader.biWidth;
}

int BMPManager::getHeight()
{
	return this->BMPInfoHeader.biHeight;
}



void BMPManager::distributeByteFile()
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

}

void BMPManager::extractPixelData()
{
	//1d array

	this->PixelArray = new unsigned char[this->BMPInfoHeader.biSizeImage];

	for(size_t i = 0; i< this->BMPInfoHeader.biSizeImage; i++)
		this->PixelArray[i] = this->byteFile[this->fileHeader.Offset+i];


	if (!checkIfGray()) changeToGrayScale();

	//2d array
	int additionalPixels = this->BMPInfoHeader.biWidth % 4;
	int bytesInRow = this->BMPInfoHeader.biWidth * 3 + additionalPixels;

	this->PixelArray2D = new unsigned char* [bytesInRow];
	for (int i = 0; i < bytesInRow; i++)
		this->PixelArray2D[i] = new unsigned char[this->BMPInfoHeader.biHeight];

	for (int y = this->BMPInfoHeader.biHeight-1; y >=0 ; y--)
		for (int x = 0; x < bytesInRow; x++)
		{
			this->PixelArray2D[x][y] = this->PixelArray[y * (bytesInRow) + x];	//PixelArray2D is now in order form left top corner to right ottom corner
		}																		//where PixelArray is as always from left bottom corner to right top corner

	//INFO:
	//this->BMPInfoHeader.biSizeImage == (this->BMPInfoHeader.biWidth*3 + additionalPixels) * BMPInfoHeader.biHeight)

}

bool BMPManager::checkIfGray()
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

void BMPManager::changeToGrayScale()
{
	int addtionalPixels = this->BMPInfoHeader.biWidth % 4;

	size_t currentByte = 0;
	for (int y = this->BMPInfoHeader.biHeight - 1; y >= 0; y--)
	{
		for (int x = 0; x < this->BMPInfoHeader.biWidth; x++)
		{
			//unsigned char sum = this->PixelArray[currentByte] + this->PixelArray[currentByte + 1] + this->PixelArray[currentByte + 2];

			float blue = this->PixelArray[currentByte + 2] / 255.0f;
			float green = this->PixelArray[currentByte + 1] / 255.0f;
			float red = this->PixelArray[currentByte] / 255.0f;

			float waged = red * 0.2126 + green * 0.7152 + blue * 0.0722; //Good waged grayscale transformation

			for (int i = 0; i < 3; i++)
				this->PixelArray[currentByte + i] = waged * 255.0f;
			currentByte += 3;
		}
		currentByte += addtionalPixels;
	}

	//if (checkIfGray()) std::cout << "Szare"; else std::cout << "Nie jest szare";
}



unsigned __int8 BMPManager::read8(int& byte) //1 Byte
{
	unsigned __int8 b0;
	b0 = this->byteFile[byte++];
	return b0;
}

unsigned __int16 BMPManager::read16(int& byte) //2 Bytes
{
	unsigned __int8 b0, b1;

	b0 = this->byteFile[byte++];
	b1 = this->byteFile[byte++];

	return ((b1<<8)|b0);
}

unsigned __int32 BMPManager::read32(int& byte) //4 Bytes
{
	unsigned __int8 b0, b1, b2, b3;

	b0 = this->byteFile[byte++];
	b1 = this->byteFile[byte++];
	b2 = this->byteFile[byte++];
	b3 = this->byteFile[byte++];

	return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}