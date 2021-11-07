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
	std::vector<BYTE*> calculatedArrays;


	for (int i = 0; i < numerOfThreads; i++)
		calculatedArrays.push_back(nullptr);


	int arrayStartOffset = 0;
	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < numerOfThreads; i++)
	{
		Threads.push_back(std::thread(Sobel, bitmap->getGrayArray(), bitmap->getHeight(), bitmap->getWidth(), length[i], arrayStartOffset, std::ref(calculatedArrays[i])));
		arrayStartOffset += length[i];
	}

	//ptr = Sobel(bitmap->getGrayArray(), bitmap->getHeight(), bitmap->getWidth(), arraySize, 0, std::ref(calculatedArrays[0])); //Temporary


	for (int i = 0; i < numerOfThreads; i++)
		while(true)
			if (Threads[i].joinable())
			{
				Threads[i].join();
				break;
			}

	auto end = std::chrono::steady_clock::now();


	//adding all arrays together into one fianl array
	BYTE* completeResult = new BYTE[arraySize];
	int currentByte = 0;
	for (int i = 0; i < numerOfThreads; i++)
	{
		for (int j = 0; j < length[i]; j++)
		{
			completeResult[currentByte] = calculatedArrays[i][currentByte];
			currentByte++;
		}
	}
	ptr = completeResult;

	std::chrono::duration<double> elapsed_seconds = end - start;

	return elapsed_seconds;
}
