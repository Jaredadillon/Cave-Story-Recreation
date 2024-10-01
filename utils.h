#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Utils {
public:

	//split a string <txt> everywhere a particular <ch> character is found
	static unsigned int split(const string& txt, vector<string>& strs, char ch) {
		int pos = txt.find(ch);
		int initialPos = 0;
		strs.clear();
		//take that character and store those substrings in a vector <strs>
		while (pos != string::npos) {
			strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
			//resetting positions for the next iteration
			initialPos = pos + 1;
			pos = txt.find(ch, initialPos);
		}
		//add the very last grouping in the xml node
		strs.push_back(txt.substr(initialPos, min<int>(pos, txt.size() - (initialPos + 1))));
		return strs.size();
	}
};


#endif