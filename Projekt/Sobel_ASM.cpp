#include "Sobel_ASM.h"
#include <thread>
#include <vector>
#include <windows.h>

void Sobel_ASM::loadAsmDLL()
{
	HINSTANCE asmDLL = LoadLibrary(L"AsemblerDLL.dll");

	if (asmDLL)
	{
		this->wykonajASM = (FunkcjaASM_t)GetProcAddress(asmDLL, "FunkcjaASM");// w string jest nazwa funkcji
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

std::chrono::duration<double> Sobel_ASM::executeInASM(int numerOfThreads, BMPManager* bitmap)
{
	std::vector<std::thread> Threads;

	for (int i = 0; i < numerOfThreads; i++)
		Threads.push_back(std::thread());


	auto start = std::chrono::steady_clock::now();
	if (loaded_library)
	{
		for (int i = 0; i < numerOfThreads; i++)
		{
			Threads[i] = std::thread(*wykonajASM);
		}

		//Sobel()
		for (int i = 0; i < numerOfThreads; i++)
			Threads[i].join();
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	return elapsed_seconds;
}
