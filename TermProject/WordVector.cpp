#include "WordVector.h"
#include "ErrorManagement.h"
#include <sstream>
#include <fstream>

WordVector::WordVector() {
	all_line_idx.clear();
}

std::vector<std::string>& WordVector::getTextList() {
	return text;
}
std::vector<int>& WordVector::getAllLineIdx() {
	return all_line_idx;
}

void WordVector::splitWord(std::string text_temp) {

	std::string string_buffer;
	text.clear();
	std::istringstream ss(text_temp);

	while (getline(ss, string_buffer, ' ')) {
		if (string_buffer.size() > 75) throw std::string(ERR_MSG_FILE_WORD_OVER);
		text.push_back(string_buffer);
	}
}

void WordVector::insertWord(int line_idx_size, int line_and_idx, std::string word) {
	if (line_idx_size == 0) WordVector::instance()->getTextList().insert(WordVector::instance()->getTextList().begin(), word);
	else WordVector::instance()->getTextList().insert(WordVector::instance()->getTextList().begin() + line_and_idx, word);
}

void WordVector::eraseWord(int line_and_idx) {
	WordVector::instance()->getTextList().erase(WordVector::instance()->getTextList().begin() + line_and_idx);
}

void WordVector::changeWord(std::string beforeWord, std::string afterWord) {
	for (int i = 0; i < WordVector::instance()->getTextList().size(); i++) {
		if (WordVector::instance()->getTextList()[i]._Equal(beforeWord)) {
			WordVector::instance()->getTextList().erase(WordVector::instance()->getTextList().begin() + i);
			WordVector::instance()->getTextList().insert(WordVector::instance()->getTextList().begin() + i, afterWord);
		}
	}
}

int WordVector::searchWord(std::string word) {
	int i = 0;
	for (; i < WordVector::instance()->getTextList().size(); i++) {
		if (WordVector::instance()->getTextList()[i]._Equal(word)) {
			break;
		}
	}
	return i;
}

WordVector* WordVector::instance() {
	if (!s_instance) {
		s_instance = new WordVector();
	}
	return s_instance;
}
WordVector* WordVector::s_instance = 0;