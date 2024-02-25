#include <iostream>
#include <queue>
#include <fstream>
#include "CircularBuffer.hpp"

#define MB_INT 1048576/sizeof(int)
#define MB_INT256 256 * 1048576/sizeof(int)

struct CircularBufferInfo
{
	int size_info_buffer = 0;
	CircularBuffer<int> * CircularBufferEnd = nullptr;

}Info_Main;

int main()
{
	std::queue<CircularBufferInfo> QueueCircular;
	int couter_QueueCircular = 0;
	CircularBuffer<int> *CircularBufferPtr = nullptr;

	int data;
	int sizeCircular = 0;

	////////////////////////////////////
	std::fstream current_file;
	current_file.open("300_MB", std::ios::binary | std::ios::in);

	while( couter_QueueCircular != 256 && !current_file.eof())///!!!
	{
		//флаг опущен
		CircularBufferPtr = new CircularBuffer<int>(MB_INT256);//
		while( !CircularBufferPtr->isFilled())
		{
			current_file.read((char*) &data, sizeof(int));
			if(current_file.eof())
							break;
			CircularBufferPtr->write(data);
		}

		Info_Main.size_info_buffer = CircularBufferPtr->getSizeFilled();
		Info_Main.CircularBufferEnd = CircularBufferPtr->getObjectCircular();
		QueueCircular.push(Info_Main);
		//флаг поднят
		++couter_QueueCircular;

	}

}
