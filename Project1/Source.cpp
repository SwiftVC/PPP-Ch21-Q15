/*
*	Author:	CS
*	Date:	2022_03_22	YYYY_MM_DD
*
*	GUI file is used for the following queries:
*	- read user's file into buffer
*	- count user-defined string in passage
*	- output word(s) with the largest count
*	- output word(s) with the most characters
*	- output word(s) with the least characters
*	- output word(s) starting with the user-defined character sequence
*	- output word(s) with the user-defined number of characters
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include "CS_Text_Display.h"
#include "gui_query_contents.h"

int main() {
	Point tl{ 50,50 };
	gui_query_contents win{ tl, 1200,700, "Query text | Try: output.txt" };
	
	win.wait_for_button();

	return 0;
}

