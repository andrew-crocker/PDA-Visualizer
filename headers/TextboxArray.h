#include <iostream>
using namespace std;

#ifndef _TextboxArray_
#define _TextboxArray_

#include "Textbox.h"

class TextboxArray {
	int numTextboxes;
	//int current_index;
	Textbox ** TboxArray;
public:
	TextboxArray();
	TextboxArray(const char *);
	~TextboxArray();
	TextboxArray & operator=(const TextboxArray);
	void draw();
	void handleKeyPress(unsigned char, int, int);
	void handleMousePress(int, int, int, int);
	void handleMouseMovement(int, int);
};

#endif