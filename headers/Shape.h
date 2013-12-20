#ifndef _Shape_
#define _Shape_
#include "ColorPoint2.h"

class Shape {
 protected:
 	static Color DEFAULT_COLOR;
 	static Point2 DEFAULT_POSITION;
  Color color;
  Point2 position;
 public:
  Shape();
  Shape(Point2 p, Color c);
  Shape(Point2 p);
  Shape(istream &is);
  Color getColor() const;
  Point2 getPosition() const;
  void setColor(Color c);
  void setColor(int, int, int);
  void setPosition(double x, double y);
  void display();
  void sendGLColor(Color color);
  void sendGLColor();
  virtual void draw() = 0;
  virtual void update(double x, double y) = 0;
  virtual bool containsPoint(int x, int y) = 0;
};

#endif // _Shape_
