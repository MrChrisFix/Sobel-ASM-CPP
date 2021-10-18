#include "CppDLL.h"
#include "pch.h"

extern "C" __declspec(dllexport) void tangensMilion()
{
	double a = 0;
	for (int i = 0; i < 1000000; i++)
		a += i;
	return;
}
