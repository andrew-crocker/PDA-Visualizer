#include <iostream>
#include <math.h>
using namespace std;
#include "../headers/Shape.h"

extern double distance(int, int, Point2);

Color Shape::DEFAULT_COLOR(0, 0, 255);
Point2 Shape::DEFAULT_POSITION(100, 100);

Shape::Shape () {
	color = DEFAULT_COLOR;
	position = DEFAULT_POSITION;
}

Shape::Shape (Point2 p, Color c) {
	color = c;
	position = p;
}

Shape::Shape (istream &is ) {
	is >> color.red >> color.green >> color.blue;
	is >> position.x >> position.y;
}

Color Shape::getColor() const {
	return color;
}

Point2 Shape::getPosition() const {
	return position;
}

void Shape::setColor(Color c) {
	color = c;
}

void Shape::setColor(int r, int g, int b) {
	Color c(r, g, b);
	color = c;
}

void Shape::setPosition(int x, int y) {
	position.x = x;
	position.y = y;
}

void Shape::display() {
	// color.display();
	position.display();
}

void Shape::sendGLColor() {
	glColor3f(color.red/255.0, color.green/255.0, color.blue/255.0);
}

void Shape::sendGLColor(Color c) {
	glColor3f(c.red/255.0, c.green/255.0, c.blue/255.0);
}
