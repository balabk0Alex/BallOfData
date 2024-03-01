#include <thread>
#include <chrono>
#include "FileThread.hpp"



int main()
{
	std::string output_file;

	std::cout << "Please enter the file to be read" << std::endl;
	std::cin >> output_file;std::cout << std::endl;

	FileThread file1(output_file,"OutputFile");
	std::cout << "complete!!!";

	return 0;
}
