#pragma once
#include <iostream>

template< typename T >
class CircularBuffer
{
	class Frame
		{
		public:
			T data;
			Frame *nextFrame;
		};
public:
	CircularBuffer();
	CircularBuffer(int size_);
	~CircularBuffer();
	bool write( T newdata);
	bool read(T & outdata);
	CircularBuffer* getObjectCircular(){return this;}
	int getSizeFilled(){ return couterWr;}
	bool isFilled(){return couterWr == size;}
private:
	Frame * readPtr;
	Frame * writePtr;

	int couterRe;
	int couterWr;

	int size;

};


template<typename T>
CircularBuffer<T>::CircularBuffer(int size_):couterWr(0),couterRe(0), size(size_)
{
	Frame *timeFramePtr = new Frame;//I create the header element//
	if(timeFramePtr == nullptr)
	{
		std::cout << "EROR MEMORY";
	}
	Frame *framePtr = nullptr;

	readPtr = writePtr = timeFramePtr;

	for(int i = 1; i < size; ++i)
	{
		framePtr = new Frame;
		if(framePtr == nullptr)
		{
			std::cout << "EROR MEMORY";
			exit(1);
		}
		timeFramePtr->nextFrame = framePtr;
		timeFramePtr = framePtr;
	}
	framePtr->nextFrame = writePtr;//last element point to the first element//
}

template< typename T >
CircularBuffer<T>::~CircularBuffer()
{
	Frame *timeFramePtr = nullptr;
	int timesize = size - 1;

	for(int i = 0; i < timesize; ++i)
	{
		timeFramePtr = readPtr;
		readPtr = readPtr->nextFrame;
		delete readPtr;
	}
	delete readPtr;

}

template< typename T>
bool CircularBuffer<T>::write( T newdata)
{
	if(couterWr == size)
		return 0;
	writePtr->data = newdata;
	writePtr = writePtr->nextFrame;
	return ++couterWr;
}

template< typename T>
bool CircularBuffer<T>::read(T & outdata)
{
	if (couterRe > couterWr)
		return 0;

	outdata = readPtr->data;
	readPtr = readPtr->nextFrame;

	return ++couterRe;

}



