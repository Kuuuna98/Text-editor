#include "TextEditor.h"
#include "WordVector.h"
#include "ErrorManagement.h"


TextEditor::TextEditor(std::string file_name) {
	text_file_name = file_name;
	book = new TextBook();
	WordVector::instance()->splitWord(book->loadFile(text_file_name));
	now_page_idx = 0;
	page_idx.push_back(0);

	if (WordVector::instance()->get_text_list().size() > 0) {

		now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);
	}

}

void TextEditor::run() {
	int execute = 1;
	std::string consol_msg = "";
	while (execute) {
		try {
			if (WordVector::instance()->get_text_list().size() == 0) consol_msg = "텍스트에 내용없음";
			execute = call(consol_msg);
			consol_msg = "";
		}
		catch (std::string s) {
			consol_msg = s;
		}
		catch (...) {
			consol_msg = "Error";
		}
	}
}

int TextEditor::call(std::string consol_msg = "") {
	std::string answer = menu(consol_msg);
	if (answer.size() < 1) throw std::string(ERR_MSG_SIZE_ZERO);

	int idx = 0, line_num = 0, sequence_num = 0;
	std::vector<std::string> answer_split = keyword_check(answer);

	switch (answer.front())
	{
	case 'n':

		if (WordVector::instance()->get_text_list().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
		if (page_idx[now_page_idx + 1] >= WordVector::instance()->get_text_list().size()) throw std::string(ERR_MSG_LAST_PAGE);
		if (page_idx[now_page_idx] == 0) now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		now_page_idx++;		
		idx = print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);
		if (page_idx[now_page_idx + 1] >= WordVector::instance()->get_text_list().size()) {
			page_idx.erase(page_idx.begin()+ now_page_idx);
			page_idx.insert(page_idx.begin()+ now_page_idx,idx);
		}
		break; 

	case 'p':
		if (WordVector::instance()->get_text_list().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
		if (page_idx[now_page_idx] == 0) throw std::string(ERR_MSG_FIRST_PAGE);
		if (page_idx[now_page_idx + 1] >= WordVector::instance()->get_text_list().size()) now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		for (auto v : page_idx) {
			std::cout << " " << v;
		}
		std::cout << std::endl;

		now_page_idx--;
		print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);	

		for (auto v : page_idx) {
			std::cout << " " << v;
		}
		std::cout << std::endl;
		break; 

	case 't':
		book->writeFile(text_file_name, WordVector::instance()->get_text_list());
		return 0;

	case 's':
		if (WordVector::instance()->get_text_list().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
		if (answer_split[0].size() > 75) throw std::string(ERR_MSG_WORD_OVER);

		idx = searchWord(answer_split[0]);

		if (idx >= WordVector::instance()->get_text_list().size()) {
			throw std::string(ERR_MSG_WORD_NOT_FOUND);
		}
		else {
			std::cout << std::endl<< now_page_idx <<" "<< page_idx[986]<<" 이전: ";
			for (auto v : page_idx) {
				std::cout << " "<<v;
			}
			std::cout << std::endl;

			now_page_idx = search_set_page_and_line(WordVector::instance()->get_text_list(), idx);
			print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);
		}
		now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);
		break; 

	case 'd':

		if (WordVector::instance()->get_text_list().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);

		line_num = std::stoi(answer_split[0]);
		sequence_num = std::stoi(answer_split[1]);

		if (line_num < 1 || line_num > line_idx.size() - 1) throw std::string(ERR_MSG_LINE_OVER);  
		if (1 > sequence_num || line_idx[line_num] - line_idx[line_num - 1] < sequence_num) throw std::string(ERR_MSG_WORD_INDEX_OVER);

		eraseWord(line_num, sequence_num);

		now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);
		print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		break;
	
	case 'c': 
	
		if (WordVector::instance()->get_text_list().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
		if (answer_split[0].size() > 75 || answer_split[1].size() > 75) throw std::string(ERR_MSG_WORD_OVER);

		idx = searchWord(answer_split[0]);
		if (idx >= WordVector::instance()->get_text_list().size()) {
			throw std::string(ERR_MSG_WORD_NOT_FOUND);
		}

		changeWord(answer_split[0], answer_split[1]);
		now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), 0);
		print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		break;

	case 'i':

		std::cout << std::endl << now_page_idx << " 이전iiii: ";
		for (auto v : page_idx) {
			std::cout << " " << v;
		}
		std::cout << std::endl;

		line_num = std::stoi(answer_split[0]);
		sequence_num = std::stoi(answer_split[1]);
		if (line_idx.size() == 0) {
			if (line_num != 1 || sequence_num != 1) {
				throw std::string(ERR_MSG_SIZE_ZERO_WORD_INSERT);
			}
		}
		else {
			if (line_num < 1 || line_num > line_idx.size() - 1) throw std::string(ERR_MSG_LINE_OVER); 
			if (1 > sequence_num || line_idx[line_num] - line_idx[ line_num - 1] + 1 < sequence_num) throw std::string(ERR_MSG_WORD_INDEX_OVER);  //RERE
			if (answer_split[2].size() > 75)throw std::string(ERR_MSG_WORD_OVER); 
		}
		insertWord(line_num, sequence_num, answer_split[2]);

		now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);
		std::cout << std::endl << now_page_idx << " 이후iiii: ";
		for (auto v : page_idx) {
			std::cout << " " << v;
		}
		std::cout << std::endl;
		print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		std::cout << std::endl << now_page_idx << " 이끝iiii: ";
		for (auto v : page_idx) {
			std::cout << " " << v;
		}
		std::cout << std::endl;

		break;

	default:
		break;
	}
	return 1;
}

