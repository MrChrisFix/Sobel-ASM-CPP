#include "Sobel_CPP.h"

#include <thread>
#include <vector>

Sobel_CPP::Sobel_CPP()
{
}

Sobel_CPP::~Sobel_CPP()
{
}

std::chrono::duration<double> Sobel_CPP::executeInCpp(int numerOfThreads, BMPManager* bitmap)
{
	std::vector<std::thread> Threads;

	for (int i = 0; i < numerOfThreads; i++)
		Threads.push_back(std::thread());


	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < numerOfThreads; i++)
	{
		//Threads[i] = std::thread(Sobel, bitmap->getPixelArray2D(), bitmap->getHeight(), bitmap->getWidth());
	}

	//Sobel()
	for (int i = 0; i < numerOfThreads; i++)
		Threads[i].join();

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	return elapsed_seconds;
}
