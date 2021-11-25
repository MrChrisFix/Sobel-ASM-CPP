#pragma once
#include <string>
#include <fstream>
#include <iostream>

class BMPManager
{
private:
	struct fHead
	{
		unsigned __int16 fType;		//DIB identification number
		unsigned __int32 fSize;		//File size in bytes
		unsigned __int16 reserved1;	//Reserved
		unsigned __int16 reserved2;	//Reserved
		unsigned __int32 Offset;	//Offset up to the pixel bits

	}fileHeader;

	struct DIB
	{
		unsigned __int32 biSize;			//Size of the DIB
		unsigned __int32 biWidth;			//Image width
		unsigned __int32 biHeight;			//Image height
		unsigned __int16 biPlanes;			//Number of colour layers
		unsigned __int16 biBitCount;		//BitsPerPixel
		unsigned __int32 biCompression;		//Compression algorythm
		unsigned __int32 biSizeImage;		//Size of the image alone
		unsigned __int32 biXPelsPreMeter;	//Vertical reslution
		unsigned __int32 biYPelsPreMeter;	//Horizontal resolution
		unsigned __int32 biClrUsed;			//Colours in palette
		unsigned __int8 biClrImportent;		//Number of important colours in palette
		unsigned __int8 biClrRotation;		//Flag for palette rotation
		unsigned __int16 biReserved;		//Reserved
	}BMPInfoHeader;

	

private:
	std::ifstream File;

	unsigned char* byteFile;

	unsigned char* PixelArray;

	unsigned char* grayOneChannelArray;

public:
	BMPManager(std::string fileLocation);
	~BMPManager();

	System::Drawing::Bitmap^ getBitmap(); //Only for debug

	System::Drawing::Bitmap^ createBitmapFromGray();
	System::Drawing::Bitmap^ createBitmapFromGray(unsigned char* Array);

	bool isOk = true;

	unsigned char* getPixelArray();

	unsigned char* getGrayArray();
	void setGrayArray(unsigned char* arr);

	int getWidth();
	int getHeight();

private:

	void fileFormatNotSupported();

	bool checkIfGray();
	void changeToGrayScale();

	void distributeByteFile();

	void extractPixelData();

	unsigned __int8 read8(int &byte);
	unsigned __int16 read16(int &byte);
	unsigned __int32 read32(int &byte);


};

