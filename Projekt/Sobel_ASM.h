#pragma once
#include <chrono>
#include "BMPManager.h"

//Arguments: BYTE* gray, int* calculated, int* helper, int imageHeignt, int imageWidth, int bytes to calculate, int start
typedef int (*FunkcjaASM_t)(unsigned char* gray, int* calculated, int* help ,int height, __int32 width, __int32 bytes, __int32 start); //drugi nawias na argumenty


ref class Sobel_ASM
{
private:
	void loadAsmDLL();
	
	FunkcjaASM_t wykonajASM;

	bool loaded_library;

public:
	Sobel_ASM();
	~Sobel_ASM();

	std::chrono::duration<double> executeInASM(int numerOfThreads, BMPManager* bitmap, unsigned char*& ptr);

};

