#ifndef GUI_QUERY_CONTENTS_H_
#define GUI_QUERY_CONTENTS_H_

#include "CS_Shapes.h"
#include "fltk.h"
#include "Gui.h"
#include "Window.h"
#include "CS_Text_Display.h"

class gui_query_contents : public Window {
private:
	int display_box_w = 200;
	int display_box_h = 50;

	Point tl_offset{ 0, 50 };
	Point pt_in_file_name{ tl_offset.x + display_box_w, tl_offset.y + 0 * display_box_h };
	Point pt_out_word_with_largest_count{ tl_offset.x + display_box_w, tl_offset.y + 1 * display_box_h };
	Point pt_out_longest_word{ tl_offset.x + display_box_w, tl_offset.y + 2 * display_box_h };		Point pt_out_shortest_word{ tl_offset.x + 3 * display_box_w, tl_offset.y + 2 * display_box_h };
	Point pt_in_words_that_start{ tl_offset.x + display_box_w, tl_offset.y + 3 * display_box_h };
	Point pt_in_words_with_characters{ tl_offset.x + display_box_w, tl_offset.y + 4 * display_box_h };
	Point pt_out_contents{ tl_offset.x + display_box_w, tl_offset.y + 5 * display_box_h };

	std::string _filename;
	vector<std::string> _contents;

public:
	// shape/widgets listed in representation of GUI
	In_box in_file_name{ pt_in_file_name, display_box_w, display_box_h, "File name:" };													Button submit_file_name{ Point{pt_in_file_name.x + display_box_w, pt_in_file_name.y}, display_box_w, display_box_h, "SUBMIT", cb_get_filename };
	CS_Text_Display out_word_with_largest_count{ pt_out_word_with_largest_count, display_box_w, display_box_h, "Word with largest count:" };
	CS_Text_Display out_longest_word{ pt_out_longest_word, display_box_w, display_box_h, "Longest word(s):" };									CS_Text_Display out_shortest_word{ pt_out_shortest_word, display_box_w, display_box_h, "Shortest word(s):" };
	In_box in_words_that_start{ pt_in_words_that_start, display_box_w, display_box_h, "Words that start with:"};						Button submit_words_that{ Point{pt_in_words_that_start.x + display_box_w, pt_in_words_that_start.y}, display_box_w, display_box_h, "SUBMIT", cb_start_given };				CS_Text_Display out_words_that_start{ Point{pt_in_words_that_start.x + 2*display_box_w, pt_in_words_that_start.y}, display_box_w, display_box_h};
	In_box in_words_with_characters{ pt_in_words_with_characters, display_box_w, display_box_h, "Words with # characters:" };			Button submit_words_with{ Point{pt_in_words_with_characters.x + display_box_w, pt_in_words_with_characters.y}, display_box_w, display_box_h, "SUBMIT", cb_with_given };				CS_Text_Display out_words_with{ Point{pt_in_words_with_characters.x + 2 * display_box_w, pt_in_words_with_characters.y}, display_box_w, display_box_h };
	CS_Text_Display out_contents{ pt_out_contents , 3 * display_box_w, 3*display_box_h, "Contents:" };
	
	Button next_button;
	bool button_pushed;
	gui_query_contents(Point xy, int w, int h, const string& title)
		: Window(xy, w, h, title),
		button_pushed(false),
		next_button(Point(x_max() - 70, 0), 70, 20, "Exit", cb_next)
	{
		attach(next_button);

		attach(in_file_name);	attach(submit_file_name);
		attach(out_word_with_largest_count);
		attach(out_longest_word);	attach(out_shortest_word);
		attach(in_words_that_start); attach(submit_words_that);	attach(out_words_that_start);
		attach(in_words_with_characters); attach(submit_words_with); attach(out_words_with);
		attach(out_contents);
	}

	void wait_for_button()
		// modified event loop
		// handle all events (as per default), but quit when button_pushed becomes true
		// this allows graphics without control inversion
	{
		while (!button_pushed) Fl::wait();
		button_pushed = false;
		Fl::redraw();
	}

	void next() { button_pushed = true; }
	static void cb_next(Address, Address addr) // callback for next_button
	//	{ reference_to<Simple_window>(addr).next(); }
	{
		static_cast<gui_query_contents*>(addr)->next();
	}

	static void cb_get_filename(Address, Address addr) // callback for next_button
	//	{ reference_to<Simple_window>(addr).next(); }
	{
		std::string filename = static_cast<gui_query_contents*>(addr)->in_file_name.get_string();
		static_cast<gui_query_contents*>(addr)->_filename = filename;

		static_cast<gui_query_contents*>(addr)->file_name_given(addr);
	}
	static void file_name_given(Address addr) {

		static_cast<gui_query_contents*>(addr)->_contents.clear();
		scan_file(addr);
		if (static_cast<gui_query_contents*>(addr)->_contents.size() > 0)
		{

			static_cast<gui_query_contents*>(addr)->fill_largest(addr);
			static_cast<gui_query_contents*>(addr)->fill_longest(addr);
			static_cast<gui_query_contents*>(addr)->fill_shortest(addr);
			static_cast<gui_query_contents*>(addr)->fill_contents(addr);
		}
	}

	static void scan_file(Address addr) {
		std::ifstream in{ static_cast<gui_query_contents*>(addr)->_filename };

		static_cast<gui_query_contents*>(addr)->_contents.clear();
		while (in)
		{
			std::string temp;
			in >> temp;
			if (in)
				static_cast<gui_query_contents*>(addr)->_contents.push_back(temp);
		}
	}

