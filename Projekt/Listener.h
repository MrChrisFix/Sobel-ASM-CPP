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
	Bitmap^ CppChangedBitmap;
	Bitmap^ AsmChangedBitmap;

	BMPManager* bmpManager;
	Sobel_CPP Cpp;
	Sobel_ASM Asm;


public:
	Listener();
	~Listener();

	Bitmap^ reactOnFileSelected(System::String^ fileName);

	std::chrono::duration<double> reactOnStartButton(short id, short threadNumber, System::Windows::Forms::PictureBox^ pictureBox);
};

