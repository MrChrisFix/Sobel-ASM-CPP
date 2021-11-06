#include "Listener.h"
#include <msclr/marshal_cppstd.h>
#include "Sobel_CPP.h"
#include "Sobel_ASM.h"

Listener::Listener() : Cpp(), Asm()
{
	this->originalBitmap = nullptr;
	this->grayBitmap = nullptr;

	this->bmpManager = nullptr;
}

Listener::~Listener()
{
	if (this->originalBitmap) delete this->originalBitmap;
	if (this->grayBitmap) delete this->grayBitmap;

	if (this->bmpManager) delete bmpManager;
}

System::Drawing::Bitmap^ Listener::reactOnFileSelected(System::String^ fileName)
{
	this->originalBitmap = gcnew Bitmap(fileName);

	if (this->bmpManager) delete bmpManager;
	this->bmpManager = new BMPManager(msclr::interop::marshal_as<std::string>(fileName));

	this->grayBitmap = this->bmpManager->createBitmapFromGray();

	return this->originalBitmap;
}

std::chrono::duration<double> Listener::reactOnStartButton(short id, short threadNumber, System::Windows::Forms::PictureBox^ pictureBox)
{
	std::chrono::duration<double> elapsed_seconds;

	if (this->originalBitmap != nullptr)
	{
		if (id == 0) //Asembler
		{
			this->Asm.executeInASM(threadNumber, bmpManager);
			pictureBox->Image = this->grayBitmap; //this->AsmChangedBitmap;
		}
		else if (id == 1) //C++
		{
			BYTE* arrayPtr;

			elapsed_seconds = this->Cpp.executeInCpp(threadNumber, bmpManager, arrayPtr);
			pictureBox->Image = this->bmpManager->createBitmapFromGray(arrayPtr);
			
			delete[] arrayPtr;
		}
	}

	elapsed_seconds *= 1000;


	return elapsed_seconds;

}
