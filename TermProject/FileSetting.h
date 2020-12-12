#pragma once
#include <iostream>
#include <string>

class FileSetting {
private:
	static std::string text_file_name;

public:
	FileSetting(std::string text_file_name);
	static std::string get_file_name();
	void set_file_name(std::string text_file_name);
};