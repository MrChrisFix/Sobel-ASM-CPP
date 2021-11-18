#include "Listener.h"
#include <msclr/marshal_cppstd.h>
#include "Sobel_CPP.h"
#include "Sobel_ASM.h"

Listener::Listener() : Cpp(), Asm()
{
	this->originalBitmap = nullptr;
	this->grayBitmap = nullptr;
	this->CppBitmap = nullptr;

	this->bmpManager = nullptr;
	this->currentImage = -1;
}

Listener::~Listener()
{
	this->deleteAllBitmaps();
}

System::Drawing::Bitmap^ Listener::reactOnFileSelected(System::String^ fileName)
{
	this->deleteAllBitmaps();

	this->originalBitmap = gcnew Bitmap(fileName);

	if (this->bmpManager) delete bmpManager;
	this->bmpManager = new BMPManager(msclr::interop::marshal_as<std::string>(fileName));

	if (this->grayBitmap != nullptr) delete this->grayBitmap;
	this->grayBitmap = this->bmpManager->createBitmapFromGray();

	return this->originalBitmap;
}

Bitmap^ Listener::getChangedBitmap()
{
	if (currentImage == 0) return this->AsmBitmap;
	if (currentImage == 1) return this->CppBitmap;

	return nullptr;
}

void Listener::saveBitmap(System::String^ fileName)
{
	//this->bmpManager

}

std::chrono::duration<double> Listener::reactOnStartButton(short id, short threadNumber, System::Windows::Forms::PictureBox^ pictureBox)
{
	std::chrono::duration<double> elapsed_seconds;

	if (this->originalBitmap != nullptr)
	{
		if (id == 0) //Asembler
		{
			currentImage = 0;
			this->Asm.executeInASM(threadNumber, bmpManager);
			pictureBox->Image = this->grayBitmap; //this->AsmChangedBitmap;
		}
		else if (id == 1) //C++
		{
			currentImage = 1;
			BYTE* arrayPtr;

			elapsed_seconds = this->Cpp.executeInCpp(threadNumber, bmpManager, arrayPtr);
			this->CppBitmap = this->bmpManager->createBitmapFromGray(arrayPtr);
			pictureBox->Image = this->CppBitmap;
			
			if(arrayPtr!=nullptr) delete[] arrayPtr;
		}
	}

	elapsed_seconds *= 1000;


	return elapsed_seconds;

}

void Listener::deleteAllBitmaps()
{
	if (this->originalBitmap) delete this->originalBitmap;
	if (this->grayBitmap) delete this->grayBitmap;
	if (this->CppBitmap) delete this->CppBitmap;

	if (this->bmpManager) delete bmpManager;

	this->originalBitmap = nullptr;
	this->grayBitmap = nullptr;
	this->CppBitmap = nullptr;

	this->bmpManager = nullptr;
}