int TextEditor::set_page_and_line(std::vector<std::string> text, int now) {
	int idx_temp=now;
	std::vector<int> line_idx_temp;
	int temp_now_page_idx = 0;

	page_idx.clear();
	page_idx.push_back(now);

	//이전페이지
	while (idx_temp > 0) {
		temp_now_page_idx++;
		idx_temp = check_prevline_idx(text, idx_temp)[0];
		page_idx.insert(page_idx.begin(),idx_temp);
	}

	idx_temp = now;
	//다음페이지
	while (idx_temp < WordVector::instance()->get_text_list().size()) {
		line_idx_temp = check_nextline_idx(text, idx_temp);
		if (idx_temp == now) line_idx = line_idx_temp;
		idx_temp = line_idx_temp[line_idx_temp.size()-1];
		if (idx_temp == WordVector::instance()->get_text_list().size()) {
			page_idx.pop_back();
			page_idx.push_back(line_idx_temp[0]);
		}
		page_idx.push_back(idx_temp);
	}

	return temp_now_page_idx;
}
int TextEditor::search_set_page_and_line(std::vector<std::string> text, int now) {
	int temp_now_page_idx = 0;
	std::vector<int> line_idx_temp;
	int page_total = 1;
	int line_char_total = 0;
	int idx_temp = now;

	line_idx_temp = check_line_idx(text, now);

	if (line_idx_temp.size() < 20) {
		page_idx.clear();
		page_idx.push_back(now);

		//이전페이지
		while (idx_temp > 0) {
			temp_now_page_idx++;
			idx_temp = check_prevline_idx(text, idx_temp)[0];
			page_idx.insert(page_idx.begin(), idx_temp);
		}
		line_idx = line_idx_temp;
		page_idx.push_back(line_idx[line_idx.size()-1]);
		return temp_now_page_idx;
	}
	else {
		return set_page_and_line(text, now);
	}

}

