#include "Listener.h"
#include <msclr/marshal_cppstd.h>
#include "Sobel_CPP.h"
#include "Sobel_ASM.h"

Listener::Listener() : Cpp(), Asm()
{
	this->originalBitmap = nullptr;
	this->grayBitmap = nullptr;
	this->CppChangedBitmap = nullptr;
	this->AsmChangedBitmap = nullptr;

	this->bmpManager = nullptr;
}

Listener::~Listener()
{
	if (this->originalBitmap) delete this->originalBitmap;
	if (this->grayBitmap) delete this->grayBitmap;
	if (this->CppChangedBitmap) delete this->CppChangedBitmap;
	if (this->AsmChangedBitmap) delete this->AsmChangedBitmap;

	if (this->bmpManager) delete bmpManager;
}

System::Drawing::Bitmap^ Listener::reactOnFileSelected(System::String^ fileName)
{
	this->originalBitmap = gcnew Bitmap(fileName);

	if (this->bmpManager) delete bmpManager;
	this->bmpManager = new BMPManager(msclr::interop::marshal_as<std::string>(fileName));

	this->grayBitmap = this->bmpManager->getBitmap();

	return this->originalBitmap;
}

std::chrono::duration<double> Listener::reactOnStartButton(short id, short threadNumber, System::Windows::Forms::PictureBox^ pictureBox)
{
	auto start = std::chrono::steady_clock::now(); //TODO:Delete
	if (this->originalBitmap != nullptr)
	{
		if (id == 0) //Asembler
		{
			this->Asm.executeInASM(threadNumber, bmpManager);
			pictureBox->Image = this->grayBitmap; //this->AsmChangedBitmap;
		}
		else if (id == 1) //C++
		{
			//auto Image = Sobel
			this->Cpp.executeInCpp(threadNumber, bmpManager);
			auto Image = this->bmpManager->createBitmapFrom2DArray();
			pictureBox->Image = Image;
			//pictureBox->Image = this->grayBitmap; //this->CppChangedBitmap;
		}
	}
	auto end = std::chrono::steady_clock::now(); //TODO:Delete

	std::chrono::duration<double> elapsed_seconds = end - start; //TODO:Delete

	elapsed_seconds *= 1000; //TODO:Delete


	return elapsed_seconds;

}
