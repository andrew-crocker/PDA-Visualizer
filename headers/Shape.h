#ifndef _SHAPE_
#define _SHAPE_

#include <fstream>
#include <GL/glut.h>
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
	Shape(istream &is);

	Color getColor() const;
	Point2 getPosition() const;

	void setColor(Color c);
	void setColor(int r, int g, int b);
	void setPosition(int x, int y);
	virtual void display();
	virtual bool containsPoint(int, int) const =0;
	virtual void draw() =0;
	virtual void drawText() =0;
	virtual void setHoverTrue() =0;
	virtual void setHoverFalse() =0;
	virtual bool getHover() =0;
	virtual void setClickedTrue() =0;
	virtual void setClickedFalse() =0;
	void sendGLColor();
};

#endif
