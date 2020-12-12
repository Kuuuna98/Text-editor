#include "FileSetting.h"

FileSetting::FileSetting(std::string text_file_name = "") {
	text_file_name = text_file_name;
}
std::string FileSetting::get_file_name() {
	return text_file_name;
}
void FileSetting::set_file_name(std::string text_file_name = "") {
	this->text_file_name = text_file_name;
}std::string FileSetting::text_file_name = "";
