#include <math.h>
#include "../headers/Button.h"
using namespace std;

const int Button::DEFAULT_WIDTH = 0;
const int Button::DEFAULT_HEIGHT = 0;
const string Button::DEFAULT_LABEL = "";
const Color Button::DEFAULT_COLOR(0, 0, 0);
const Point2 Button::DEFAULT_POSITION(0, 0);


Button::Button() {
	// active_color = DEFAULT_COLOR;
	normal_color = DEFAULT_COLOR;
	hover_color = DEFAULT_COLOR;
	clicked_color = DEFAULT_COLOR;
	position = DEFAULT_POSITION;
	pre_clicked = false;
	clicked = false;
	hover = false;
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
	label = DEFAULT_LABEL;
}

Button::Button(Color c1, Color c2, Color c3, Point2 p, int w, int h, string text) {
	// active_color = c1;
	normal_color = c1;
	hover_color = c2;
	clicked_color = c3;
	position = p;
	pre_clicked = false;
	clicked = false;
	hover = false;
	width = w;
	height = h;
  	label = text;
}

Button::Button(istream &is) {
	char temp;
	temp = is.peek();
	if (temp == '~') {
		getline(is, label);		// skip line		
	}
	else {
		is >> normal_color.red >> normal_color.green >> normal_color.blue;
		is >> hover_color.red >> hover_color.green >> hover_color.blue;
		is >> clicked_color.red >> clicked_color.green >> clicked_color.blue;
		is >> position.x >> position.y;
		is >> width >> height;
		getline(is, label);		// removes excess white space at end of line
		temp = is.peek();
		if (temp == '~') {
			getline(is, label); // skip line
		}
		getline(is, label);
	}
	pre_clicked = false;
	clicked = false;
	hover = false;
	// active_color = normal_color;
}

// Color Button::getColor() const {
// 	return active_color;
// }

Point2 Button::getPosition() const {
	return position;
}

void Button::setPosition(int x, int y) {
	position.x = x;
	position.y = y;
}

// void Button::setColor(Color c) {
// 	color = c;
// }

// void Button::setColor(int r, int g, int b) {
// 	Color c(r, g, b);
// 	color = c;
// }

void Button::sendGLColor(Color c) {
	glColor3f(c.red/255.0, c.green/255.0, c.blue/255.0);
}

void Button::display() {
	cout << "----" << label << "----" << endl;
	normal_color.display("Normal Color: ");
	hover_color.display("Hover Color: ");
	clicked_color.display("Clicked Color: ");
	position.display();
	cout << "Width: " << width << endl;
	cout << "Height: " << height << endl;
	for (unsigned int i = 0; i < 8+label.length(); ++i)
		cout << "-";
	cout << endl << endl;
}

bool Button::containsPoint( int x, int y ) const {
	int minX = min( position.x, position.x + width );
	int maxX = max( position.x, position.x + width );
	int minY = min( position.y, position.y + height );
	int maxY = max( position.y, position.y + height );
	return ( x > minX && x < maxX && y > minY && y < maxY );
}

void Button::drawBox(int x, int y, int w, int h) {
	glBegin(GL_POLYGON);
    glVertex2f(x, y);  // upper left
    glVertex2f(x, y + h);  // lower left
    glVertex2f(x + w, y + h);  // lower right
    glVertex2f(x + w, y);  // upper right
	glEnd();
}

void Button::draw() {
	// draw the textbox
	glColor3f(.25, .25, .25);  // dark gray
	// sendGLColor(normal_color);
	drawBox(position.x, position.y, width, height);
	if (clicked) {
		// glColor3f(1,1,1);  // white
		sendGLColor(clicked_color);
		// if (pre_clicked) {
		// sendGLColor(clicked_color);
		// }
	}
	else if (hover) {
		// glColor3f(.75, .75, .75);  // light gray
		sendGLColor(hover_color);
	}
	else {
		sendGLColor(normal_color);
	}
	drawBox(position.x+5, position.y+5, width-10, height-10);
	drawText();
}

void Button::drawText() {
	glColor3f(0.0, 0.0, 0.0);

	char * text = new char [label.length()+1];
	strcpy (text, label.c_str());
    
    glRasterPos2f(position.x+width/2-(11*label.length()/2), position.y+height/2+8);
    int length = strlen(text);
    for (int i = 0; i < length; ++i)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
}

void Button::setHover(bool b) {
	// if (!clicked) {
		hover = b;
	// }
	// if (clicked)
	// 	active_color = clicked_color;
	// else
	// 	active_color = normal_color;
}

bool Button::getHover() {
	return hover;
}

void Button::setPreClicked(bool b) {
	pre_clicked = b;
	// if (clicked)
	// 	active_color = clicked_color;
	// else
	// 	active_color = normal_color;
	// cout << "You didn't click " << this->label << endl;
	// cout << this->clicked << endl;
}

bool Button::getPreClicked() {
	return pre_clicked;
}

void Button::setClicked(bool b) {
	clicked = b;
	// pre_clicked = !b;
	// active_color = clicked_color;
	// cout << "You clicked " << this->label << endl;	
	// cout << this->label << " Clicked: " << this->clicked << ", Hover: " << this->hover << endl;
}

bool Button::getClicked() {
	return clicked;
}

string Button::getLabel() {
	return label;
}

double distance ( int x, int y, Point2 p) {
	double dx = p.x - x, dy = p.y - y;
	return sqrt(dx * dx + dy * dy);
}