std::vector<int> TextEditor::check_prevline_idx(std::vector<std::string> text, int next) {

	std::vector<int> line_idx_temp;
	int page_total = 1;
	int line_char_total = 0;
	int i = next-1;

	for (; i >= 0 && line_idx_temp.size() < 20; i--) {

		if (line_char_total + 1 + text[i].size() > 75) { //next line

			if(line_char_total + text[i].size() == 75) {
				line_char_total = 0;
				line_idx_temp.insert(line_idx_temp.begin(), i);
			}
			else {
				line_char_total = text[i].size();
				line_idx_temp.insert(line_idx_temp.begin(), i+1);
			}
		}
		else {
			line_char_total += 1 + text[i].size();
		}

	}

	if (i < 0) {
		line_idx_temp = check_nextline_idx(text, 0);
	}
	else {
		line_idx_temp.push_back(next);
	}

	std::cout << next << "prev line_idx_temp: ";
	for (auto v : line_idx_temp) {
		std::cout << " " << v;
	}
	std::cout << std::endl;

	return line_idx_temp;
}
std::vector<int> TextEditor::check_nextline_idx(std::vector<std::string> text, int now) {
	std::vector<int> line_idx_temp;
	int page_total = 1;
	int line_char_total = 0;
	int i = now;
	std::vector<int> line_idx_init = check_line_idx(text, 0);

	if (line_idx_init[line_idx_init.size() - 1] == WordVector::instance()->get_text_list().size()) {
		return line_idx_init;
	}

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


	if (line_idx_temp [0]!=0&& line_idx_temp.size() < 20) {
		
		std::vector<int> prev_line_idx_temp = check_nextline_idx(text, page_idx[page_idx.size()-2]);

		int target = prev_line_idx_temp.size() - (1 + 20 - line_idx_temp.size());
		for (int j = prev_line_idx_temp.size() - 2; j >= target;j--) {
			line_idx_temp.insert(line_idx_temp.begin(), prev_line_idx_temp[j]);
		}
	}

	line_idx_temp.push_back(i);

	return line_idx_temp;
}

std::vector<int> TextEditor::check_line_idx(std::vector<std::string> text, int now) {
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

	line_idx_temp.push_back(i);

	return line_idx_temp;
}

int TextEditor::print_text(std::vector<std::string> text, int now) {

	if (WordVector::instance()->get_text_list().size() == 0) return -1;
	line_idx = check_line_idx(text, now);

	for (int i = line_idx[0], line_num = 0; i < text.size() && line_num <= 20; i++) {

		if (i == line_idx[line_num]) { //next line
			if (++line_num > 20) break;

			if (line_num < 10) std::cout << std::endl << ' ' << line_num << "|";
			else std::cout << std::endl << line_num << "|";
		}
		std::cout << ' ' << text[i];
	}

	std::cout << std::endl << "----------------------------------------------------------------------------------";


	return line_idx[0];
}


std::string TextEditor::getInvalidInputMSG(bool follow, char keyword) {
	std::string msg = "Invalid input: The '";
	msg += keyword;

	if (follow) msg += "' keyword can be followed by anything word.";
	else msg += "' keyword cannot be followed by anything word.";

	return msg;
}

