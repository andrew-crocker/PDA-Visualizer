#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#ifndef _BUTTON_
#define _BUTTON_

#include "ColorPoint2.h"
#include "texture.h"

class Button {
	static const int DEFAULT_WIDTH, DEFAULT_HEIGHT;
	static const string DEFAULT_LABEL;
	static const Color DEFAULT_COLOR;
	static const Point2 DEFAULT_POSITION;
	// Color active_color;
	Color normal_color;
	Color hover_color;
	Color clicked_color;
	Point2 position;
	bool pre_clicked, clicked, hover;
	int width, height;
	string label;
public:
	Button();
	Button(Color, Color, Color, Point2, int, int, string);
	Button(istream &is);

	// Color getColor() const;
	Point2 getPosition() const;
	// void setColor(Color c);
	// void setColor(int r, int g, int b);
	void setPosition(int x, int y);
	void sendGLColor(Color);

	void display();
	bool containsPoint(int x, int y) const;
	void drawBox(int, int, int, int);
	void draw();
	void drawText();
	void setHover(bool);
	bool getHover();
	void setPreClicked(bool);
	bool getPreClicked();
	void setClicked(bool);
	bool getClicked();
	string getLabel();
};

#endif
