#pragma once
#include <chrono>
#include "BMPManager.h"

//Arguments: BYTE* gray, int* calculated, int* helper, int imageHeignt, int imageWidth, int bytes to calculate, int start
typedef int (*ASMSobelFunction_t)(unsigned char* gray, int* calculated, int height, __int32 width, __int32 bytes, __int32 start);

typedef int (*ASMNormalizeFunction_t)(int* calculated, unsigned char*normalized, __int32 minimum, __int32 maximum, __int32 bytes, __int32 start);


ref class Sobel_ASM
{
private:
	void loadAsmDLL();
	
	ASMSobelFunction_t doASMSobel;
	ASMNormalizeFunction_t doASMNormalization;

	bool loaded_library;

public:
	Sobel_ASM();
	~Sobel_ASM();

	std::chrono::duration<double> executeInASM(int numerOfThreads, BMPManager* bitmap, unsigned char*& ptr, System::Windows::Forms::ProgressBar^ progress);

};

