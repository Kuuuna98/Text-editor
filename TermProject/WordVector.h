#pragma once

#include <iostream>
#include <vector>
#include <string>

class WordVector {

private:
	std::string stringBuffer;
	std::vector<std::string> text;
	static WordVector* s_instance;
	WordVector();
public:
	void splitWord(std::string text_temp);
	std::vector<std::string>& getTextList();
	static WordVector* instance();

};