	static void cb_start_given(Address, Address addr) {
		std::string start_with = static_cast<gui_query_contents*>(addr)->in_words_that_start.get_string();
		auto& vect = static_cast<gui_query_contents*>(addr)->_contents;
		std::string ans = match_word_start(vect, start_with);
		static_cast<gui_query_contents*>(addr)->out_words_that_start.set_buffer(ans);
	}

	static void cb_with_given(Address, Address addr) {
		int i = static_cast<gui_query_contents*>(addr)->in_words_with_characters.get_int();
		auto& vect = static_cast<gui_query_contents*>(addr)->_contents;
		std::string ans = return_words_with_i_count(vect, i);
		static_cast<gui_query_contents*>(addr)->out_words_with.set_buffer(ans);
	}

	static std::string word_count_query(vector<std::string>& vect, std::string query);		//count user - defined string in passage
	static std::string word_with_largest_count(vector<std::string>& vect); // output word(s) with the largest count
	static std::string longest_word(vector<std::string>& vect); // output word(s) with the most characters
	static std::string shortest_word(vector<std::string>& vect); // output word(s) with the least characters
	static std::string match_word_start(vector<std::string>& vect, std::string start); // output word(s) starting with the user-defined character sequence
	static std::string return_words_with_i_count(vector<std::string>& vect, int i);	// output word(s) with the user-defined number of characters


	void fill_contents(Address addr) {
		auto& vect_copy = static_cast<gui_query_contents*>(addr)->_contents;
		std::string preview;
		for (int i = 0; i < vect_copy.size(); i++) {
			preview += vect_copy[i];
			preview += ' ';
			if (i % 20 == 0 && i!=0)
				preview += '\n';
		}
		static_cast<gui_query_contents*>(addr)->out_contents.set_buffer(preview);
	}

	void fill_largest(Address addr) {
		static_cast<gui_query_contents*>(addr)->out_word_with_largest_count.set_buffer(word_with_largest_count(static_cast<gui_query_contents*>(addr)->_contents));
	}

	void fill_longest(Address addr) {
		static_cast<gui_query_contents*>(addr)->out_longest_word.set_buffer(longest_word(static_cast<gui_query_contents*>(addr)->_contents));
	}

	void fill_shortest(Address addr) {
		static_cast<gui_query_contents*>(addr)->out_shortest_word.set_buffer(shortest_word(static_cast<gui_query_contents*>(addr)->_contents));
	}



};



std::string gui_query_contents::word_count_query(vector<std::string>& vect, std::string query) {
	std::string ret;
	int count{ 0 };
	for (int i = 0; i < vect.size(); i++)
		if (vect[i] == query)
			++count;
	ret = "query appeared " + std::to_string(count);
	if (count == 1)
		ret+= " time.\n";
	else
		ret+= " times.\n";

	return ret;
}
std::string gui_query_contents::word_with_largest_count(vector<std::string>& vect) {
	std::string ret;
	std::map<std::string, int> words;

	for (int i = 0; i < vect.size(); i++)
		++words[vect[i]];
	int most_common{ 0 };

	for (auto temp : words)
		if (temp.second > most_common)
			most_common = temp.second;

	vector<std::string> most_common_words;
	for (auto temp : words)
		if (temp.second == most_common)
			most_common_words.push_back(temp.first);

	for (int i = 0; i < most_common_words.size(); i++)
		ret += most_common_words[i] + ' ';
	ret += '\n';

	return ret;
}

std::string gui_query_contents::longest_word(vector<std::string>& vect) {
	std::string ret;
	int longest{ 0 };
	for (int i = 0; i < vect.size(); i++)
		if (vect[i].size() > longest)
			longest = vect[i].size();

	vector<std::string> longest_words;
	for (auto temp : vect)
		if (temp.size() == longest)
			longest_words.push_back(temp);

	for (int i = 0; i < longest_words.size(); i++)
		ret += longest_words[i] + '\t';

	return ret;
}

std::string gui_query_contents::shortest_word(vector<std::string>& vect) {
	std::string ret;
	int shortest = vect[0].size();
	for (int i = 0; i < vect.size(); i++)
		if (vect[i].size() < shortest && vect[i].size() > 0)
			shortest = vect[i].size();

	vector<std::string> shortest_words;
	for (auto temp : vect)
		if (temp.size() == shortest)
			shortest_words.push_back(temp);

	std::sort(shortest_words.begin(), shortest_words.end());
	shortest_words.erase(std::unique(shortest_words.begin(), shortest_words.end()), shortest_words.end());

	for (int i = 0; i < shortest_words.size(); i++)
		ret += shortest_words[i] + ' ';

	return ret;
}

std::string gui_query_contents::match_word_start(vector<std::string>& vect, std::string start) {
	std::string ret;
	vector<std::string> matching_words;

	for (int i = 0; i < vect.size(); i++)
	{
		bool match{ false };
		if (start.size() <= vect[i].size()) {
			match = true;
			for (int j = 0; j < start.size(); j++) {
				if (start[j] != vect[i][j])
					match = false;
			}
		}
		if (match)
			matching_words.push_back(vect[i]);
	}

	std::sort(matching_words.begin(), matching_words.end());
	matching_words.erase(std::unique(matching_words.begin(), matching_words.end()), matching_words.end());

	for (int i = 0; i < matching_words.size(); i++)
		ret += matching_words[i] + ' ';

	return ret;
}

std::string gui_query_contents::return_words_with_i_count(vector<std::string>& vect, int i) {
	std::string ret;
	vector<std::string> matching_words;
	for (auto temp : vect)
		if (temp.size() == i)
			matching_words.push_back(temp);

	std::sort(matching_words.begin(), matching_words.end());
	matching_words.erase(std::unique(matching_words.begin(), matching_words.end()), matching_words.end());

	for (int i = 0; i < matching_words.size(); i++)
		ret += matching_words[i] + ' ';

	return ret;
}

#endif