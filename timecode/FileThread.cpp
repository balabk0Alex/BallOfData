#include "FileThread.hpp"

//constructor , assign value to variables and run threads
FileThread::FileThread(std::string InNameFile, std::string OutNameFile):CircularBufferPtr(nullptr),end(true),counterData(0),
counter_QueueCircular(0), record(false), endmain(false)
{
	current_file.open( InNameFile, std::ios::binary | std::ios::in);//open files
	out_file.open(OutNameFile, std::ios::binary | std::ios::out);

	if( current_file )//check files
	{
		raport_thread = std::thread(&FileThread::raport, this);
		actually_thread = std::thread(&FileThread::main_process, this);//start threads
		output_thread = std::thread(&FileThread::outputInFile, this);

		actually_thread.join();
		output_thread.join();
		raport_thread.join();
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

	period = (1.0 / (speed * MB_INT ) * GIGA);//NANOSECONDS!!!
	limit_data *= MB_INT; //number execute cycle!!!
	limit_data++;
}


//THREAD1
void FileThread::main_process()
{
	std::chrono::time_point starttime = std::chrono::high_resolution_clock::now();
	std::chrono::time_point endtime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> duration;

	get_inform();//input data

	int data;//??

	while( counter_QueueCircular != 256 && !current_file.eof() && (counterData < limit_data) )
	{                                                          //while file open and buffer not full
		CircularBufferPtr = new CircularBuffer<int>(MB_INT256);//
		if(CircularBufferPtr == nullptr)
		{
			std::cout << "MEMORY ERROR";
			break;
		}



		while( !CircularBufferPtr->isFilled() && (counterData < limit_data) )
		{
			starttime = std::chrono::high_resolution_clock::now();
			current_file.read((char*) &data, sizeof(int));
			if(current_file.eof())
							break;//???
			CircularBufferPtr->write(data);
			endtime = std::chrono::high_resolution_clock::now();
			duration = endtime - starttime;
			++counterData;
			if( duration.count() * GIGA < period)
				std::this_thread::sleep_for(std::chrono::nanoseconds(period) - duration);
		}

		Info_Main.size_info_buffer = CircularBufferPtr->getSizeFilled();
		Info_Main.CircularBufferEnd = CircularBufferPtr->getObjectCircular();

		QueueCircular.push(Info_Main);
		++counter_QueueCircular;
	}

	endmain = true;

}

//THREAD2
void FileThread::outputInFile()
{
	std::chrono::time_point starttime = std::chrono::high_resolution_clock::now();
	std::chrono::time_point endtime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> duration;

	int data;
	float sumtime = 0;

	while(end)
	{
		while( QueueCircular.size())
		{
			starttime = std::chrono::high_resolution_clock::now();
			while(QueueCircular.front().CircularBufferEnd->read(data))
			{
			out_file.write((char *)&data, sizeof(int));
			endtime = std::chrono::high_resolution_clock::now();
			duration = endtime - starttime;//время записи 1 байта
			sumtime += duration.count();//
			middle_speed =  (sumtime / (counterData * sizeof(int)));
			middle_speed *= MB;
			record = true;
			}
			//расчёт здесь
			QueueCircular.pop();//DELETE!!!!
			if(!QueueCircular.size() && endmain ) end = false;
		}

	}
	record = false;
}

void FileThread::raport()
{
	while(end)
	{
	while(record)
	{
		std::cout <<"RAPORT:\n"
				"Middle speed of recording(B/s):" << middle_speed<< std::endl;
		std::cout << "Full cells:" << counter_QueueCircular << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
	}
}
