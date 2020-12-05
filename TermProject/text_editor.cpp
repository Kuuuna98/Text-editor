#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


int main() {
	
	std::string stringBuffer;
	std::string text_file_name = "test.txt";
	std::vector<std::string> text;

	std::ifstream in(text_file_name);

	text.clear();
	if (in.is_open())
	{
		std::string text_temp; //변수명 변경
		getline(in, text_temp);
		std::istringstream ss(text_temp);

		while (getline(ss, stringBuffer, ' ')) {
			text.push_back(stringBuffer);
		}
	}
	else std::cout << "Unable to open file";
	in.close();

	return 0;
}