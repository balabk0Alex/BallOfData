#include <thread>
#include <chrono>
#include "FileThread.hpp"



int main()
{

	std::thread thread1, thread2;


	std::chrono::time_point start = std::chrono::high_resolution_clock::now();

	FileThread file1("300_MB","OutputFile",thread1,thread2);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;



	std::cout << "TIME: " << duration.count() << std::endl;
	std::cout << "complete!!!";
}
