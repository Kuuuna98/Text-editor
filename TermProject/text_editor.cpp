#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class TextBook {
private:
	std::string stringBuffer;
	std::string text_file_name;
public:
	std::vector<std::string> text;

public:

	TextBook(std::string text_file_name) {
		this->text_file_name = text_file_name;
		readFile();
	}
private:
	void readFile() {
		std::ifstream in(text_file_name);

		text.clear();
		if (in.is_open())
		{
			std::string text_temp; //변수명 변경
			getline(in, text_temp);
			std::istringstream ss(text_temp);

			while (getline(ss, stringBuffer, ' ')) {
				text.push_back(stringBuffer);
			}
		}
		else std::cout << "Unable to open file";
		in.close();
	}

};



class TextEditor {

private:
	TextBook* textbook = nullptr;
	std::vector<int> page_idx;
	std::vector<int> line_idx;
	int now_page_idx;

public:
	TextEditor(TextBook* textbook) :now_page_idx(0) {
		this->textbook = textbook;
		page_idx.push_back(0);
		page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, now_page_idx));
	}

	void run() {
		int execute = 1;
		while (execute) {
			execute = call();
		}
	}

	int call(std::string consol = "") {
		std::string answer = menu(consol);
		int idx = 0;

		switch (answer.front()) {
		case 'n':
			if (page_idx[now_page_idx + 1] >= textbook->text.size()) {  //RE
				call("This is the last page!");
			}
			else {
				now_page_idx++;
				if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
				page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
			}
			break;

		case 'p':
			if (page_idx[now_page_idx] == 0) {
				call("This is the first page!");
			}
			else {
				now_page_idx--;
				print_text(textbook->text, page_idx[now_page_idx]);
			}
			break;
		case 'i':
			insertWord(1, 10, "hello");
			if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
			page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
			break;
		case 'd':
			eraseWord(2, 10);
			if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
			page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
			break;
		case 'c':
			changeWord("hello", "bye");
			now_page_idx = 0;
			if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
			page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
			break;
		case 's':
			idx = searchWord("hello");
			if (idx >= textbook->text.size()) {
				call("No words are found in the text.");
			}
			else {
				now_page_idx = 1;
				page_idx.clear();
				page_idx.push_back(-1);
				page_idx.push_back(idx);
				page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
			}
			break;
		case 't':
			fileWrite();
			return 0;
		default:
			break;
		}
		return 1;
	}

	//삭제 예정
