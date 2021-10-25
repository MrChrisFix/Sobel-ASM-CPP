#pragma once
#include <chrono>
#include "BMPManager.h"

using namespace System::Drawing;

ref class Listener
{
private:

	Bitmap^ originalBitmap;
	Bitmap^ grayBitmap;
	Bitmap^ CppChangedBitmap;
	Bitmap^ AsmChangedBitmap;

	BMPManager* bmpManager;

public:
	Listener();
	~Listener();

	Bitmap^ reactOnFileSelected(System::String^ fileName);

	std::chrono::duration<double> reactOnStartButton(short id, short threadNumberId, System::Windows::Forms::PictureBox^ pictureBox);
};

