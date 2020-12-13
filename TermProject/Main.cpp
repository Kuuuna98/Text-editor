#include "TextEditor.h"

int main() {
	try {
		TextEditor* textEditor = new TextEditor("test.txt");
		textEditor->run();
	}
	catch (std::string s) {
		std::cout << s << std::endl;
	}
	catch (...) {
		std::cout << "Error";
	}

	return 0;
}