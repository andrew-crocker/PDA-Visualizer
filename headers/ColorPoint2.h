#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

#ifndef _ColorPoint2_
#define _ColorPoint2_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

struct Color {
	int red;
	int green;
	int blue;
	Color(int r = 0, int g = 0, int b = 0) {
		red = r;
		green = g;
		blue = b;
	}
	Color &operator=(const Color &c) {
		red = c.red;
		green = c.green;
		blue = c.blue;
		return *this;
	}
	void display(const char * id) {
		cout << id << red << " " << green << " " << blue << endl;
	}
};

struct Point2D {
	int x;
	int y;
	Point2D(int xPos = 0, int yPos = 0) {
		x = xPos;
		y = yPos;
	}
	Point2D &operator=(const Point2D &p) {
		x = p.x;
		y = p.y;
		return *this;
	}
	void display() {
		cout << "Position: " << x << " " << y << endl;
	}
};

struct Point2 {
	double x;
	double y;
	Point2(double xPos = 0, double yPos = 0) {
		x = xPos;
		y = yPos;
	}
	Point2 &operator=(const Point2 &p) {
		x = p.x;
		y = p.y;
		return *this;
	}
	void display() {
		cout << "Position: " << x << " " << y << endl;
	}
};


#endif