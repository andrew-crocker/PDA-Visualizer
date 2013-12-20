#ifndef _Circle_
#define _Circle_
#include "Shape.h"
#include "math.h"

class Circle: public Shape {
  double radius;
 public:
  Circle();
  Circle(Point2 p, Color c, double r, string);
  Circle(Point2 p, double r, string);
  Circle(istream &is);
  string label;
  void display();
  void drawText(int, int, const char *, void *);
  void draw();
  virtual bool containsPoint(int x, int y);
  virtual void update(double x, double y);
  Point2 getPosition();
  void setPosition(double, double);
  double getRadius();
  void changeRadius(double);
  void setRadius(double);
};

#endif // _Circle_
