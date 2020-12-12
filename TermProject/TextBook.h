#pragma once

#include <iostream>
#include <string>
#include <vector>

class TextBook {

public:
	std::string loadFile(std::string text_file_name);
	void writeFile(std::string text_file_name, std::vector<std::string> text_list);
};
