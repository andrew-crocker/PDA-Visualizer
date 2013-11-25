#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#ifndef _TEXTBOX_
#define _TEXTBOX_

#include "ColorPoint2.h"

class Textbox {
	// static unsigned int MAX_NUM_CHARS_IN_TEXTBOX;
	static Point2 DEFAULT_POSITION;
	static int DEFAULT_WIDTH, DEFAULT_HEIGHT;
	string textInBox, label;
	Point2 boundingBox;
	int width, height;
	bool overTextBox, clicked;
	unsigned int max_num_chars;
public:
	Textbox();
	Textbox(Point2, int, int);
	Textbox(istream &);

	void drawBox(int, int, int, int);
	void drawText(int, int, const char *);
	void draw();
	bool containsPoint(int, int) const;

	void setOverTextBox(bool);
	bool getOverTextBox();
	void setClicked(bool);
	bool getClicked();
	string getTextInBox();
	void setTextInBox(const char *);
	void addTextInBox(unsigned char);
	void deleteTextInBox();
	unsigned int getTextInBoxLength();
	unsigned int getMaxNumChars();
	void setMaxNumChars(unsigned int);
};

#endif