#include "TextEditor.h"
#include "WordVector.h"
#include "ErrorManagement.h"


TextEditor::TextEditor(std::string file_name) {
	text_file_name = file_name;
	book = new TextBook();
	WordVector::instance()->splitWord(book->loadFile(text_file_name));
	
	now_page_line_idx = 0;
	line_idx.clear();

	if (WordVector::instance()->getTextList().size() > 0) {
		lineIdxInit(WordVector::instance()->getTextList(),0);
		for (int i = now_page_line_idx; i < WordVector::instance()->getAllLineIdx().size() && line_idx.size() < 21; i++) {
			line_idx.push_back(WordVector::instance()->getAllLineIdx()[i]);
		}
		printText(WordVector::instance()->getTextList());
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
		nextPage(answer_split);
		break; 

	case 'p':
		prevPage(answer_split);
		break; 

	case 't':
		book->writeFile(text_file_name, WordVector::instance()->getTextList());
		return 0;

	case 's':
		searchEdit(answer_split);
		break; 

	case 'd':
		eraseEdit(answer_split);
		break;
	
	case 'c': 
		changeEdit(answer_split);
		break;

	case 'i':
		insertEdit(answer_split);
		break;

	default:
		break;
	}
	return 1;
}


std::vector<int> TextEditor::searchPrevLineIdx(std::vector<std::string> text, int idx) {

	std::vector<int> temp_search_line_idx;
	int now_line_char_total=0, prev_line_char_total=0;
	int target_line_idx = 0;


	for (; target_line_idx < WordVector::instance()->getAllLineIdx().size(); target_line_idx++) {
		if (idx < WordVector::instance()->getAllLineIdx()[target_line_idx]) {
			target_line_idx--;
			break;
		}
	}

	if (idx == WordVector::instance()->getAllLineIdx()[target_line_idx]) {
		target_line_idx--;
		temp_search_line_idx.clear();
		for (; target_line_idx >= 0; target_line_idx--) {
			temp_search_line_idx.insert(temp_search_line_idx.begin(), WordVector::instance()->getAllLineIdx()[target_line_idx]);
		}
		return temp_search_line_idx;
	}

	int last_idx = idx;
	now_line_char_total = 0;
	prev_line_char_total = 0;

	for (int i = WordVector::instance()->getAllLineIdx()[target_line_idx]; i < last_idx; i++) {
		now_line_char_total += text[i].size() + 1;
	}

	while (now_line_char_total > 0 && target_line_idx > 0) {
		last_idx = WordVector::instance()->getAllLineIdx()[target_line_idx] - 1;
		for (int i = WordVector::instance()->getAllLineIdx()[--target_line_idx]; i < last_idx; i++) {
			prev_line_char_total += text[i].size() + 1;
		}

		now_line_char_total = prev_line_char_total + now_line_char_total - 1;
		prev_line_char_total = 0;
		if (now_line_char_total <= 75) {
			break;
		}
		else {
			int i = WordVector::instance()->getAllLineIdx()[target_line_idx];
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
			temp_search_line_idx.insert(temp_search_line_idx.begin(), WordVector::instance()->getAllLineIdx()[target_line_idx]);
		}

	}


	return temp_search_line_idx;
	
}

void TextEditor::lineIdxInit(std::vector<std::string> text, int st_idx) {
	WordVector::instance()->getAllLineIdx().clear();

	int line_char_total = 0;
	int i = st_idx;

	for (; i < text.size(); i++) {
		if (line_char_total == 0 || line_char_total + 1 + text[i].size() > 75) {
			line_char_total = text[i].size();
			WordVector::instance()->getAllLineIdx().push_back(i);
		}
		else {
			line_char_total += 1 + text[i].size();
		}
	}
	WordVector::instance()->getAllLineIdx().push_back(i);
}

