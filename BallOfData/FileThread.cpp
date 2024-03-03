#include "FileThread.hpp"

//constructor , assign value to variables and run threads
FileThread::FileThread(std::string InNameFile, std::string OutNameFile):end(true),counter_QueueCircular(0)
{
	current_file.open( InNameFile, std::ios::binary | std::ios::in);//open files
	out_file.open(OutNameFile, std::ios::binary | std::ios::out);

	if( current_file )//check files
	{
		actually_thread = std::thread(&FileThread::main_process, this);//start threads
		output_thread = std::thread(&FileThread::outputInFile, this);

		actually_thread.join();
		output_thread.join();
	}
	else
	{
		std::cout << "FILE DO NOT OPEN ";
	}
}

//function require data from user
//also data convert to other type
void FileThread::get_inform()
{
	int speed;//time val

	std::cout << "Please, specify the recoding speed ( MB/s): "; std::cin >> speed;///!!!
	std::cout << std::endl << "Please, specify the size of recording data (MB) :"; std::cin >> limit_data;///!

	period = (1.0 / (speed * MB_INT )) * GIGA;
	limit_data *= MB_INT; //number execute cycle!!!
}


//THREAD1
void FileThread::main_process()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> starttime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endtime;

	std::chrono::duration<float> duration;//ОПТИМИЗИРОВАТЬ
	CircularBufferInfo Info_Main;
	CircularBuffer<int> *CircularBufferPtr = nullptr;
	int counterCircle = 0;


	get_inform();//input data


	int data;//??

	while( counter_QueueCircular != 256 && !current_file.eof() && counterCircle < limit_data)
	{                                                          //while file open and buffer not full
		CircularBufferPtr = new CircularBuffer<int>(MB_INT256);//
		if(CircularBufferPtr == nullptr)
		{
			std::cout << "MEMORY ERROR";
			break;
		}



		while( !CircularBufferPtr->isFilled() && counterCircle < limit_data)
			{
			    starttime = std::chrono::high_resolution_clock::now();
				current_file.read((char*) &data, sizeof(int));
				if(current_file.eof())
								break;//???
				CircularBufferPtr->write(data);
				endtime = std::chrono::high_resolution_clock::now();
				duration = endtime - starttime;
				if( duration.count() * GIGA < period)
				std::this_thread::sleep_for(std::chrono::nanoseconds( (int)duration.count() * GIGA - period ));
				counterCircle++;
			}

		Info_Main.size_info_buffer = CircularBufferPtr->getSizeFilled();
		Info_Main.CircularBufferEnd = CircularBufferPtr->getObjectCircular();

		QueueCircular.push(Info_Main);
		++counter_QueueCircular;
	}
	end = false;
}

//THREAD2
void FileThread::outputInFile()
{
	int data;

	std::chrono::time_point<std::chrono::high_resolution_clock> starttime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endtime;
	std::chrono::duration<float> duration;

	double speed;

	while(end || QueueCircular.size())
	{
		while( QueueCircular.size())
		{
			starttime = std::chrono::high_resolution_clock::now();

			while(QueueCircular.front().CircularBufferEnd->read(data))
			out_file.write((char *)&data, sizeof(int));

			endtime = std::chrono::high_resolution_clock::now();
			duration = endtime - starttime;
			speed = (duration.count() * GIGA) / (QueueCircular.front().CircularBufferEnd->getSizeFilled() * sizeof(int));

			QueueCircular.pop();//DELETE!!!!
		}

		std::cout << "filled cells: " << counter_QueueCircular << std::endl;

	}
}
