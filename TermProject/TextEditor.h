#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "TextBook.h"


class TextEditor {

private:
	TextBook* book;
	std::vector<int> all_line_idx;
	std::vector<int> page_idx;
	std::vector<int> line_idx;
	int now_page_idx = 0;
	int now_page_line_idx = 0;
	std::string text_file_name;

public:
	TextEditor(std::string text_file_name);
	void run();

private:
	int call(std::string consol_msg);


	int set_page_and_line(std::vector<std::string> text, int now);
	int search_set_page_and_line(std::vector<std::string> text, int now);

	void line_idx_init(std::vector<std::string> text, int st_idx);
	void page_idx_init(std::vector<std::string> text);
	std::vector<int>  search_prev_line_idx(std::vector<std::string> text, int idx);
	std::vector<int> line_idx_search(std::vector<std::string> text, int st_idx, int target_idx);

	std::vector<int> check_line_idx(std::vector<std::string> text, int now);
	std::vector<int> check_prevline_idx(std::vector<std::string> text, int next);
	std::vector<int> check_nextline_idx(std::vector<std::string> text, int now);

	void print_text(std::vector<std::string> text);
	std::string getInvalidInputMSG(bool follow, char keyword);
	std::vector<std::string> keyword_check(std::string answer);
	void insertWord(int line, int idx, std::string word);
	void eraseWord(int line, int idx);
	void changeWord(std::string beforeWord, std::string afterWord);
	int searchWord(std::string word);
	std::string menu(std::string consol);
};
