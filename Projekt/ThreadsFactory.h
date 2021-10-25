#pragma once
#include <vector>
#include <thread>


class ThreadsFactory
{
	std::vector<std::thread> Threads;


public:
	ThreadsFactory();
	~ThreadsFactory();

	void executeFuntion();

};

