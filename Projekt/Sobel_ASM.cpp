#include "Sobel_ASM.h"
#include <thread>
#include <vector>
#include <windows.h>

void Sobel_ASM::loadAsmDLL()
{
	HINSTANCE asmDLL = LoadLibrary(L"AsemblerDLL.dll");

	if (asmDLL)
	{
		this->wykonajASM = (FunkcjaASM_t)GetProcAddress(asmDLL, "Sobel");// w string jest nazwa funkcji
		if (wykonajASM!= nullptr)
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

std::chrono::duration<double> Sobel_ASM::executeInASM(int numerOfThreads, BMPManager* bitmap, BYTE*& ptr)
{
	//Slicing the array into parts for multithreading
	int* length = new int[numerOfThreads];
	int arraySize = bitmap->getHeight() * bitmap->getWidth();
	int rest = arraySize % numerOfThreads;
	int temp = rest;

	for (int i = 0; i < numerOfThreads; i++)
	{
		length[i] = (arraySize - rest) / numerOfThreads;
	}
	for (int i = 0; i < rest; i++)
	{
		length[i]++;
	}


	//std::vector<std::thread> Threads;

	

	int* calculated = new int[arraySize];
	int* helper = new int[arraySize];
	auto start = std::chrono::steady_clock::now();
	if (loaded_library)
	{
		for (int i = 0; i < numerOfThreads; i++)
		{
			//Threads.push_back(std::thread(wykonajASM, bitmap->getGrayArray(), calculated, helper, bitmap->getHeight(), bitmap->getWidth(), length[0], 0));
		}

		//
		//for (int i = 0; i < numerOfThreads; i++)
		//	Threads[i].join();

		wykonajASM(bitmap->getGrayArray(), calculated, helper, bitmap->getHeight(), bitmap->getWidth(), length[0], 0);
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	return elapsed_seconds;
}
