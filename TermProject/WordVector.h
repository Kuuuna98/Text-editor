#pragma once

#include <iostream>
#include <vector>
#include <string>

class WordVector {

private:
	std::vector<std::string> text;
	std::vector<int> all_line_idx;
	static WordVector* s_instance;
	WordVector();
public:
	static WordVector* instance();

	std::vector<std::string>& getTextList();
	std::vector<int>& getAllLineIdx();

	void splitWord(std::string text_temp);
	void insertWord(int line_idx_size, int line_and_idx, std::string word);
	void eraseWord(int line_and_idx);
	void changeWord(std::string beforeWord, std::string afterWord);
	int searchWord(std::string word);

};
