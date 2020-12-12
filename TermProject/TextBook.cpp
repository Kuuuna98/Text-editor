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
	else throw std::string("Unable to open file");
	in.close();

	return text_temp;
}

void TextBook::writeFile(std::string text_file_name, std::vector<std::string> text_list) {
	std::string text_file = "";

	for (auto vec : text_list) {
		text_file += vec + " ";
	}
	std::ofstream out(text_file_name);

	if (out.is_open()) {
		out.write(text_file.c_str(), text_file.size());
	}
	else throw std::string("Unable to open file");
	out.close();
}