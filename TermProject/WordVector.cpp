#include "WordVector.h"
#include "ErrorManagement.h"
#include <sstream>
#include <fstream>

std::vector<std::string>& WordVector::get_text_list() {
	return text;
}

void WordVector::splitWord(std::string text_temp) {

	text.clear();
	std::istringstream ss(text_temp);

	while (getline(ss, stringBuffer, ' ')) {
		if (stringBuffer.size() > 75) throw std::string(ERR_MSG_FILE_WORD_OVER);
		text.push_back(stringBuffer);
	}
}
WordVector* WordVector::instance() {
	if (!s_instance) {
		s_instance = new WordVector;
	}
	return s_instance;
}
WordVector* WordVector::s_instance = 0;