void TextEditor::printText(std::vector<std::string> text) {

	if (WordVector::instance()->getTextList().size() > 0) {

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

void TextEditor::insertEdit(std::vector<std::string> answer_split) {
	int line_num = std::stoi(answer_split[0]);
	int sequence_num = std::stoi(answer_split[1]);

	if (line_idx.size() == 0) {
		if (line_num != 1 || sequence_num != 1) {
			throw std::string(ERR_MSG_SIZE_ZERO_WORD_INSERT);
		}
		else {
			WordVector::instance()->insertWord(line_idx.size(),0, answer_split[2]);

			line_idx.clear();
			line_idx.push_back(0);
			line_idx.push_back(1);

			WordVector::instance()->getAllLineIdx().clear();
			WordVector::instance()->getAllLineIdx().push_back(0);
			WordVector::instance()->getAllLineIdx().push_back(1);
			now_page_line_idx = 0;

			printText(WordVector::instance()->getTextList());
		}
	}
	else {
		if (line_num < 1 || line_num > line_idx.size() - 1) throw std::string(ERR_MSG_LINE_OVER);
		if (1 > sequence_num || line_idx[line_num] - line_idx[line_num - 1] + 1 < sequence_num) throw std::string(ERR_MSG_WORD_INDEX_OVER);  //RERE
		if (answer_split[2].size() > 75)throw std::string(ERR_MSG_WORD_OVER);

		WordVector::instance()->insertWord(line_idx.size(), line_idx[line_num - 1] + (sequence_num - 1), answer_split[2]);
		int idx = now_page_line_idx;
		for (; idx < WordVector::instance()->getAllLineIdx().size(); idx++) {
			if (line_idx[line_num - 1] == WordVector::instance()->getAllLineIdx()[idx]) break;
		}

		std::vector<int> temp_line_idx = WordVector::instance()->getAllLineIdx();
		lineIdxInit(WordVector::instance()->getTextList(), line_idx[line_num - 1]);

		for (int j = idx - 1; j >= 0; j--) { WordVector::instance()->getAllLineIdx().insert(WordVector::instance()->getAllLineIdx().begin(), temp_line_idx[j]); }

		int j = line_num - 1;
		for (; j < line_idx.size(); j++, idx++) {
			line_idx[j] = WordVector::instance()->getAllLineIdx()[idx];
		}

		if (idx < WordVector::instance()->getAllLineIdx().size()) {
			line_idx.push_back(WordVector::instance()->getAllLineIdx()[idx]);
		}


		printText(WordVector::instance()->getTextList());
		
	}
}
void TextEditor::eraseEdit(std::vector<std::string> answer_split) {

	if (WordVector::instance()->getTextList().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);

	int line_num = std::stoi(answer_split[0]);
	int sequence_num = std::stoi(answer_split[1]);

	if (WordVector::instance()->getTextList().size() == 1) {
		if (line_num == 1 || sequence_num == 1) {
			WordVector::instance()->eraseWord(line_idx[line_num - 1] + sequence_num - 1);

			line_idx.clear();
			WordVector::instance()->getAllLineIdx().clear();
		}
	}
	else {
		if (line_num < 1 || line_num > line_idx.size() - 1) throw std::string(ERR_MSG_LINE_OVER);
		if (1 > sequence_num || line_idx[line_num] - line_idx[line_num - 1] < sequence_num) throw std::string(ERR_MSG_WORD_INDEX_OVER);
		WordVector::instance()->eraseWord(line_idx[line_num - 1] + sequence_num - 1);
		int idx = now_page_line_idx;
		

		for (; idx < WordVector::instance()->getAllLineIdx().size(); idx++) {
			if (line_idx[line_num - 1] == WordVector::instance()->getAllLineIdx()[idx]) break;
		}


		std::vector<int> temp_line_idx = WordVector::instance()->getAllLineIdx();
		lineIdxInit(WordVector::instance()->getTextList(), line_idx[line_num - 1]);


		for (int j = idx - 1; j >= 0; j--) { WordVector::instance()->getAllLineIdx().insert(WordVector::instance()->getAllLineIdx().begin(), temp_line_idx[j]); }
		
		int j = line_num - 1;
		for (; j < line_idx.size() && idx < WordVector::instance()->getAllLineIdx().size(); j++, idx++) { line_idx[j] = WordVector::instance()->getAllLineIdx()[idx]; }

		if (j < line_idx.size()) {
			line_idx.pop_back();
		}
		
		
	}
	printText(WordVector::instance()->getTextList());
}
void TextEditor::changeEdit(std::vector<std::string> answer_split) {
	if (WordVector::instance()->getTextList().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
	if (answer_split[0].size() > 75 || answer_split[1].size() > 75) throw std::string(ERR_MSG_WORD_OVER);

	int idx = WordVector::instance()->searchWord(answer_split[0]);
	if (idx >= WordVector::instance()->getTextList().size()) {
		throw std::string(ERR_MSG_WORD_NOT_FOUND);
	}

	WordVector::instance()->changeWord(answer_split[0], answer_split[1]);

	lineIdxInit(WordVector::instance()->getTextList(), 0);
	now_page_line_idx = 0;

	line_idx.clear();
	for (int i = now_page_line_idx; i < WordVector::instance()->getAllLineIdx().size() && line_idx.size() < 21; i++) {
		line_idx.push_back(WordVector::instance()->getAllLineIdx()[i]);
	}

	printText(WordVector::instance()->getTextList());
}
void TextEditor::searchEdit(std::vector<std::string> answer_split) {
	if (WordVector::instance()->getTextList().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
	if (answer_split[0].size() > 75) throw std::string(ERR_MSG_WORD_OVER);

	int idx = WordVector::instance()->searchWord(answer_split[0]);

	if (idx >= WordVector::instance()->getTextList().size()) {
		throw std::string(ERR_MSG_WORD_NOT_FOUND);
	}
	else {

		std::vector<int> temp = searchPrevLineIdx(WordVector::instance()->getTextList(), idx);

		lineIdxInit(WordVector::instance()->getTextList(), idx);
		now_page_line_idx = 0;

		for (int i = temp.size() - 1; i >= 0; i--) {
			now_page_line_idx++;
			WordVector::instance()->getAllLineIdx().insert(WordVector::instance()->getAllLineIdx().begin(), temp[i]);
		}

		line_idx.clear();
		for (int i = now_page_line_idx; i < WordVector::instance()->getAllLineIdx().size() && line_idx.size() < 21; i++) {
			line_idx.push_back(WordVector::instance()->getAllLineIdx()[i]);
		}

		printText(WordVector::instance()->getTextList());
	}
}

void TextEditor::nextPage(std::vector<std::string> answer_split) {
	if (WordVector::instance()->getTextList().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
	if (now_page_line_idx + 20 >= WordVector::instance()->getAllLineIdx().size() - 1) throw std::string(ERR_MSG_LAST_PAGE);

	line_idx.clear();
	now_page_line_idx += 20;

	if ((now_page_line_idx + 19) > (WordVector::instance()->getAllLineIdx().size() - 1)) {
		int i = WordVector::instance()->getAllLineIdx().size() - 1;
		for (; i >= 0 && line_idx.size() < 21; i--) {
			line_idx.insert(line_idx.begin(), WordVector::instance()->getAllLineIdx()[i]);
		}
		now_page_line_idx = i + 1;
	}
	else {
		for (int i = now_page_line_idx; i < WordVector::instance()->getAllLineIdx().size() && line_idx.size() < 21; i++) {
			line_idx.push_back(WordVector::instance()->getAllLineIdx()[i]);
		}
	}

	printText(WordVector::instance()->getTextList());
}
void TextEditor::prevPage(std::vector<std::string> answer_split) {
	if (WordVector::instance()->getTextList().size() == 0) throw std::string(ERR_MSG_SIZE_ZERO);
	if (WordVector::instance()->getAllLineIdx()[now_page_line_idx] == 0) throw std::string(ERR_MSG_FIRST_PAGE);

	line_idx.clear();
	now_page_line_idx -= 20;

	if (now_page_line_idx < 0 || WordVector::instance()->getAllLineIdx()[now_page_line_idx] == 0) {
		lineIdxInit(WordVector::instance()->getTextList(), 0);
		now_page_line_idx = 0;
	}

	for (int i = now_page_line_idx; i < WordVector::instance()->getAllLineIdx().size() && line_idx.size() < 21; i++) {
		line_idx.push_back(WordVector::instance()->getAllLineIdx()[i]);
	}

	printText(WordVector::instance()->getTextList());

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

