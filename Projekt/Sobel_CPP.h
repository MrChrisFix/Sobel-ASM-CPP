#pragma once

#include <CppDLL.h>
#include <chrono>
#include "BMPManager.h"

ref class Sobel_CPP
{
private:


public:
	Sobel_CPP();
	~Sobel_CPP();

	std::chrono::duration<double> executeInCpp(int numerOfThreads, BMPManager* bitmap);


};

