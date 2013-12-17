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
  	while (ss >> tmp) variables.push_back(tmp);

  	// get terminals
  	getline(f, buff);
  	stringstream ss2(buff);
  	while (ss2 >> tmp) terminals.push_back(tmp);

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
  		while (ss3 >> s) productions[i].push_back(s);
  	} 
}

CFG::CFG(string v, string t, char ss, vector <string> p) {
	char tmp;
	stringstream ss1(v);
	while (ss1 >> tmp) variables.push_back(tmp);

	stringstream ss2(t);
	while (ss2 >> tmp) terminals.push_back(tmp);

	start_state = ss;

	productions.resize(p.size());
	for (int i=0; i < p.size(); i++) {
		string s;
		char buff;
		stringstream ss3(p[i]);
		ss3 >> buff;
		while (ss3 >> s) productions[i].push_back(s);
	}
}