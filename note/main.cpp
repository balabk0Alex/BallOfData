#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <queue>

using namespace std::literals::chrono_literals;


int main()
{
	int speed;
	double period;
	char c;

	////////////////////////////
	std::chrono::duration delta =std::chrono::duration<double>();

	std::fstream current_file;

	current_file.open("MAIN_BINARY", std::ios::binary | std::ios::in);
	if(!current_file)
		exit(1);

	std::queue<char> data_file;

	std::cout << "Please specify the speed (bit/s)" << std::endl;
	std::cin >> speed;

	period = 1.0 / speed;

	delta =std::chrono::duration<double>(period);

	while( current_file.read((char*) &c, sizeof(char)).good())//запиши в переменную c 1 байт
	{
		auto start = std::chrono::high_resolution_clock::now();
		data_file.push(c);
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << data_file.size() << "  " << (end-start).count() << std::endl;
		std::this_thread::sleep_for(delta - ( end - start));
	}

	//


}
