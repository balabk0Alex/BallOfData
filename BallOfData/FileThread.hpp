#pragma once
#include <iostream>
#include <queue>
#include <fstream>
#include <thread>

#include "CircularBuffer.hpp"
#define MB_INT 1048576/sizeof(int)
#define MB_INT256 256 * 1048576/sizeof(int)
#define GIGA 1000000000

//struct of buffer
struct CircularBufferInfo
{
	int size_info_buffer = 0;
	CircularBuffer<int> * CircularBufferEnd = nullptr;
};


class FileThread
{
public:
	FileThread(std::string InNameFile, std::string OutNameFile);

private:

	void main_process();
	void outputInFile();
	void get_inform();

	std::fstream current_file;
	std::fstream out_file;

	std::queue<CircularBufferInfo> QueueCircular;

	int period;
	int limit_data;
	int counter_QueueCircular;

	bool end;

	std::thread actually_thread;
	std::thread output_thread;

};

