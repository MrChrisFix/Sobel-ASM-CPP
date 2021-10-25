#pragma once
#include <chrono>
#include "BMPManager.h"

typedef int (*FunkcjaASM_t)(); //drugi nawias na argumenty


ref class Sobel_ASM
{
private:
	void loadAsmDLL();
	
	FunkcjaASM_t wykonajASM;

	bool loaded_library;

public:
	Sobel_ASM();
	~Sobel_ASM();

	std::chrono::duration<double> executeInASM(int numerOfThreads, BMPManager* bitmap);

};

