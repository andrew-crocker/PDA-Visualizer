#include <iostream>
#include "../headers/Circle.h"
#include <math.h>
using namespace std;

Circle::Circle() {
  color.red = 255;
  color.green = 148;
  color.blue = 90;
  position.x = position.y = 200;
  radius = 70;
  label = "";
}

Circle::Circle(Point2 p, Color c, int r, string l):Shape(p, c) {
  radius = r;
  label = l;
}

Circle::Circle(istream &is):Shape(is) {
  is >> radius;
}

void Circle::display() {
  cout << "Circle ";
  // Shape::display();
  cout << radius;
}

// the drawText function draws some text at location x, y
//   note:  the text to be drawn is a C-style string!
void Circle::drawText(int x, int y, const char *text, void * font) {
  glRasterPos2f( x, y );
  int length = strlen(text);
  for (int i = 0; i < length; i++)
      glutBitmapCharacter(font, text[i]);
}

void Circle::draw() {
  Shape::sendGLColor(color);
  const int NUM_DIVS = 50;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2d(position.x, position.y);
  for (int i = 0; i < NUM_DIVS; ++i)
    glVertex2d(position.x + radius*cos(i*2*M_PI/(NUM_DIVS-1)), position.y + radius*sin(i*2*M_PI/(NUM_DIVS-1)));
  drawText(position.x-(4*label.size()/2), position.y-10, label.c_str(), GLUT_BITMAP_9_BY_15);
  glEnd();
}

bool Circle::containsPoint(int x, int y) {
  double dx = x - position.x;
  double dy = y - position.y;
  bool a = (dx < radius);
  bool b = (dy < radius);
  bool c = (a && b);
  return c;
}

void Circle::update(int x, int y) {
  double dx = x - position.x;
  double dy = y - position.y;
  radius = sqrt(dx*dx + dy*dy);
}
