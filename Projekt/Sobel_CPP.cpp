#include "Sobel_CPP.h"

#include <thread>
#include <vector>

Sobel_CPP::Sobel_CPP()
{
}

Sobel_CPP::~Sobel_CPP()
{
}

std::chrono::duration<double> Sobel_CPP::executeInCpp(int numerOfThreads, BMPManager* bitmap, BYTE*& ptr)
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
	
	std::vector<std::thread> Threads;

	int* calcArray = new int[arraySize];

	int arrayStartOffset = 0;
	auto start = std::chrono::steady_clock::now();

	//Fisrt use of threads: creating the number array
	for (int i = 0; i < numerOfThreads; i++)
	{
		Threads.push_back(std::thread(Sobel, bitmap->getGrayArray(), bitmap->getHeight(), bitmap->getWidth(), length[i], arrayStartOffset, calcArray));
		arrayStartOffset += length[i];
	}


	for (int i = 0; i < numerOfThreads; i++)
		while(true)
			if (Threads[i].joinable())
			{
				Threads[i].join();
				break;
			}

	int minimum, maximum;
	minimum = maximum = calcArray[0];
	for (int i = 0; i < arraySize; i++)
	{
		if (calcArray[i] < minimum) minimum = calcArray[i];
		if (calcArray[i] > maximum) maximum = calcArray[i];
	}

	//Second use of threads: Normalizing the array to [0-255]
	BYTE*normalized = new BYTE[arraySize];
	arrayStartOffset = 0;
	for (int i = 0; i < numerOfThreads; i++)
	{
		Threads[i] = std::thread(Normalize, calcArray, minimum, maximum, length[i], arrayStartOffset, std::ref(normalized));
		arrayStartOffset += length[i];
	}

	for (int i = 0; i < numerOfThreads; i++)
		if (Threads[i].joinable())
		{
			Threads[i].join();
		}

	auto end = std::chrono::steady_clock::now();

	ptr = normalized;

	std::chrono::duration<double> elapsed_seconds = end - start;

	if(length != nullptr) delete[] length;
	if (calcArray != nullptr) delete[] calcArray;
	Threads.clear();

	return elapsed_seconds;
}

