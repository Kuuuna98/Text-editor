#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


int main() {
	
	std::string text_file_name = "test.txt";

	std::ifstream in(text_file_name);

	if (in.is_open())
	{
		std::string text_temp; //변수명 변경
		getline(in, text_temp);
		std::cout << text_temp << std::endl;
	}
	else std::cout << "Unable to open file";
	in.close();

	return 0;
}