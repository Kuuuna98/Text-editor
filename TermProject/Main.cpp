#include "TextBook.h"
#include "TextEditor.h"

int main() {
	TextEditor* textEditor = new TextEditor("test.txt");
	textEditor->run();
	return 0;
}