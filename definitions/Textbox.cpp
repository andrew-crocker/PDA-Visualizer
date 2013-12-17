#include "../headers/Textbox.h"

using namespace std;

// unsigned int Textbox::MAX_NUM_CHARS_IN_TEXTBOX = 10;
Point2 Textbox::DEFAULT_POSITION(0, 0);
int Textbox::DEFAULT_WIDTH = 0;
int Textbox::DEFAULT_HEIGHT = 0;


Textbox::Textbox() {
	textInBox = "";
	boundingBox = DEFAULT_POSITION;
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
	overTextBox = false;
	clicked = false;
	max_num_chars = (width-15)/9;
}

Textbox::Textbox(const Textbox &t) {
	textInBox = t.textInBox;
	boundingBox = t.boundingBox;
	width = t.width;
	height = t.height;
	overTextBox = t.overTextBox;
	clicked = t.clicked;
	max_num_chars = t.max_num_chars;
}

Textbox::Textbox(Point2 p, int w, int h) {
	textInBox = "";
	boundingBox = p;
	width = w;
	height = h;
	overTextBox = false;
	clicked = false;
	max_num_chars = (width-15)/9;
}

Textbox::Textbox(Point2 p, int w, int h, char new_text) {
	textInBox = new_text;
	textInBox += " ";
	boundingBox = p;
	width = w;
	height = h;
	overTextBox = false;
	clicked = false;
	max_num_chars = (width-15)/9;
}

Textbox::Textbox(istream &is) {
	textInBox = "";
	is >> boundingBox.x >> boundingBox.y >> width >> height >> label;
	if (label == "none")
		label = "";
	overTextBox = false;
	clicked = false;
	max_num_chars = (width-15)/9;
}

 // the following function draws a rectangle, given
//   the upper left vertex and the width and height
void Textbox::drawBox(int x, int y, int w, int h) {
	glBegin(GL_POLYGON);
    glVertex2f(x, y);  // upper left
    glVertex2f(x, y + h);  // lower left
    glVertex2f(x + w, y + h);  // lower right
    glVertex2f(x + w, y);  // upper right
	glEnd();
}

// the drawText function draws some text at location x, y
//   note:  the text to be drawn is a C-style string!
void Textbox::drawText(int x, int y, const char *text, void * font) {
	glRasterPos2f( x, y );
	int length = strlen(text);
	for (int i = 0; i < length; i++)
	    glutBitmapCharacter(font, text[i]);
}

void Textbox::draw() {
	// draw the textbox
	glColor3f(.25, .25, .25);  // dark gray
	drawBox(boundingBox.x, boundingBox.y, width, height);
	if (overTextBox || clicked)
		glColor3f(1,1,1);  // white
	else
		glColor3f(.75, .75, .75);  // light gray
	drawBox(boundingBox.x+5, boundingBox.y+5, width-10, height-10);
	glColor3f(0, 0, 0);  // black
	drawText(boundingBox.x+(width/2)-(4.5*label.length()), boundingBox.y-9, label.c_str(), GLUT_BITMAP_HELVETICA_18);
	if (clicked) { // draw with a cursor
		string withCursor(textInBox);
	    withCursor += '|';
	    drawText(boundingBox.x+8, boundingBox.y+height/2+4, withCursor.c_str(), GLUT_BITMAP_9_BY_15);
	}
	else
		drawText(boundingBox.x+8, boundingBox.y+height/2+4, textInBox.c_str(), GLUT_BITMAP_9_BY_15);
}

bool Textbox::containsPoint( int x, int y ) const {
	int minX = min( boundingBox.x, boundingBox.x + width );
	int maxX = max( boundingBox.x, boundingBox.x + width );
	int minY = min( boundingBox.y, boundingBox.y + height );
	int maxY = max( boundingBox.y, boundingBox.y + height );
	return ( x > minX && x < maxX && y > minY && y < maxY );
}

void Textbox::setOverTextBox(bool b) {
	overTextBox = b;
}

bool Textbox::getOverTextBox() {
	return overTextBox;
}

void Textbox::setClicked(bool b) {
	clicked = b;
}

bool Textbox::getClicked() {
	return clicked;
}

string Textbox::getTextInBox() {
	return textInBox;
}

void Textbox::setTextInBox(const char * text) {
	textInBox = text;
}

void Textbox::addTextInBox(unsigned char text) {
	textInBox += text;
}

void Textbox::deleteTextInBox() {
	textInBox.erase(textInBox.end()-1);
}

unsigned int Textbox::getTextInBoxLength() {
	return textInBox.length();
}

unsigned int Textbox::getMaxNumChars() {
	return max_num_chars;
}

void Textbox::setMaxNumChars(unsigned int m) {
	max_num_chars = m;
}

Textbox & Textbox::operator=(const Textbox t) {
	textInBox = t.textInBox;
	label = t.label;
	boundingBox = t.boundingBox;
	width = t.width;
	height = t.height;
	overTextBox = t.overTextBox;
	clicked = t.clicked;
	max_num_chars = t.max_num_chars;
	return *this;
}

Point2 Textbox::getPosition() {
	return boundingBox;
}

int Textbox::getWidth() {
	return width;
}

int Textbox::getHeight() {
	return height;
}