/*
int print_text1(std::vector<std::string> text, int now) {
	std::vector<int> line_idx_temp;
	int page_total = 1;
	int line_char_total = 0;
	int line_num = 0;


	//line_idx_temp.push_back(now);
	//line_char_total = text[now].size();

	//std::cout << ' ' << line_num++ << "| " << text[i++];

	for (int i = now; i < text.size() && line_num <= 20; i++) {

		if (line_char_total==0 || line_char_total + 1 + text[i].size() > 75) { //next line

			if (++line_num > 20) {
				break;
			}
			line_idx_temp.push_back(i);
			if (line_num < 10) std::cout << std::endl << ' ' << line_num << "|";
			else std::cout << std::endl << line_num << "|";
			line_char_total = text[i].size();
		}
		else {
			line_char_total += 1 + text[i].size();
		}
		std::cout << ' ' << text[i];
	}

	return now;
}
*/

	int print_text(std::vector<std::string> text, int now) {

		int line_num = 0;

		if (now == -1) {
			line_idx = check_prevline_idx(text, page_idx[now_page_idx - 1]);
			if (line_idx[0] == 0) {
				page_idx.erase(page_idx.begin() + now_page_idx);
				now_page_idx = 0;
				now = line_idx[0];
			}
			else {
				page_idx.insert(page_idx.begin() + now_page_idx + 1, line_idx[0]);
				now_page_idx++;
				now = line_idx[0];
			}
		}
		else {
			line_idx = check_nextline_idx(text, now);
		}

		for (int i = line_idx[0]; i < text.size() && line_num <= 20; i++) {

			if (i == line_idx[line_num]) { //next line
				if (++line_num > 20) break;

				if (line_num < 10) std::cout << std::endl << ' ' << line_num << "|";
				else std::cout << std::endl << line_num << "|";
			}
			std::cout << ' ' << text[i];
		}

		return line_idx[20];
	}

	std::vector<int> check_prevline_idx(std::vector<std::string> text, int now) {
		std::vector<int> line_idx_temp;
		int page_total = 1;
		int line_char_total = 0;
		int i = now;

		for (; i >= 0 && line_idx_temp.size() <= 20; i--) {

			if (line_char_total + 1 + text[i].size() > 75) { //prev line

				if (line_idx_temp.size() == 20) {
					break;
				}

				line_char_total = text[i].size();
				line_idx_temp.insert(line_idx_temp.begin(), i);
			}
			else {
				line_char_total += 1 + text[i].size();
			}

		}

		if (line_idx_temp.size() < 20) {
			return check_nextline_idx(text, 0);
		}
		line_idx_temp.push_back(0);

		return line_idx_temp;
	}

	std::vector<int> check_nextline_idx(std::vector<std::string> text, int now) {
		std::vector<int> line_idx_temp;
		int page_total = 1;
		int line_char_total = 0;
		int i = now;

		for (; i < text.size() && line_idx_temp.size() <= 20; i++) {

			if (line_char_total == 0 || line_char_total + 1 + text[i].size() > 75) { //next line

				if (line_idx_temp.size() == 20) {
					break;
				}

				line_char_total = text[i].size();
				line_idx_temp.push_back(i);
			}
			else {
				line_char_total += 1 + text[i].size();
			}

		}

		if (line_idx_temp.size() < 20) {

			int size = line_idx_temp.size();
			for (int i = 0; i < 20 - size; i++) {
				line_idx_temp.insert(line_idx_temp.begin(), line_idx[19 - i]);
			}
		}
		line_idx_temp.push_back(i);

		return line_idx_temp;
	}

private:

	void insertWord(int line, int idx, std::string word) {
		textbook->text.insert(textbook->text.begin() + line_idx[line - 1] + (idx - 1), word);
	}

	void eraseWord(int line, int idx) {
		textbook->text.erase(textbook->text.begin() + line_idx[line - 1] + idx - 1);
	}

	void changeWord(std::string beforeWord, std::string afterWord) {
		//페이지 원래대로
		for (int i = 0; i < textbook->text.size(); i++) {
			if (textbook->text[i]._Equal(beforeWord)) {
				textbook->text.erase(textbook->text.begin() + i);
				textbook->text.insert(textbook->text.begin() + i, afterWord);
			}
		}
	}

	int searchWord(std::string word) {
		//페이지 원래대로
		int i = 0;
		for (; i < textbook->text.size(); i++) {
			if (textbook->text[i]._Equal(word)) {
				break;
			}
		}
		return i;
	}

	void fileWrite() { //구현
		//std::ofstream writeFile;            //쓸 목적의 파일 선언
		//writeFile.open("test2.txt");    //파일 열기
		//char arr[11] = "BlockDMask";        //파일에 쓸 문자열



		//if (writeFile.is_open())    //파일이 열렸는지 확인

		//{

		//	writeFile.write(arr, 10);    //파일에 문자열 쓰기

		//}

		//writeFile.close();
	}

	std::string menu(std::string consol) {
		std::string answer;

		std::cout << std::endl << "----------------------------------------------------------------------------------" << std::endl;
		std::cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << std::endl;
		std::cout << "----------------------------------------------------------------------------------" << std::endl;
		std::cout << "(콘솔 메세지)" << consol << std::endl;
		std::cout << "----------------------------------------------------------------------------------" << std::endl;
		std::cout << "입력: ";

		std::cin >> answer;
		std::cout << "----------------------------------------------------------------------------------";

		std::cin.get();

		return answer;
	}
};


int main() {

	TextBook* book = new TextBook("test.txt");
	TextEditor* textEditor = new TextEditor(book);

	textEditor->run();

	return 0;
}