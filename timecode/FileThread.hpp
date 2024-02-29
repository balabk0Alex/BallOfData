#pragma once
#include <iostream>
#include <chrono>
#include <queue>
#include <fstream>
#include <thread>

#include "CircularBuffer.hpp"
#define MB 1048576
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
	void raport();

	std::fstream current_file;
	std::fstream out_file;

	CircularBufferInfo Info_Main;
	std::queue<CircularBufferInfo> QueueCircular;
	CircularBuffer<int> *CircularBufferPtr;

	int counter_QueueCircular;
	int period;
	int limit_data;
	int counterData;
	double middle_speed;

	bool end;
	bool record;
	bool endmain;

	std::thread actually_thread;
	std::thread output_thread;
	std::thread raport_thread;

};

