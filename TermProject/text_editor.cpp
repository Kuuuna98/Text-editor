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
		std::string consol_msg = "";
		while (execute) {
			try {
				execute = call(consol_msg);
				consol_msg = "";
			}
			catch (std::string s) {
				consol_msg = s;
			}
			catch (...) {
				consol_msg = "오잉";
			}
		}
	}

	int call(std::string consol_msg = "") {
		std::string answer = menu(consol_msg);
		if (answer.size() < 1) throw std::string("Invalid input");

		int idx = 0;
		std::vector<std::string> answer_split = keyword_check(answer);

		switch (answer.front())
		{
		case 'n':
			if (page_idx[now_page_idx + 1] >= textbook->text.size()) {  //RE
				throw std::string("This is the last page!");
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

		case 't':
			break;
		case 's':
			break;

		case 'd':
			break;

		case 'i':
			break;

		default:
			break;
		}
		return 1;
	}


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

		std::cout << std::endl << "----------------------------------------------------------------------------------";

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

	std::string getInvalidInputMSG(bool follow, char keyword) {
		std::string msg = "Invalid input: The '";
		msg += keyword;

		if (follow) msg += "' keyword can be followed by anything word.";
		else msg += "' keyword cannot be followed by anything word.";

		return msg;
	}

	std::vector<std::string> keyword_check(std::string answer) {

		char keyword(answer.front());
		std::string message = "";
		std::vector<std::string> answer_split;
		std::string word = "";
		bool front_parentheses = false, back_parentheses = false;

		switch (keyword)
		{
		case 'n':
		case 'p':
		case 't':

			if (answer.size() > 1) throw std::string(getInvalidInputMSG(false, keyword));

			answer_split.push_back("" + keyword);
			return answer_split;

		case 's':
			if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));

			for (int i = 1; i < answer.size(); i++) {
				if (answer.at(i) == ' ') {
					if (front_parentheses && !back_parentheses) throw std::string("Invalid input: There should be no spaces in parentheses.");
				}
				else if (answer.at(i) == '(') {
					if (front_parentheses) word += '(';
					front_parentheses = true;
				}
				else if (answer.at(i) == ')') {
					if (back_parentheses) word += ')';
					back_parentheses = true;
				}
				else {
					if (back_parentheses) word += ')';
					back_parentheses = false;
					word += answer.at(i);
				}
			}
			if (!back_parentheses) throw std::string("Invalid input");

			answer_split.push_back(word);
			return answer_split;

		case 'c':
			if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));


			for (int i = 1; i < answer.size(); i++) {
				if (answer.at(i) == ' ') {
					if (front_parentheses && !back_parentheses) throw std::string("Invalid input: There should be no spaces in parentheses.");
				}
				else if (answer.at(i) == ',') {
					answer_split.push_back(word);
					word = "";
					if (answer_split.size() > 1) throw std::string("Invalid input");
				}
				else {
					if (answer.at(i) == '(') {
						if (front_parentheses) word += '(';
						front_parentheses = true;
					}
					else if (answer.at(i) == ')') {
						if (back_parentheses) word += ')';
						back_parentheses = true;
					}
					else {
						if (back_parentheses) word += ')';
						back_parentheses = false;
						word += answer.at(i);
					}
				}
			}
			answer_split.push_back(word);
			if (!back_parentheses || answer_split.size() < 2) throw std::string("Invalid input");
			return answer_split;

		case 'd':
			if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));

			for (int i = 1; i < answer.size(); i++) {
				if (answer.at(i) == ' ') {
					if (front_parentheses && !back_parentheses) throw std::string("Invalid input: There should be no spaces in parentheses.");
				}
				else if (answer.at(i) == ',') {
					answer_split.push_back(word);
					word = "";
					if (answer_split.size() > 1) throw std::string("Invalid input");
				}
				else {
					if (answer.at(i) == '(') {
						if (front_parentheses) throw std::string("Invalid input");
						front_parentheses = true;
					}
					else if (answer.at(i) == ')') {
						if (back_parentheses) throw std::string("Invalid input");
						back_parentheses = true;
					}
					else {
						if (back_parentheses) throw std::string("Invalid input");
						if (answer.at(i) < 48 || answer.at(i) > 57) throw std::string("Invalid input: Keyword 'd' must only enter numbers for the argument.");
						word += answer.at(i);
					}
				}
			}
			answer_split.push_back(word);
			if (!back_parentheses || answer_split.size() < 2) throw std::string("Invalid input");
			return answer_split;

		case 'i':
			if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));

			for (int i = 1; i < answer.size(); i++) {
				if (answer.at(i) == ' ') {
					if (front_parentheses && !back_parentheses) throw std::string("Invalid input: There should be no spaces in parentheses.");
				}
				else if (answer.at(i) == ',') {
					//if(answer_split.size()==1 && word) //숫자 변환
					answer_split.push_back(word);
					word = "";
					if (answer_split.size() > 2) throw std::string("Invalid input");
				}
				else {
					if (answer.at(i) == '(') {
						if (front_parentheses) throw std::string("Invalid input");
						front_parentheses = true;
					}
					else if (answer.at(i) == ')') {
						if (back_parentheses) throw std::string("Invalid input");
						back_parentheses = true;
					}
					else {
						if (back_parentheses) throw std::string("Invalid input");
						//if (answer.at(i) > 47 && answer.at(i) < 58) throw std::string("Invalid input: Keyword 'd' must only enter numbers for the argument.");
						word += answer.at(i);
					}
				}
			}
			answer_split.push_back(word);
			if (!back_parentheses || answer_split.size() < 3) throw std::string("Invalid input");
			return answer_split;
		default:
			throw std::string("Invalid input");
		}
	}

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
		std::ofstream out("test2.txt");            //쓸 목적의 파일 선언 // 리셋 후 다시 쓰기 RERERER
		//char arr[11] = "BlockDMask";        //파일에 쓸 문자열
		if (out.is_open()) {
			//	writeFile.write(arr, 10);    //파일에 문자열 쓰기
		}
		out.close();
	}

	std::string menu(std::string consol) {
		std::string answer;

		std::cout << std::endl << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료" << std::endl;
		std::cout << "----------------------------------------------------------------------------------" << std::endl;
		std::cout << "(콘솔 메세지) " << consol << std::endl;
		std::cout << "----------------------------------------------------------------------------------" << std::endl;
		std::cout << "입력: ";

		//std::cin >> answer;
		getline(std::cin, answer);
		std::cout << "----------------------------------------------------------------------------------";


		return answer;
	}
};

int main() {

	TextBook* book = new TextBook("test.txt");
	TextEditor* textEditor = new TextEditor(book);

	textEditor->run();

	return 0;
}