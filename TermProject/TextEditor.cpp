#include "TextEditor.h"
#include "WordVector.h"
#include "ErrorManagement.h"


TextEditor::TextEditor(std::string file_name) {
	text_file_name = file_name;
	book = new TextBook();
	WordVector::instance()->splitWord(book->loadFile(text_file_name));

	now_page_line_idx = 0;
	line_idx.clear();

	if (WordVector::instance()->get_text_list().size() > 0) {
		line_idx_init(WordVector::instance()->get_text_list(),0);
		for (int i = now_page_line_idx; i < all_line_idx.size() && line_idx.size() < 21; i++) {
			line_idx.push_back(all_line_idx[i]);
		}
		print_text(WordVector::instance()->get_text_list());
	}

}

void TextEditor::run() {
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
			consol_msg = "Error";
		}
	}
}

int TextEditor::call(std::string consol_msg = "") {
	std::string answer = menu(consol_msg);
	if (answer.size() < 1) throw std::string(ERR_MSG_INVALID_INPUT);

	int idx = 0, line_num = 0, sequence_num = 0;
	std::vector<std::string> answer_split = keyword_check(answer);
	std::vector<int> temp_line_idx;

	switch (answer.front())
	{
	case 'n':

		if (WordVector::instance()->get_text_list().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
		if (now_page_line_idx + 20 >= all_line_idx.size()-1) throw std::string(ERR_MSG_LAST_PAGE);

		line_idx.clear();
		now_page_line_idx += 20;

		if ((now_page_line_idx + 19) > (all_line_idx.size() - 1)) {
			int i = all_line_idx.size() - 1;
			for (; i >= 0 && line_idx.size() < 21; i--) {
				line_idx.insert(line_idx.begin(), all_line_idx[i]);
			}
			now_page_line_idx = i+1;
		}
		else {
			for (int i = now_page_line_idx; i < all_line_idx.size() && line_idx.size() < 21; i++) {
				line_idx.push_back(all_line_idx[i]);
			}
		}

		print_text(WordVector::instance()->get_text_list());

		break; 

	case 'p':
		if (WordVector::instance()->get_text_list().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
		if (all_line_idx[now_page_line_idx] == 0) throw std::string(ERR_MSG_FIRST_PAGE);

		line_idx.clear();
		now_page_line_idx -= 20;

		if (now_page_line_idx < 0 || all_line_idx[now_page_line_idx] == 0) {
			line_idx_init(WordVector::instance()->get_text_list(), 0);
			now_page_line_idx = 0;
		}

		for (int i = now_page_line_idx; i < all_line_idx.size() && line_idx.size() < 21; i++) {
			line_idx.push_back(all_line_idx[i]);
		}
		
		print_text(WordVector::instance()->get_text_list());
	
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

			std::vector<int> temp = search_prev_line_idx(WordVector::instance()->get_text_list(), idx);

			line_idx_init(WordVector::instance()->get_text_list(), idx);
			now_page_line_idx = 0;

			for (int i = temp.size() - 1; i >= 0; i--) {
				now_page_line_idx++;
				all_line_idx.insert(all_line_idx.begin(), temp[i]);
			}

			line_idx.clear();
			for (int i = now_page_line_idx; i < all_line_idx.size() && line_idx.size() < 21; i++) {
				line_idx.push_back(all_line_idx[i]);
			}

			print_text(WordVector::instance()->get_text_list());
		}

		break; 

	case 'd':

		if (WordVector::instance()->get_text_list().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);

		line_num = std::stoi(answer_split[0]);
		sequence_num = std::stoi(answer_split[1]);

		if (WordVector::instance()->get_text_list().size() == 1) {
			if (line_num == 1 || sequence_num == 1) {
				eraseWord(line_num, sequence_num);
				line_idx.clear();
				all_line_idx.clear();
			}
		}
		else {
			if (line_num < 1 || line_num > line_idx.size() - 1) throw std::string(ERR_MSG_LINE_OVER);
			if (1 > sequence_num || line_idx[line_num] - line_idx[line_num - 1] < sequence_num) throw std::string(ERR_MSG_WORD_INDEX_OVER);

			eraseWord(line_num, sequence_num);
			idx = now_page_line_idx;
			for (; idx < all_line_idx.size(); idx++) {
				if (line_idx[line_num - 1] == all_line_idx[idx]) break;
			}

			temp_line_idx = all_line_idx;
			line_idx_init(WordVector::instance()->get_text_list(), line_idx[line_num - 1]);


			for (int j = idx - 1; j >= 0; j--) { all_line_idx.insert(all_line_idx.begin(), temp_line_idx[j]); }

			for (int j = line_num - 1; j < line_idx.size(); j++, idx++) { line_idx[j] = all_line_idx[idx]; }
		}
		print_text(WordVector::instance()->get_text_list());
		break;
	
	case 'c': 
	
		if (WordVector::instance()->get_text_list().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
		if (answer_split[0].size() > 75 || answer_split[1].size() > 75) throw std::string(ERR_MSG_WORD_OVER);

		idx = searchWord(answer_split[0]);
		if (idx >= WordVector::instance()->get_text_list().size()) {
			throw std::string(ERR_MSG_WORD_NOT_FOUND);
		}

		changeWord(answer_split[0], answer_split[1]);

		line_idx_init(WordVector::instance()->get_text_list(),0);
		now_page_line_idx = 0;

		line_idx.clear();
		for (int i = now_page_line_idx; i < all_line_idx.size() && line_idx.size() < 21; i++) {
			line_idx.push_back(all_line_idx[i]);
		}

		print_text(WordVector::instance()->get_text_list());

		break;

	case 'i':

		line_num = std::stoi(answer_split[0]);
		sequence_num = std::stoi(answer_split[1]);

		if (line_idx.size() == 0) {
			if (line_num != 1 || sequence_num != 1) {
				throw std::string(ERR_MSG_SIZE_ZERO_WORD_INSERT);
			}
			else {
				insertWord(line_num, sequence_num, answer_split[2]);
				line_idx.clear();
				line_idx.push_back(0);
				line_idx.push_back(1);

				all_line_idx.clear();
				all_line_idx.push_back(0);
				all_line_idx.push_back(1);
				now_page_line_idx = 0;

				print_text(WordVector::instance()->get_text_list());
			}
		}
		else {
			if (line_num < 1 || line_num > line_idx.size() - 1) throw std::string(ERR_MSG_LINE_OVER);
			if (1 > sequence_num || line_idx[line_num] - line_idx[line_num - 1] + 1 < sequence_num) throw std::string(ERR_MSG_WORD_INDEX_OVER);  //RERE
			if (answer_split[2].size() > 75)throw std::string(ERR_MSG_WORD_OVER);

			insertWord(line_num, sequence_num, answer_split[2]);
			idx = now_page_line_idx;
			for (; idx < all_line_idx.size(); idx++) {
				if (line_idx[line_num - 1] == all_line_idx[idx]) break;
			}

			temp_line_idx = all_line_idx;
			line_idx_init(WordVector::instance()->get_text_list(), line_idx[line_num - 1]);

			for (int j = idx - 1; j >= 0; j--) { all_line_idx.insert(all_line_idx.begin(), temp_line_idx[j]); }
			for (int j = line_num - 1; j < line_idx.size(); j++, idx++) { line_idx[j] = all_line_idx[idx]; }

			print_text(WordVector::instance()->get_text_list());
		}
		break;

		

	default:
		break;
	}
	return 1;
}


std::vector<int> TextEditor::search_prev_line_idx(std::vector<std::string> text, int idx) {

	std::vector<int> temp_search_line_idx;
	int now_line_char_total=0, prev_line_char_total=0;
	int target_line_idx = 0;


	for (; target_line_idx < all_line_idx.size(); target_line_idx++) {
		if (idx < all_line_idx[target_line_idx]) {
			target_line_idx--;
			break;
		}
	}

	if (idx == all_line_idx[target_line_idx]) {
		target_line_idx--;
		temp_search_line_idx.clear();
		for (; target_line_idx >= 0; target_line_idx--) {
			temp_search_line_idx.insert(temp_search_line_idx.begin(), all_line_idx[target_line_idx]);
		}
		return temp_search_line_idx;
	}

	int last_idx = idx;
	now_line_char_total = 0;
	prev_line_char_total = 0;

	for (int i = all_line_idx[target_line_idx]; i < last_idx; i++) {
		now_line_char_total += text[i].size() + 1;
	}

	while (now_line_char_total > 0 && target_line_idx > 0) {
		last_idx = all_line_idx[target_line_idx] - 1;
		for (int i = all_line_idx[--target_line_idx]; i < last_idx; i++) {
			prev_line_char_total += text[i].size() + 1;
		}

		now_line_char_total = prev_line_char_total + now_line_char_total - 1;
		prev_line_char_total = 0;
		if (now_line_char_total <= 75) {
			break;
		}
		else {
			int i = all_line_idx[target_line_idx];
			for (; i < last_idx; i++) {
				now_line_char_total -= text[i].size() + 1;
				prev_line_char_total += text[i].size() + 1;
				if (now_line_char_total <= 75) break;
			}

			temp_search_line_idx.insert(temp_search_line_idx.begin(), i+1);

			now_line_char_total = prev_line_char_total;
			prev_line_char_total = 0;
		}
	}



	if (0 >= target_line_idx) {
		temp_search_line_idx.insert(temp_search_line_idx.begin(), 0);
	}
	else {
		for (;target_line_idx >= 0; target_line_idx--) {
			temp_search_line_idx.insert(temp_search_line_idx.begin(), all_line_idx[target_line_idx]);
		}

	}


	return temp_search_line_idx;
	
}

void TextEditor::line_idx_init(std::vector<std::string> text, int st_idx) {
	all_line_idx.clear();

	int line_char_total = 0;
	int i = st_idx;

	for (; i < text.size(); i++) {
		if (line_char_total == 0 || line_char_total + 1 + text[i].size() > 75) {
			line_char_total = text[i].size();
			all_line_idx.push_back(i);
		}
		else {
			line_char_total += 1 + text[i].size();
		}
	}
	all_line_idx.push_back(i);
}


void TextEditor::print_text(std::vector<std::string> text) {

	if (WordVector::instance()->get_text_list().size() > 0) {

	for (int i = 0, line_num = 1; i < line_idx.size() - 1 && line_num <= 20; i++, line_num++) {

		if (line_num < 10) std::cout << std::endl << ' ' << line_num << "|";
		else std::cout << std::endl << line_num << "|";

		for (int j = line_idx[i]; j < line_idx[i + 1]; j++) {
			std::cout << ' ' << text[j];
		}
	}
}
	std::cout << std::endl << "----------------------------------------------------------------------------------";
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
	else WordVector::instance()->get_text_list().insert(WordVector::instance()->get_text_list().begin() + line_idx[line - 1] + (idx - 1), word);
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

