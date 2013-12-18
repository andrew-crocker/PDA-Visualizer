// class to store information about the user-entered CFG

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#ifndef _CFG_
#define _CFG_

struct RPP {
	char read;
	char pop;
	char push;
	RPP(char r, char p1, char p2) {
		read = r;
		pop = p1;
		push = p2;
	}
};

class CFG {
	vector<char> variables;
	vector<char> terminals;
	vector< vector<string> > productions;
public:
	char start_state;
	int good;
	int pda;
	string error;
	// making these vectors public for convenience
	vector<RPP> one;
	vector< vector<RPP> > two;   // size two
	vector< vector<RPP> > three; // size three
	vector< vector<RPP> > four;  // size four
	CFG(const char * filename);
	CFG(string v, string t, const char ss, vector <string> p);
	int save(string filename);
	int isGood();
	int validChar(char c);
	void makeStructure();
};

#endif