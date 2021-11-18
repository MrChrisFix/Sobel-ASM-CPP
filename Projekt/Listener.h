#pragma once
#include <chrono>
#include "BMPManager.h"
#include "Sobel_CPP.h"
#include "Sobel_ASM.h"

using namespace System::Drawing;

ref class Listener
{
private:

	Bitmap^ originalBitmap;
	Bitmap^ grayBitmap;
	Bitmap^ CppBitmap;
	Bitmap^ AsmBitmap;

	BMPManager* bmpManager;
	Sobel_CPP Cpp;
	Sobel_ASM Asm;

	int currentImage;


public:
	Listener();
	~Listener();

	Bitmap^ reactOnFileSelected(System::String^ fileName);
	Bitmap^ getChangedBitmap();
	void saveBitmap(System::String^ fileName);

	std::chrono::duration<double> reactOnStartButton(short id, short threadNumber, System::Windows::Forms::PictureBox^ pictureBox);

private:

	void deleteAllBitmaps();
};

