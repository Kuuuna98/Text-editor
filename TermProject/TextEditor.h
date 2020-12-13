#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TextBook.h"


class TextEditor {

private:
	TextBook* book;
	std::vector<int> line_idx;
	int now_page_line_idx = 0;
	std::string text_file_name;

public:
	TextEditor(std::string text_file_name);
	void run();

private:
	int call(std::string consol_msg);
	
	void lineIdxInit(std::vector<std::string> text, int st_idx);
	std::vector<int>  searchPrevLineIdx(std::vector<std::string> text, int idx);

	void printText(std::vector<std::string> text);
	std::string menu(std::string consol);

	std::string getInvalidInputMSG(bool follow, char keyword);
	std::vector<std::string> keyword_check(std::string answer);

	void insertEdit(std::vector<std::string> answer_split);
	void eraseEdit(std::vector<std::string> answer_split);
	void changeEdit(std::vector<std::string> answer_split);
	void searchEdit(std::vector<std::string> answer_split);
	void nextPage(std::vector<std::string> answer_split);
	void prevPage(std::vector<std::string> answer_split);

};

