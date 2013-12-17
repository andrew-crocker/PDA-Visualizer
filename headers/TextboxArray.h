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
	TextboxArray(Textbox **, int);
	~TextboxArray();
	TextboxArray & operator=(const TextboxArray);
	// TextboxArray *& operator+=(const TextboxArray);
	void draw();
	int handleKeyPress(unsigned char, int, int);
	void handleMousePress(int, int, int, int);
	void handleMouseMovement(int, int);
	int getNumTextboxes();
	string getText(int);
	void addTextbox(Textbox &);
	void removeTextbox();
	int getClicked(int);
	Point2 getPosition(int);
	int getWidth(int);
	int getHeight(int);
};

#endif