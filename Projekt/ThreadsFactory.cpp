#include "ThreadsFactory.h"

ThreadsFactory::ThreadsFactory()
{
	for (int i = 0; i < 64; i++)
	{
		Threads.push_back(std::thread());
	}
}

ThreadsFactory::~ThreadsFactory()
{
}