std::vector<std::string> TextEditor::keyword_check(std::string answer) {

	char keyword(answer.front());
	std::string message = "";
	std::vector<std::string> answer_split;
	std::string word = "";
	bool front_parentheses = false, back_parentheses = false;

	switch (keyword)
	{
	case ' ': throw std::string(ERR_MSG_SPACE_KEYWORD);
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
				if (front_parentheses && !back_parentheses) throw std::string(ERR_MSG_SPACE_IN_PARENTHESES);
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
		if (!back_parentheses) throw std::string(ERR_MSG_NOT_CLOSED_PARENTHESES);

		answer_split.push_back(word);
		return answer_split;

	case 'c':
		if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));


		for (int i = 1; i < answer.size(); i++) {
			if (answer.at(i) == ' ') {
				if (front_parentheses && !back_parentheses) throw std::string(ERR_MSG_SPACE_IN_PARENTHESES);
			}
			else if (answer.at(i) == ',') {
				answer_split.push_back(word);
				word = "";
				if (answer_split.size() > 1) throw std::string(ERR_MSG_CHECK_ARGUMENT);
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
		if (!back_parentheses) throw std::string(ERR_MSG_NOT_CLOSED_PARENTHESES);
		if(answer_split.size() < 2) throw std::string(ERR_MSG_CHECK_ARGUMENT);
		return answer_split;

	case 'd':
		if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));

		for (int i = 1; i < answer.size(); i++) {
			if (answer.at(i) == ' ') {
				if (front_parentheses && !back_parentheses) throw std::string(ERR_MSG_SPACE_IN_PARENTHESES);
			}
			else if (answer.at(i) == ',') {
				answer_split.push_back(word);
				word = "";
				if (answer_split.size() > 1) throw std::string(ERR_MSG_CHECK_ARGUMENT);
			}
			else {
				if (answer.at(i) == '(') {
					if (front_parentheses) throw std::string(ERR_MSG_CHECK_ARGUMENT);
					front_parentheses = true;
				}
				else if (answer.at(i) == ')') {
					if (back_parentheses) throw std::string(ERR_MSG_CHECK_ARGUMENT);
					back_parentheses = true;
				}
				else {
					if (back_parentheses) throw std::string(ERR_MSG_CHECK_ARGUMENT);
					if (answer.at(i) < 48 || answer.at(i) > 57) throw std::string(ERR_MSG_KEYWORD_D_ARGUMENT);
					word += answer.at(i);
				}
			}
		}
		answer_split.push_back(word);
		if (!back_parentheses)throw std::string(ERR_MSG_NOT_CLOSED_PARENTHESES);
		if (answer_split.size() < 2) throw std::string(ERR_MSG_CHECK_ARGUMENT);
		return answer_split;

	case 'i':
		if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));

		for (int i = 1; i < answer.size(); i++) {
			if (answer.at(i) == ' ') {
				if (front_parentheses && !back_parentheses) throw std::string(ERR_MSG_SPACE_IN_PARENTHESES);
			}
			else if (answer.at(i) == ',') {
				answer_split.push_back(word);
				word = "";
				if (answer_split.size() > 2) throw std::string(ERR_MSG_CHECK_ARGUMENT);
			}
			else {
				if (answer.at(i) == '(') {
					if (front_parentheses) throw std::string(ERR_MSG_CHECK_ARGUMENT);
					front_parentheses = true;
				}
				else if (answer.at(i) == ')') {
					if (back_parentheses) throw std::string(ERR_MSG_CHECK_ARGUMENT);
					back_parentheses = true;
				}
				else {
					if (back_parentheses) throw std::string(ERR_MSG_CHECK_ARGUMENT);
					if (answer_split.size() < 2 && (answer.at(i) < 48 || answer.at(i) > 57)) throw std::string(ERR_MSG_KEYWORD_I_ARGUMENT);
					word += answer.at(i);
				}
			}
		}
		answer_split.push_back(word);
		if (!back_parentheses)throw std::string(ERR_MSG_NOT_CLOSED_PARENTHESES);
		if(answer_split.size() < 3) throw std::string(ERR_MSG_CHECK_ARGUMENT);
		return answer_split;
	default:
		throw std::string(ERR_MSG_INVALID_INPUT);
	}
}

void TextEditor::insertWord(int line, int idx, std::string word) {
	if (line_idx.size() == 0) WordVector::instance()->get_text_list().insert(WordVector::instance()->get_text_list().begin(), word);
	else WordVector::instance()->get_text_list().insert(WordVector::instance()->get_text_list().begin() + line_idx[ line - 1] + ( idx - 1), word);
}

void TextEditor::eraseWord(int line, int idx) {
	WordVector::instance()->get_text_list().erase(WordVector::instance()->get_text_list().begin() + line_idx[ line - 1] + idx - 1);
}

void TextEditor::changeWord(std::string beforeWord, std::string afterWord) {
	for (int i = 0; i < WordVector::instance()->get_text_list().size(); i++) {
		if (WordVector::instance()->get_text_list()[i]._Equal(beforeWord)) {
			WordVector::instance()->get_text_list().erase(WordVector::instance()->get_text_list().begin() + i);
			WordVector::instance()->get_text_list().insert(WordVector::instance()->get_text_list().begin() + i, afterWord);
		}
	}
}

int TextEditor::searchWord(std::string word) {
	int i = 0;
	for (; i < WordVector::instance()->get_text_list().size(); i++) {
		if (WordVector::instance()->get_text_list()[i]._Equal(word)) {
			break;
		}
	}
	return i;
}

std::string TextEditor::menu(std::string consol) {
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