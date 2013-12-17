#include "../headers/CFG.h"

#include <sstream>

CFG::CFG(const char *filename) {
	ifstream f(filename);
	if( !f.good() ) {
		cerr << "Warning: Unable to open " << filename << ", ignoring it." << endl;
		return;
  	}
  	
  	string buff;
	char tmp;

  	// get the variables
  	getline(f, buff);
  	stringstream ss(buff);
  	while (ss >> tmp) 
  		variables.push_back(tmp);

  	// get terminals
  	getline(f, buff);
  	stringstream ss2(buff);
  	while (ss2 >> tmp) 
  		terminals.push_back(tmp);

  	// get start state
  	f >> start_state;

  	// get productions
  	productions.resize(variables.size());
  	
  	for (int i=0; !f.eof(); i++) {
  		string s;
  		char buff;
  		getline(f, s);
  		stringstream ss3(s);
  		ss3 >> buff;
  		while (ss3 >> s) 
  			productions[i].push_back(s);
  	} 

  	f.close();

  	if (this.good()) 
  		this.makeStructure();
}

CFG::CFG(string v, string t, const char ss, vector <string> p) {
	char tmp;
	stringstream ss1(v);
	while (ss1 >> tmp) 
		variables.push_back(tmp);

	stringstream ss2(t);
	while (ss2 >> tmp) 
		terminals.push_back(tmp);

	start_state = ss;

	productions.resize(p.size());
	for (int i=0; i < p.size(); i++) {
		string s;
		char buff;
		stringstream ss3(p[i]);
		ss3 >> buff;
		while (ss3 >> s) 
			productions[i].push_back(s);
	}

	if (this.good()) 
		this.makeStructure();
}

int CFG::save(string filename) {
	ofstream f(filename.c_str());
	if ( !f.good() ) {
		cerr << "Warning: Unable to open " << filename << ", ignoring it." << endl;
		return 0;
	}
	// write the variables to the file
	for (int i=0; i < variables.size(); i++) {
		f << variables[i];
		if (i < variables.size()-1)
			f << ' ';
	}
	f << '\n';

	// write the terminals to the file
	for (int i=0; i < terminals.size(); i++) {
		f << terminals[i];
		if (i < terminals.size()-1)
			f << ' ';
	}
	f << '\n';

	// write the start state to the file
	f << start_state;
	f << '\n';

	// write the productions to the file
	for (int i=0; i < productions.size(); i++) {
		f << variables[i];
		f << ' ';
		for (int j=0; j < productions[i].size(); j++) {
			f << productions[i][j];
			if (j < productions[i].size()-1)
				f << ' ';
		}
		if (i < productions.size()-1)
			f << '\n';
	}

	f.close();

	return 1;
}

int CFG::good() {
	// check if any of the productions are empty
	for (int i=0; i < productions.size(); i++) {
		if (productions[i].empty()) {
			cerr << "Error: There are no productions for " << variables[i] << "." << endl;
			return 0;
		}
	}
	// no capital E's in variables
	for (int i=0; i < variables.size(); i++) {
		if (variables[i] == 'E') {
			cerr << "Error: 'E' cannot be used as a variable." << endl;
			return 0;
		}
	}
	// check if start state exists in variables
	bool in = false;
	for (int i=0; i < variables.size(); i++) {
		if (variables[i] == start_state) {
			in = true;
			break;
		}
	}
	if (!in) {
		cerr << "Error!: The start state was not entered as a variable." << endl;
		return 0;
	}
	// check if any of the characters in productions aren't a variable, terminal, or E
	for (int i=0; i < productions.size(); i++) {
		for (int j=0; j < productions[i].size(); j++) {
			for (int k=0; k < productions[i][j].size(); k++) {
				char c = productions[i][j][k];
				if (!validChar(c)) {
					cerr << "Error: " << c << " is not a valid character." << endl;
					return 0;
				}
			}
		}
	}

	return 1;
}

int CFG::validChar(char c) {
	// check for epsilon character
	if (c == 'E')
		return 1;
	// check if it's a variable
	for (int i=0; i < variables.size(); i++)
		if (c == variables[i])
			return 1;
	// check if it's a terminal
	for (int i=0; i < terminals.size(); i++)
		if (c == terminals[i])
			return 1;

	return 0;
}

void CFG::makeStructure() {
	// each of the levels represents one transition (edge) between nodes
	two.resize(2);
	three.resize(3);
	four.resize(4);

	// fill vectors for productions
	for (int i=0; i < productions.size(); i++) {
		for (int j=0; j < productions[i].size(); j++) {
			switch(productions[i][j].size()) {
				case(1):
					RPP rpp('E', variables[i], productions[i][j][0]);
					one.push_back(rpp);
					break;
				case(2):
					RPP rpp('E', variables[i], productions[i][j][0]);
					two[0].push_back(rpp);
					RPP rpp2('E', 'E', productions[i][j][1]);
					two[1].push_back(rpp2);
					break;
				case(3):
					RPP rpp('E', variables[i], productions[i][j][0]);
					three[0].push_back(rpp);
					RPP rpp2('E', 'E', productions[i][j][1]);
					three[1].push_back(rpp2);
					RPP rpp3('E', 'E', productions[i][j][2]);
					three[2].push_back(rpp3);
					break;
				case(4):
					RPP rpp('E', variables[i], productions[i][j][0]);
					four[0].push_back(rpp);
					RPP rpp2('E', 'E', productions[i][j][1]);
					four[1].push_back(rpp2);
					RPP rpp3('E', 'E', productions[i][j][2]);
					four[2].push_back(rpp3);
					RPP rpp4('E', 'E', productions[i][j][3]);
					four[3].push_back(rpp4);
					break;
			}
		}
	}
	// make sure to add terminal productions
	for (int i=0; i < terminals.size(); i++) {
		RPP rpp(terminals[i], terminals[i], 'E');
		one.push_back(rpp);
	}
}

