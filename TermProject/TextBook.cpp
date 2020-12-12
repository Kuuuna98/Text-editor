#include <sstream>
#include <fstream>
#include "TextBook.h"


std::string TextBook::loadFile(std::string text_file_name) {

	std::ifstream in(text_file_name);
	std::string text_temp;

	if (in.is_open())
	{
		getline(in, text_temp);

	}
	else std::cout << "Unable to open file";
	in.close();

	return text_temp;
}
