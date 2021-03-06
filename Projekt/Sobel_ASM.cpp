#include "Sobel_ASM.h"
#include <thread>
#include <vector>
#include <windows.h>

void Sobel_ASM::loadAsmDLL()
{
	HINSTANCE asmDLL = LoadLibrary(L"AsemblerDLL.dll");

	if (asmDLL)
	{
		this->doASMSobel = (ASMSobelFunction_t)GetProcAddress(asmDLL, "Sobel");// in the string is the asm proc name
		this->doASMNormalization = (ASMNormalizeFunction_t)GetProcAddress(asmDLL, "Normalize");

		if (doASMSobel!= nullptr && doASMNormalization != nullptr)
		{
			loaded_library = true;
		}
	}
}


Sobel_ASM::Sobel_ASM()
{
	loaded_library = false;
	loadAsmDLL();
}

Sobel_ASM::~Sobel_ASM()
{

}

std::chrono::duration<double> Sobel_ASM::executeInASM(int numerOfThreads, BMPManager* bitmap, unsigned char*& ptr, System::Windows::Forms::ProgressBar^ progress)
{
	progress->Maximum = numerOfThreads * 2;

	//Slicing the array into parts for multithreading
	int* length = new int[numerOfThreads];
	int arraySize = bitmap->getHeight() * bitmap->getWidth();
	int rest = arraySize % numerOfThreads;

	for (int i = 0; i < numerOfThreads; i++)
	{
		length[i] = (arraySize - rest) / numerOfThreads;
	}
	for (int i = 0; i < rest; i++)
	{
		length[i]++;
	}

	std::vector<std::thread> Threads;

	int* calculated = new int[arraySize];
	BYTE* normalized = new BYTE[arraySize];
	for (int i = 0; i < arraySize; i++) calculated[i] = 0;
	int arrayStartOffset = 0;

	auto start = std::chrono::steady_clock::now();
	if (loaded_library)
	{
		//First use of threads
		for (int i = 0; i < numerOfThreads; i++)
		{
			Threads.push_back(std::thread(*doASMSobel, bitmap->getGrayArray(), calculated, bitmap->getHeight(), bitmap->getWidth(), length[i], arrayStartOffset));
			arrayStartOffset += length[i];
		}

		for (int i = 0; i < numerOfThreads; i++)
			if (Threads[i].joinable())
			{
				Threads[i].join();
				progress->Value += 1;
			}

		//Find minimum and maximum for normalization 
		int minimum, maximum;
		minimum = maximum = calculated[0];
		for (int i = 0; i < arraySize; i++)
		{
			if (calculated[i] < minimum) minimum = calculated[i];
			if (calculated[i] > maximum) maximum = calculated[i];
		}

		//Second use of threads
		arrayStartOffset = 0;
		for (int i = 0; i < numerOfThreads; i++)
		{
			Threads[i] = std::thread(*doASMNormalization, calculated, normalized, minimum, maximum, length[i], arrayStartOffset);
			arrayStartOffset += length[i];
		}

		for (int i = 0; i < numerOfThreads; i++)
			if (Threads[i].joinable())
			{
				Threads[i].join();
				progress->Value += 1;
			}

		ptr = normalized;
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	if (length != nullptr) delete[] length;
	if (calculated != nullptr) delete[] calculated;
	Threads.clear();

	return elapsed_seconds;
}
