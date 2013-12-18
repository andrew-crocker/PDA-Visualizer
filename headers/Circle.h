#ifndef _Circle_
#define _Circle_
#include "Shape.h"
#include "math.h"

class Circle: public Shape {
  int radius;
 public:
  Circle();
  Circle(Point2 p, Color c, int r, string);
  Circle(istream &is);
  string label;
  void display();
  void drawText(int, int, const char *, void *);
  void draw();
  virtual bool containsPoint(int x, int y);
  virtual void update(int x, int y);
};

#endif // _Circle_
