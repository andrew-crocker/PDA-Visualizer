// class to store information about the user-entered CFG

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#ifndef _CFG_
#define _CFG_

class CFG {
	vector<char> variables;
	vector<char> terminals;
	char start_state;
	vector< vector<string> > productions;
public:
	CFG(const char *filename);
	CFG(string v, string t, char ss, vector <string> p);
};

#endif