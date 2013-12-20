#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "headers/Window.h"
#include "headers/CFG.h"
#include "headers/Circle.h"

#define PI 3.14159265358979323846

using namespace std;

bool mouseIsDragging = false;

int numWindows;
Window ** WinArray;
int num_new_textboxes = 0;
Textbox * new_textboxes[6];
CFG * cfg;
Circle ** circs;
Circle ** circs2;
int num_circles;
bool circle_clicked = 0;
int clicked_circle = -1;
int global_x, global_y;

void createNewWindow(Window &, Window);


double distance (int x2, int y2, Point2 p) {
	double dx = p.x - x2;
	double dy = p.y - y2;
	// cout << sqrt(dx * dx + dy * dy) << endl;
	return sqrt(dx * dx + dy * dy);
}

// the drawText function draws some text at location x, y
//   note:  the text to be drawn is a C-style string!
void drawsText(int x, int y, const char *text, void * font) {
	glRasterPos2f( x, y );
	int length = strlen(text);
	for (int i = 0; i < length; i++)
	    glutBitmapCharacter(font, text[i]);
}

void loadWindows(const char * filename ) {
  ifstream g (filename);
  if( !g.good() ) {
	cerr << "Warning: Unable to open " << filename << ", ignoring it." << endl;
	return;
  }
  g >> numWindows;
  WinArray = new Window* [numWindows];
  for(int i = 0; i < numWindows; i++) {
	WinArray[i] = new Window(g);
	g.get();
  } 
  g.close();
}

void exitAll() {
	for (int i = 0; i < numWindows; ++i) {
		if (WinArray[i]->get_current_id() != 0) {
			glutDestroyWindow(WinArray[i]->get_current_id());
		}
	}
	exit(0);
}

void mouseCoords2(int &x, int&y, int w2, int h2) {
  int w = glutGet(GLUT_WINDOW_WIDTH);
  int h = glutGet(GLUT_WINDOW_HEIGHT);
  float ratiox = (float)w/(float)w2;
  float ratioy = (float)h/(float)h2;
  x/=ratiox;
  y/=ratioy;
}

void mouseCoords(int &x, int&y)
{	int win = glutGetWindow();
	for (int i = 0; i < numWindows; ++i) {
		if (win == WinArray[i]->get_current_id()) {
			mouseCoords2(x, y, WinArray[i]->get_width(), WinArray[i]->get_height());
		}
	}
}

void drawWindow0() {
	WinArray[0]->drawWindow(1);
}

void drawWindow1() {
	WinArray[1]->drawWindow(1);
}

void drawWindow2() {
	WinArray[2]->drawWindow(1);
}

void drawWindow3() {
	WinArray[3]->drawWindow(1);
}

void drawWindow4() {
	WinArray[4]->drawWindow(1);
}

void drawProductions(vector <RPP> rpp, Point2 p) {
	string buff;
	for (int i=0; i < rpp.size(); i++) {
		buff = rpp[i].read;
		buff += ',';
		buff += rpp[i].pop;
		buff += " -> ";
		buff += rpp[i].push;
		drawsText(p.x, p.y+(19*i), buff.c_str(), GLUT_BITMAP_HELVETICA_18);
	}
}

void drawTriangle(double x, double y, Point2 p) {
	double x2, y2, x3, y3;
	if (x == p.x) {
		x2 = x-9;
		x3 = x+9;
		if (p.y > y)
			y2 = y3 = y-18;
		else
			y2 = y3 = y+18;

	}
	else if (y == p.y) {
		y2 = y-9;
		y3 = y+9;
		if (p.x > x)
			x2 = x3 = x-18;
		else
			x2 = x3 = x+18;
	}
	else {
		double dx, dy;
		dx = (double)p.x - x;
		dy = (double)p.y - y;
		double slope = dy / dx;
		double temp = x;
		double wumbo;
		// cout << slope << " " << y << " " << endl;
		if (p.x < x) {
			wumbo = y+slope*((temp+=.01)-x);
			while ((distance(temp, wumbo, Point2(x, y))) < 18.0) {
				// cout << "mumbo: " << wumbo << endl;
				wumbo = y+slope*((temp+=.01)-x);
			}
		}
		else {
			wumbo = y+slope*((temp-=.01)-x);
			while ((distance(temp, wumbo, Point2(x, y))) < 18.0) {
				wumbo = y+slope*((temp-=.01)-x);
			}
		}
		// wumbo == y, temp == x;
		slope *= -1;
		slope = 1 / slope;

		double temp2 = temp;
		double dumbo = wumbo+slope*((temp2+=.01)-temp);
		while ((distance(temp2, dumbo, Point2(temp, wumbo))) < 9) {
			dumbo = wumbo+slope*((temp2+=.01)-temp);
		}
		x2 = temp2;
		y2 = dumbo;

		temp2 = temp;
		dumbo = wumbo+slope*((temp2-=.01)-temp);
		while ((distance(temp2, dumbo, Point2(temp, wumbo))) < 9) {
			dumbo = wumbo+slope*((temp2-=.01)-temp);
		}
		x3 = temp2;
		y3 = dumbo;

	}
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glEnd();
}

Point2 drawArc(Point2 p1, Point2 p2, Circle c, int magnitude) {
    // glClear (GL_COLOR_BUFFER_BIT);
    // glColor3f (1.0, 1.0, 0.0);
    bool in_itself = 0;
    if (c.containsPoint(p1.x, p1.y)) {
    	in_itself = 1;
    }
    double tempx, tempy, t1, t2;
    double dx, dy;
	dy = p2.y - p1.y;
	dx = p2.x - p1.x;
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 1000; ++i) {
    	double temp = (double)i/1000;
    	double y = p1.y + ((double)i/1000)*dy;
    	double x = p1.x + ((double)i/1000)*dx;
    	if (i == 500) {
    		tempx = x;
    		tempy = y+magnitude*sin((temp)*PI);
    	}
    	t1 = x;
    	t2 = y+magnitude*sin((temp)*PI);
    	if (c.containsPoint(t1, t2) && !in_itself) {
    		glEnd();
    		glFlush();     //forces previously issued commands to execute
    		drawTriangle(t1, t2, p2);
    		break;
    	}
    	else if (in_itself) {
    		if (!c.containsPoint(t1, t2)) {
    			in_itself = 0;
    		}
    	}
    	else {
    		// poop
    	}

    	// cout << x << endl;
    	//cout << sin((i/50)*PI) << endl;
    	glVertex2f(x, t2);
    	glVertex2f(x, t2+2);
    }
        // glVertex2f(100.0,100.0);      //f means floating point or those with decimals
        // glVertex2f(400.0,100.0);
        // glVertex2f(400.0, 500.0);

    return Point2(tempx-30, tempy);
}

void drawWindow5() {
	WinArray[5]->drawWindow(0);
	if (!cfg->good) {
		drawsText(130, 350, cfg->error.c_str(), GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else {
		// draw text for q_s loop
		// draw start state transition
		string s = "E,E -> ";
		s += cfg->start_state;
		// draw productions with only one transition
		// // now draw productions with two transitions

		Point2 temp = circs[1]->getPosition();
		temp = drawArc(Point2(temp.x-35, temp.y), Point2(temp.x+35, temp.y), *circs[1], 80);
		drawProductions(cfg->one, temp);

		
		temp = drawArc(circs[0]->getPosition(), circs[1]->getPosition(), *circs[1], 0);
		drawsText(temp.x, temp.y, s.c_str(), GLUT_BITMAP_HELVETICA_18);
		
		temp = drawArc(circs[1]->getPosition(), circs[2]->getPosition(), *circs[2], 0);
		drawsText(temp.x, temp.y, "E,delta -> E", GLUT_BITMAP_HELVETICA_18);
		
		temp = circs[0]->getPosition();
		temp = drawArc(Point2(temp.x-25, temp.y), Point2(temp.x+25, temp.y), *circs[0], -75);
		drawsText(temp.x, temp.y, "E,E -> delta", GLUT_BITMAP_HELVETICA_18);

		if (num_circles == 3) {

		}
		else if (num_circles == 4) {
			temp = drawArc(circs[1]->getPosition(), circs[3]->getPosition(), *circs[3], 35);
			drawProductions(cfg->two[0], temp);
			temp = drawArc(circs[3]->getPosition(), circs[1]->getPosition(), *circs[1], -35);
			drawProductions(cfg->two[1], temp);
		}
		else if (num_circles == 5) {
			temp = drawArc(circs[1]->getPosition(), circs[3]->getPosition(), *circs[3], 0);
			drawProductions(cfg->three[0], temp);
			temp = drawArc(circs[3]->getPosition(), circs[4]->getPosition(), *circs[4], 0);
			drawProductions(cfg->three[1], temp);
			temp = drawArc(circs[4]->getPosition(), circs[1]->getPosition(), *circs[1], 0);
			drawProductions(cfg->three[2], temp);
		}
		else if (num_circles == 6) {
			if (!cfg->four[0].empty()) {
				temp = drawArc(circs[1]->getPosition(), circs[3]->getPosition(), *circs[3], 0);
				drawProductions(cfg->four[0], temp);
				temp = drawArc(circs[3]->getPosition(), circs[4]->getPosition(), *circs[4], 0);
				drawProductions(cfg->four[1], temp);
				temp = drawArc(circs[4]->getPosition(), circs[5]->getPosition(), *circs[5], 0);
				drawProductions(cfg->four[2], temp);
				temp = drawArc(circs[5]->getPosition(), circs[1]->getPosition(), *circs[1], 0);
				drawProductions(cfg->four[3], temp);
			}
			else {
			temp = drawArc(circs[1]->getPosition(), circs[3]->getPosition(), *circs[3], 35);
			drawProductions(cfg->two[0], temp);
			temp = drawArc(circs[3]->getPosition(), circs[1]->getPosition(), *circs[1], -35);
			drawProductions(cfg->two[1], temp);

			temp = drawArc(circs[1]->getPosition(), circs[4]->getPosition(), *circs[4], 0);
			drawProductions(cfg->three[0], temp);
			temp = drawArc(circs[4]->getPosition(), circs[5]->getPosition(), *circs[5], 0);
			drawProductions(cfg->three[1], temp);
			temp = drawArc(circs[5]->getPosition(), circs[1]->getPosition(), *circs[1], 0);
			drawProductions(cfg->three[2], temp);
			}
		}
		else if (num_circles == 7) {
			temp = drawArc(circs[1]->getPosition(), circs[3]->getPosition(), *circs[3], 35);
			drawProductions(cfg->two[0], temp);
			temp = drawArc(circs[3]->getPosition(), circs[1]->getPosition(), *circs[1], -35);
			drawProductions(cfg->two[1], temp);

			temp = drawArc(circs[1]->getPosition(), circs[4]->getPosition(), *circs[4], 0);
			drawProductions(cfg->four[0], temp);
			temp = drawArc(circs[4]->getPosition(), circs[5]->getPosition(), *circs[5], 0);
			drawProductions(cfg->four[1], temp);
			temp = drawArc(circs[5]->getPosition(), circs[6]->getPosition(), *circs[6], 0);
			drawProductions(cfg->four[2], temp);
			temp = drawArc(circs[6]->getPosition(), circs[1]->getPosition(), *circs[1], 0);
			drawProductions(cfg->four[3], temp);
		}
		else if (num_circles == 8) {
			temp = drawArc(circs[1]->getPosition(), circs[3]->getPosition(), *circs[3], 0);
			drawProductions(cfg->three[0], temp);
			temp = drawArc(circs[3]->getPosition(), circs[4]->getPosition(), *circs[4], 0);
			drawProductions(cfg->three[1], temp);
			temp = drawArc(circs[4]->getPosition(), circs[1]->getPosition(), *circs[1], 0);
			drawProductions(cfg->three[2], temp);

			temp = drawArc(circs[1]->getPosition(), circs[5]->getPosition(), *circs[5], 0);
			drawProductions(cfg->four[0], temp);
			temp = drawArc(circs[5]->getPosition(), circs[6]->getPosition(), *circs[6], 0);
			drawProductions(cfg->four[1], temp);
			temp = drawArc(circs[6]->getPosition(), circs[7]->getPosition(), *circs[7], 0);
			drawProductions(cfg->four[2], temp);
			temp = drawArc(circs[7]->getPosition(), circs[1]->getPosition(), *circs[1], 0);
			drawProductions(cfg->four[3], temp);
		}
		else if (num_circles == 9) {
			temp = drawArc(circs[1]->getPosition(), circs[3]->getPosition(), *circs[3], 35);
			drawProductions(cfg->two[0], temp);
			temp = drawArc(circs[3]->getPosition(), circs[1]->getPosition(), *circs[1], -35);
			drawProductions(cfg->two[1], temp);

			temp = drawArc(circs[1]->getPosition(), circs[4]->getPosition(), *circs[4], 0);
			drawProductions(cfg->three[0], temp);
			temp = drawArc(circs[4]->getPosition(), circs[5]->getPosition(), *circs[5], 0);
			drawProductions(cfg->three[1], temp);
			temp = drawArc(circs[5]->getPosition(), circs[1]->getPosition(), *circs[1], 0);
			drawProductions(cfg->three[2], temp);

			temp = drawArc(circs[1]->getPosition(), circs[6]->getPosition(), *circs[6], 0);
			drawProductions(cfg->four[0], temp);
			temp = drawArc(circs[6]->getPosition(), circs[7]->getPosition(), *circs[7], 0);
			drawProductions(cfg->four[1], temp);
			temp = drawArc(circs[7]->getPosition(), circs[8]->getPosition(), *circs[8], 0);
			drawProductions(cfg->four[2], temp);
			temp = drawArc(circs[8]->getPosition(), circs[1]->getPosition(), *circs[1], 0);
			drawProductions(cfg->four[3], temp);
		}

		for (int i = 0; i < num_circles; ++i) {
			circs2[i]->draw();
			circs[i]->draw();
		}

	}
	glutSwapBuffers();
}

void drawWindow6() {
	WinArray[6]->drawWindow(1);
}

void drawWindow7() {
	WinArray[7]->drawWindow(1);
}

void drawWindow8() {
	WinArray[8]->drawWindow(1);
}

void drawWindow9() {
	WinArray[9]->drawWindow(1);
}

void drawWindow10() {
	WinArray[10]->drawWindow(1);
}

void drawWindow11() {
	WinArray[11]->drawWindow(1);
}

void handleSpecialKeypress(int key, int x, int y) {
	int x2 = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int y2 = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	// x2 /= 2;
	// y2 /= 2;
	// cout << x2 << " " << y2 << endl;

	switch (key) {
		case GLUT_KEY_UP:
			// cout << "zooming in" << endl;
			for (int i = 0; i < num_circles; ++i) {
				double y1 = circs[i]->getPosition().y;
				double x1 = circs[i]->getPosition().x;
				double dx = x2 - x1;
				double dy = y2 - y1;

				if (labs(dx) > 3)
					x1 -= dx-(dx * 0.9);
				if (labs(dy) > 3)
					y1 -= dy-(dy * 0.9);

				circs2[i]->update(x1, y1);
				circs2[i]->changeRadius(1/0.9);
				circs[i]->update(x1, y1);
				circs[i]->changeRadius(1/0.9);
			}
			break;
		case GLUT_KEY_DOWN:
			// cout << "zooming out" << endl;
			for (int i = 0; i < num_circles; ++i) {
				double y1 = circs[i]->getPosition().y;
				double x1 = circs[i]->getPosition().x;
				double dx = x2 - x1;
				double dy = y2 - y1;

				if (labs(dx) > 3)
					x1 += dx-(dx * 0.9);
				if (labs(dy) > 3)
					y1 += dy-(dy * 0.9);

				circs2[i]->update(x1, y1);
				circs2[i]->changeRadius(0.9);
				circs[i]->update(x1, y1);
				circs[i]->changeRadius(0.9);
			}
			break;
		case GLUT_KEY_LEFT:
			//nothing yet
			break;
		case GLUT_KEY_RIGHT:
			//nothing yet
			break;
		default:
			//nothing yet
			break;
	}
	glutPostRedisplay();
}

void keyboard( unsigned char c, int x, int y ) {
	int current = 0;
	int win = glutGetWindow();
	for (current = 0; current < numWindows; ++current) {
		if (win == WinArray[current]->get_current_id()) {
			break;
		}
	}
	int textbox_control = WinArray[current]->t_box_keypress(c, x, y);
	if (textbox_control) {
		if (current == 3) { // CFG Entry window
			if (WinArray[3]->get_textbox_clicked(0)) {
				char s = WinArray[3]->get_new_text_in_box(0);
				// cout << "s: " << (int)s << endl;
				if (textbox_control == -1) {
					WinArray[3]->remove_textbox();
					if (num_new_textboxes > 0)
						--num_new_textboxes;
				}
				else if (s != 0 && s != 32) { // s is a new character that isn't a space
					if (num_new_textboxes == 0) {
						Point2 p(335, 240);
						int wid = 200;
						int hei = 45;
						new_textboxes[num_new_textboxes] = new Textbox(p, wid, hei, s);
						WinArray[3]->add_new_textbox(*new_textboxes[num_new_textboxes]);
						++num_new_textboxes;
						// cout << s << endl;
					}
					else if (num_new_textboxes < 6) {
						Point2 p = WinArray[3]->get_previous_textbox_position();
						int wid = WinArray[3]->get_previous_textbox_width();
						int hei = WinArray[3]->get_previous_textbox_height();
						p.y += 46;
						new_textboxes[num_new_textboxes] = new Textbox(p, wid, hei, s);
						WinArray[3]->add_new_textbox(*new_textboxes[num_new_textboxes]);
						++num_new_textboxes;
						// cout << s << endl;
					}
				}
			}
		}
	}
	else {
		switch(c) {
		case 'q':
		case 'Q':
		case 27:
		
			// WinArray[current]->undraw();
			if (!WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[0]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (WinArray[1]->get_exists()) {
				WinArray[1]->undraw();
				createNewWindow(*WinArray[1], *WinArray[0]);
			}


			exitAll();
			break;
		default:
			break;
		}
	}
	glutPostRedisplay();
}
 // start screen
void mouse0(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	// cout << "0" << endl;
	WinArray[0]->t_box_mousepress(mouseButton, state, x, y);
	// cout << "1" << endl;
	if (GLUT_LEFT_BUTTON == mouseButton ) {
	// cout << "\t2" << endl;
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[0]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
		// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[0]->get_clicked(); // See if something was clicked
			// They want to quit, and they haven't done it already
			WinArray[0]->reset_clicks();
			if (button_id == "Quit" && !WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[0]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (button_id == "Quit" && WinArray[1]->get_exists()) {
				WinArray[1]->undraw();
				createNewWindow(*WinArray[1], *WinArray[0]);
			}
			// User wants to access the Help screen
			else if (button_id == "Help") {
				WinArray[7]->set_previous_id(0); // previous window is start window
				WinArray[0]->undraw();
				createNewWindow(*WinArray[7], *WinArray[0]);
			}
			// User wants to create a PDA
			else if (button_id == "Create") {
				// cout << "Create1" << endl;
				WinArray[0]->undraw();
				// cout << "Create2" << endl;
				createNewWindow(*WinArray[2], *WinArray[0]);
			}
			else {
				// cout << glutGetWindow() << endl;
				// They clicked somewhere else on the screen
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}

// quit screen
void mouse1(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[1]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[1]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
		// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[1]->get_clicked();
			WinArray[1]->reset_clicks();
			if (button_id == "Quit") {
				exitAll();
			}
			else if (button_id == "Back") {
				WinArray[1]->set_exists(0);
				WinArray[1]->undraw();
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}

// new or load screen
void mouse2(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[2]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[2]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
		// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[2]->get_clicked();
			WinArray[2]->reset_clicks();
			// They want to quit, and they haven't done it already
			if (button_id == "Quit" && !WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[2]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (button_id == "Quit" && WinArray[1]->get_exists()) {
				WinArray[1]->undraw();
				createNewWindow(*WinArray[1], *WinArray[2]);
			}
			else if (button_id == "Back") {
				// cout << "Back" << endl;
				WinArray[2]->undraw();
				// cout << "Back" << endl;
				createNewWindow(*WinArray[0], *WinArray[2]);
			}
			else if (button_id == "Help") {
				WinArray[8]->set_previous_id(2); // previous window is start window
				WinArray[2]->undraw();
				createNewWindow(*WinArray[8], *WinArray[2]);
			}
			else if (button_id == "New") {
				WinArray[2]->undraw();
				createNewWindow(*WinArray[3], *WinArray[2]);
			}
			else if (button_id == "Load") {
				WinArray[2]->undraw();
				createNewWindow(*WinArray[4], *WinArray[2]);
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}

// called after user clicks 'go' from cfg entry screen or load file screen
void make_circles() {
	num_circles = 3;
	if (!cfg->two[0].empty())
		num_circles+=1;
	if (!cfg->three[0].empty())
		num_circles+=2;
	if (!cfg->four[0].empty())
		num_circles+=3;

	circs = new Circle*[num_circles];
	circs2 = new Circle*[num_circles];
	
	circs[0] = new Circle(Point2(75, 350), 40, "Qstart");
	circs[1] = new Circle(Point2(350, 350), 50, "Qloop");
	circs[2] = new Circle(Point2(120, 500), 40, "Qaccept");
	circs2[0] = new Circle(Point2(75, 350), Color(0, 0, 0), 42, "");
	circs2[1] = new Circle(Point2(350, 350), Color(0, 0, 0), 52, "");
	circs2[2] = new Circle(Point2(120, 500), Color(0, 0, 0), 42, "");
	if (num_circles == 3) {
		circs[2] = new Circle(Point2(610, 350), 40, "Qaccept");
		circs2[2] = new Circle(Point2(610, 350), Color(0, 0, 0), 42, "");
	}
	else if (num_circles == 4) {
		circs[2] = new Circle(Point2(350, 165), 40, "Qaccept");
		circs2[2] = new Circle(Point2(350, 165), Color(0, 0, 0), 42, "");
		circs[3] = new Circle(Point2(625, 350), 25, "Q1");
		circs2[3] = new Circle(Point2(625, 350), Color(0, 0, 0), 27, "");
	}
	else if (num_circles == 5) {
		circs[2] = new Circle(Point2(350, 165), 40, "Qaccept");
		circs2[2] = new Circle(Point2(350, 165), Color(0, 0, 0), 42, "");
		circs[3] = new Circle(Point2(570, 260), 25, "Q1");
		circs[4] = new Circle(Point2(570, 455), 25, "Q2");
		circs2[3] = new Circle(Point2(570, 260), Color(0, 0, 0), 27, "");
		circs2[4] = new Circle(Point2(570, 455), Color(0, 0, 0), 27, "");
	}
	else if (num_circles == 6) {
		if (!cfg->four[0].empty()) {
			circs[2] = new Circle(Point2(350, 165), 40, "Qaccept");
			circs2[2] = new Circle(Point2(350, 165), Color(0, 0, 0), 42, "");
			circs[3] = new Circle(Point2(530, 220), 25, "Q1");
			circs[4] = new Circle(Point2(650, 350), 25, "Q2");
			circs[5] = new Circle(Point2(530, 490), 25, "Q3");
			circs2[3] = new Circle(Point2(530, 220), Color(0, 0, 0), 27, "");
			circs2[4] = new Circle(Point2(650, 350), Color(0, 0, 0), 27, "");
			circs2[5] = new Circle(Point2(530, 490), Color(0, 0, 0), 27, "");
		}
		else {
			circs[2] = new Circle(Point2(235, 155), 40, "Qaccept");
			circs2[2] = new Circle(Point2(235, 155), Color(0, 0, 0), 42, "");
			circs[3] = new Circle(Point2(560, 200), 25, "Q1");
			circs[4] = new Circle(Point2(590, 410), 25, "Q2");
			circs[5] = new Circle(Point2(525, 520), 25, "Q3");
			circs2[3] = new Circle(Point2(560, 200), Color(0, 0, 0), 27, "");
			circs2[4] = new Circle(Point2(590, 410), Color(0, 0, 0), 27, "");
			circs2[5] = new Circle(Point2(525, 520), Color(0, 0, 0), 27, "");
		}
	}
	else if (num_circles == 7) {
			circs[2] = new Circle(Point2(450, 140), 40, "Qaccept");
			circs2[2] = new Circle(Point2(450, 140), Color(0, 0, 0), 42, "");
			circs[3] = new Circle(Point2(160, 150), 25, "Q1");
			circs[4] = new Circle(Point2(595, 300), 25, "Q2");
			circs[5] = new Circle(Point2(650, 455), 25, "Q3");
			circs[6] = new Circle(Point2(530, 560), 25, "Q4");
			circs2[3] = new Circle(Point2(160, 150), Color(0, 0, 0), 27, "");
			circs2[4] = new Circle(Point2(595, 300), Color(0, 0, 0), 27, "");
			circs2[5] = new Circle(Point2(650, 455), Color(0, 0, 0), 27, "");
			circs2[6] = new Circle(Point2(530, 560), Color(0, 0, 0), 27, "");
	}
	else if (num_circles == 8) {
			circs[3] = new Circle(Point2(290, 155), 25, "Q1");
			circs[4] = new Circle(Point2(420, 155), 25, "Q2");
			circs[5] = new Circle(Point2(570, 280), 25, "Q3");
			circs[6] = new Circle(Point2(640, 420), 25, "Q4");
			circs[7] = new Circle(Point2(525, 510), 25, "Q5");
			circs2[3] = new Circle(Point2(329, 155), Color(0, 0, 0), 27, "");
			circs2[4] = new Circle(Point2(420, 155), Color(0, 0, 0), 27, "");
			circs2[5] = new Circle(Point2(570, 280), Color(0, 0, 0), 27, "");
			circs2[6] = new Circle(Point2(640, 420), Color(0, 0, 0), 27, "");
			circs2[7] = new Circle(Point2(525, 510), Color(0, 0, 0), 27, "");
	}
	else if (num_circles == 9) {
			circs[3] = new Circle(Point2(180, 170), 25, "Q1");
			circs[4] = new Circle(Point2(365, 125), 25, "Q2");
			circs[5] = new Circle(Point2(510, 175), 25, "Q3");
			circs[6] = new Circle(Point2(585, 290), 25, "Q4");
			circs[7] = new Circle(Point2(590, 430), 25, "Q5");
			circs[8] = new Circle(Point2(520, 445), 25, "Q6");
			circs2[3] = new Circle(Point2(180, 170), Color(0, 0, 0), 27, "");
			circs2[4] = new Circle(Point2(365, 125), Color(0, 0, 0), 27, "");
			circs2[5] = new Circle(Point2(510, 175), Color(0, 0, 0), 27, "");
			circs2[6] = new Circle(Point2(585, 290), Color(0, 0, 0), 27, "");
			circs2[7] = new Circle(Point2(590, 430), Color(0, 0, 0), 27, "");
			circs2[8] = new Circle(Point2(520, 445), Color(0, 0, 0), 27, "");
	}
	else
		cout << "error" << endl;
}

// load file screen
void mouse3(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[3]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[3]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
			// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[3]->get_clicked();
			WinArray[3]->reset_clicks();
			// They want to quit, and they haven't done it already
			if (button_id == "Quit" && !WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[3]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (button_id == "Quit" && WinArray[1]->get_exists()) {
				WinArray[1]->undraw();
				createNewWindow(*WinArray[1], *WinArray[3]);
			}
			else if (button_id == "Back") {
				// cout << "Back" << endl;
				WinArray[3]->undraw();
				// cout << "Back" << endl;
				createNewWindow(*WinArray[2], *WinArray[3]);
			}
			else if (button_id == "Help") {
				WinArray[9]->set_previous_id(3); // previous window is start window
				WinArray[3]->undraw();
				createNewWindow(*WinArray[9], *WinArray[3]);
			}
			else if (button_id == "Go!") {
				string variables = WinArray[3]->get_text_in_box(0);
				string terminals = WinArray[3]->get_text_in_box(1);
				string start_state = WinArray[3]->get_text_in_box(2);
				vector<string> productions;
				for (int i = 0; i < num_new_textboxes; ++i) {
					productions.push_back(WinArray[3]->get_text_in_box(i+3));
				}
				cfg = new CFG(variables, terminals, start_state[0], productions);
				// cout << variables << endl << terminals << endl << start_state << endl;
				// for (int i = 0; i < num_new_textboxes; ++i) {
				// 	cout << productions[i] << endl;
				// }
				if (cfg->good) 
					make_circles();
				WinArray[5]->set_previous_id(3);
				WinArray[3]->undraw();
				createNewWindow(*WinArray[5], *WinArray[3]);
			}
			else if (button_id == "ε") {
				cout << "ε" << endl;
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}

void init_circles(string filename) {
	string tempfilename = "savedCFGs/";
	tempfilename += filename;
	filename = tempfilename;
	string temp;
	ifstream f(filename.c_str());
	while (f >> temp) {
		if (temp == "PDA")
			break;
	}
	f >> num_circles;
	int x, y;
	for (int i = 0; i < num_circles; ++i) {
		f >> x >> y;
		circs[i]->setPosition(x, y);
		circs2[i]->setPosition(x, y);
	}
}

// cfg entry screen
void mouse4(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[4]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[4]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
			// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[4]->get_clicked();
			WinArray[4]->reset_clicks();
			// They want to quit, and they haven't done it already
			if (button_id == "Quit" && !WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[4]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (button_id == "Quit" && WinArray[1]->get_exists()) {
				WinArray[1]->undraw();
				createNewWindow(*WinArray[1], *WinArray[4]);
			}
			else if (button_id == "Back") {
				// cout << "Back" << endl;
				WinArray[4]->undraw();
				// cout << "Back" << endl;
				createNewWindow(*WinArray[2], *WinArray[4]);
			}
			else if (button_id == "Go!") {
				WinArray[5]->set_previous_id(4);
				WinArray[4]->undraw();
				string newfile = WinArray[4]->get_text_in_box(0);
				cfg = new CFG(newfile.c_str());
				if (cfg->good) {
					make_circles();
					if (cfg->pda) {
						init_circles(newfile);
					}
				}
				createNewWindow(*WinArray[5], *WinArray[4]);
			}
			else if (button_id == "Help") {
				WinArray[10]->set_previous_id(4); // previous window is start window
				WinArray[4]->undraw();
				createNewWindow(*WinArray[10], *WinArray[4]);
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}

// PDA viewer screen
void mouse5(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[5]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			for (int i = 0; i < num_circles; ++i) {
				if (circs2[i]->containsPoint(x, y)) {
					circle_clicked = true;
					clicked_circle = i;
					break;
				}
			}
			if (!circle_clicked) {
				global_x = x;
				global_y = y;
				// cout << "x: " << x << " y: " << y << endl;
			}
			WinArray[5]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
			// the user just let up on the button-- do something
			// cout << clicked_circle << " " << x << " " << y << endl;
			circle_clicked = false;
			clicked_circle = -1;
			mouseIsDragging = false;
			string button_id = WinArray[5]->get_clicked();
			WinArray[5]->reset_clicks();
			// They want to quit, and they haven't done it already
			if (button_id == "Quit" && !WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[5]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (button_id == "Quit" && WinArray[1]->get_exists()) {
				WinArray[1]->undraw();
				createNewWindow(*WinArray[1], *WinArray[5]);
			}
			else if (button_id == "Back") {
				WinArray[5]->undraw();
				createNewWindow(*WinArray[WinArray[5]->get_previous_id()], *WinArray[5]);
			}
			else if (button_id == "Help") {
				WinArray[11]->set_previous_id(5); // previous window is start window
				WinArray[5]->undraw();
				createNewWindow(*WinArray[11], *WinArray[5]);
			}
			else if (button_id == "Save") {
				WinArray[5]->undraw();
				createNewWindow(*WinArray[6], *WinArray[5]);
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
		// cout << "Right button clicked" << endl;
	}
	else if (mouseButton == 3 || mouseButton == 4) {
		cout << "scrolling" << endl;
		if (state == GLUT_UP) {
			cout << "Scroll up" << endl;
		}
		else if (state == GLUT_DOWN) {
			cout << "Scroll down" << endl;
		}
	}
	glutPostRedisplay();
}

void save_pda(string filename) {
	string tempfilename = "savedCFGs/";
	tempfilename += filename;
	filename = tempfilename;
	ofstream o;
	o.open(filename.c_str(), ios::out | ios::app);
	o << "PDA" << endl;
	o << num_circles << endl;
	for (int i = 0; i < num_circles; ++i) {
		o << circs[i]->getPosition().x << " " << circs[i]->getPosition().y << endl;
	}
}

// save file screen
void mouse6(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[6]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[6]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
			// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[6]->get_clicked();
			WinArray[6]->reset_clicks();
			// They want to quit, and they haven't done it already
			if (button_id == "Back") {
				WinArray[6]->undraw();
				createNewWindow(*WinArray[5], *WinArray[6]);
			}
			else if (button_id == "Save") {
				int ret;
				string savefile = WinArray[6]->get_text_in_box(0);
				// cout << "Saving to " << savefile << endl;
				if ((ret = cfg->save(savefile)) < 0) {
					string err_mesg;
					err_mesg = "Error saving to ";
					err_mesg += savefile;
					err_mesg += ". Please try again.";
					WinArray[6]->set_err_mesg(err_mesg, 25, 90, GLUT_BITMAP_HELVETICA_12);
					WinArray[6]->set_err_bool(1);
				}
				else {
					save_pda(savefile);
					WinArray[6]->set_err_bool(0);
					// WinArray[current]->undraw();
					if (!WinArray[1]->get_exists()) {
					// glutPostRedisplay();
						createNewWindow(*WinArray[1], *WinArray[0]);
						WinArray[1]->set_exists(1);
					}
					// They've already created a Quit window and they're trying to create another. :(
					else if (WinArray[1]->get_exists()) {
						WinArray[1]->undraw();
						createNewWindow(*WinArray[1], *WinArray[0]);
					}
				}
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}
 // help menu 1
void mouse7(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[7]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[7]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
			// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[7]->get_clicked();
			WinArray[7]->reset_clicks();
			// They want to quit, and they haven't done it already
			if (button_id == "Quit" && !WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[7]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (button_id == "Quit" && WinArray[1]->get_exists()) {
				WinArray[1]->undraw();
				createNewWindow(*WinArray[1], *WinArray[7]);
			}
			if (button_id == "Return") {
				WinArray[7]->undraw();
				createNewWindow(*WinArray[WinArray[7]->get_previous_id()], *WinArray[7]);
			}
			else if (button_id == "Next") {
				WinArray[7]->undraw();
				WinArray[8]->set_previous_id(WinArray[7]->get_previous_id());
				createNewWindow(*WinArray[8], *WinArray[7]);
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}

// help menu 2
void mouse8(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[8]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[8]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
			// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[8]->get_clicked();
			WinArray[8]->reset_clicks();
			// They want to quit, and they haven't done it already
			if (button_id == "Quit" && !WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[8]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (button_id == "Quit" && WinArray[1]->get_exists()) {
				WinArray[1]->undraw();
				createNewWindow(*WinArray[1], *WinArray[8]);
			}
			if (button_id == "Return") {
				WinArray[8]->undraw();
				createNewWindow(*WinArray[WinArray[8]->get_previous_id()], *WinArray[8]);
			}
			else if (button_id == "Previous") {
				WinArray[8]->undraw();
				WinArray[7]->set_previous_id(WinArray[8]->get_previous_id());
				createNewWindow(*WinArray[7], *WinArray[8]);
			}
			else if (button_id == "Next") {
				WinArray[8]->undraw();
				WinArray[9]->set_previous_id(WinArray[8]->get_previous_id());
				createNewWindow(*WinArray[9], *WinArray[8]);
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}

// help menu 3
void mouse9(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[9]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[9]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
			// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[9]->get_clicked();
			WinArray[9]->reset_clicks();
			// They want to quit, and they haven't done it already
			if (button_id == "Quit" && !WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[9]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (button_id == "Quit" && WinArray[1]->get_exists()) {
				WinArray[1]->undraw();
				createNewWindow(*WinArray[1], *WinArray[9]);
			}
			if (button_id == "Return") {
				WinArray[9]->undraw();
				createNewWindow(*WinArray[WinArray[9]->get_previous_id()], *WinArray[9]);
			}
			else if (button_id == "Previous") {
				WinArray[9]->undraw();
				WinArray[8]->set_previous_id(WinArray[9]->get_previous_id());
				createNewWindow(*WinArray[8], *WinArray[9]);
			}
			else if (button_id == "Next") {
				WinArray[9]->undraw();
				WinArray[10]->set_previous_id(WinArray[9]->get_previous_id());
				createNewWindow(*WinArray[10], *WinArray[9]);
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}

// help menu 3
void mouse10(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[10]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[10]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
			// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[10]->get_clicked();
			WinArray[10]->reset_clicks();
			// They want to quit, and they haven't done it already
			if (button_id == "Quit" && !WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[10]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (button_id == "Quit" && WinArray[1]->get_exists()) {
				WinArray[1]->undraw();
				createNewWindow(*WinArray[1], *WinArray[10]);
			}
			if (button_id == "Return") {
				WinArray[10]->undraw();
				createNewWindow(*WinArray[WinArray[10]->get_previous_id()], *WinArray[10]);
			}
			else if (button_id == "Previous") {
				WinArray[10]->undraw();
				WinArray[9]->set_previous_id(WinArray[10]->get_previous_id());
				createNewWindow(*WinArray[9], *WinArray[10]);
			}
			else if (button_id == "Next") {
				WinArray[10]->undraw();
				WinArray[11]->set_previous_id(WinArray[10]->get_previous_id());
				createNewWindow(*WinArray[11], *WinArray[10]);
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}

// help menu 3
void mouse11(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[11]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[11]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
			// the user just let up on the button-- do something
			mouseIsDragging = false;
			string button_id = WinArray[11]->get_clicked();
			WinArray[11]->reset_clicks();
			// They want to quit, and they haven't done it already
			if (button_id == "Quit" && !WinArray[1]->get_exists()) {
				// glutPostRedisplay();
				createNewWindow(*WinArray[1], *WinArray[11]);
				WinArray[1]->set_exists(1);
			}
			// They've already created a Quit window and they're trying to create another. :(
			else if (button_id == "Quit" && WinArray[1]->get_exists()) {
				createNewWindow(*WinArray[1], *WinArray[11]);
			}
			if (button_id == "Return") {
				WinArray[11]->undraw();
				createNewWindow(*WinArray[WinArray[11]->get_previous_id()], *WinArray[11]);
			}
			else if (button_id == "Previous") {
				WinArray[11]->undraw();
				WinArray[10]->set_previous_id(WinArray[11]->get_previous_id());
				createNewWindow(*WinArray[10], *WinArray[11]);
			}
			else {
				// cout << glutGetWindow() << endl;
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
	glutPostRedisplay();
}

void reposition_window(Window &new_win, Window old_win) {
	int screen_pos_x = old_win.get_x_pos();
	int screen_pos_y = old_win.get_y_pos();
	int d_width, d_height;
	if (new_win.get_width() <= old_win.get_width()) {
		d_width = old_win.get_width() - new_win.get_width();
		d_width/=2;
		screen_pos_x += d_width;
	}
	else {
		d_width = new_win.get_width() - old_win.get_width();
		d_width/=2;
		screen_pos_x -= d_width;
	}
	if (new_win.get_height() <= old_win.get_height()) {
		d_height = old_win.get_height() - new_win.get_height();
		d_height/=2;
		screen_pos_y += d_height;
	}
	else {
		d_height = new_win.get_height() - old_win.get_height();
		d_height/=2;
		screen_pos_y -= d_height;
	}

	// cout << "x: " << screen_pos_x << " y: " << screen_pos_y << endl;
	new_win.set_x_pos(screen_pos_x);
	new_win.set_y_pos(screen_pos_y);
}


void mouse_motion(int x, int y) {
	int current;
	int win = glutGetWindow();
	for (current = 0; current < numWindows; ++current) {
		if (win == WinArray[current]->get_current_id()) {
			break;
		}
	}
	if (current != 12) {
		if (current == 5 && mouseIsDragging && circle_clicked && (clicked_circle != -1)) {
			// cout << "hi" << endl;
			circs2[clicked_circle]->update(x, y);
			circs[clicked_circle]->update(x, y);
		}
		else if (current == 5 && mouseIsDragging && !circle_clicked && (clicked_circle == -1)) {
			int dx, dy;
			// cout << x << " " << y << endl;
			dx = x-global_x;
			dy = y-global_y;
			global_x += dx;
			global_y += dy;
			// cout << "dx: " << dx << " dy: " << dy << endl;
			for (int i = 0; i < num_circles; ++i) {
				circs2[i]->update(circs2[i]->getPosition().x+dx, circs2[i]->getPosition().y+dy);
				circs[i]->update(circs[i]->getPosition().x+dx, circs[i]->getPosition().y+dy);
			}
		}

		int screen_pos_x = glutGet((GLenum)GLUT_WINDOW_X);
		int screen_pos_y = glutGet((GLenum)GLUT_WINDOW_Y);
		WinArray[current]->set_x_pos(screen_pos_x);
		WinArray[current]->set_y_pos(screen_pos_y);

		mouseCoords(x, y);
		WinArray[current]->t_box_mousemotion(x, y);
		if (mouseIsDragging) {
			WinArray[current]->buttons_mousemotion_dragging(x, y);
		}
		else {
			WinArray[current]->buttons_mousemotion_notdragging(x, y);
		}
		glutPostRedisplay();
	}
}

// create a window and return the id (I really don't know what this does)
void createNewWindow(Window &new_win, Window old_win) {
	reposition_window(new_win, old_win);
	glutInitWindowPosition(new_win.get_x_pos(), new_win.get_y_pos());
	glutInitWindowSize(new_win.get_width(), new_win.get_height());
	int win = glutCreateWindow(new_win.get_name().c_str());
	new_win.set_current_id(win);
	// cout << "win: " << win << endl;
	// cout << "New Window ID: " << new_win.get_current_id() << endl;
	// int id = glutCreateWindow(windowTitle);
	if (new_win.has_texture()) {
		new_win.set_background_id(loadTexture(new_win.get_texture_file().c_str()));
	}
	else {
	// assert( 0 != id );
	// clear the window to black
	glClearColor(new_win.get_color().red/255.0, new_win.get_color().green/255.0, new_win.get_color().blue/255.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	}

	// begin setting up the coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// allow blending (for transparent textures)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glOrtho(0., new_win.get_width()-1, new_win.get_height()-1, 0., -1.0, 1.0);


	if (win == WinArray[0]->get_current_id()) {
		glutDisplayFunc(drawWindow0);
		glutMouseFunc(mouse0);
	}
	else if (win == WinArray[1]->get_current_id()) {
		glutDisplayFunc(drawWindow1);
		glutMouseFunc(mouse1);
	}
	else if (win == WinArray[2]->get_current_id()) {
		glutDisplayFunc(drawWindow2);
		glutMouseFunc(mouse2);
	}
	else if (win == WinArray[3]->get_current_id()) {
		glutDisplayFunc(drawWindow3);
		glutMouseFunc(mouse3);
	}
	else if (win == WinArray[4]->get_current_id()) {
		glutDisplayFunc(drawWindow4);
		glutMouseFunc(mouse4);
	}
	else if (win == WinArray[5]->get_current_id()) {
		glutDisplayFunc(drawWindow5);
		glutMouseFunc(mouse5);
		glutSpecialFunc(handleSpecialKeypress);
	}
	else if (win == WinArray[6]->get_current_id()) {
		glutDisplayFunc(drawWindow6);
		glutMouseFunc(mouse6);
	}
	else if (win == WinArray[7]->get_current_id()) {
		glutDisplayFunc(drawWindow7);
		glutMouseFunc(mouse7);
	}
	else if (win == WinArray[8]->get_current_id()) {
		glutDisplayFunc(drawWindow8);
		glutMouseFunc(mouse8);
	}
	else if (win == WinArray[9]->get_current_id()) {
		glutDisplayFunc(drawWindow9);
		glutMouseFunc(mouse9);
	}
	else if (win == WinArray[10]->get_current_id()) {
		glutDisplayFunc(drawWindow10);
		glutMouseFunc(mouse10);
	}
	else if (win == WinArray[11]->get_current_id()) {
		glutDisplayFunc(drawWindow11);
		glutMouseFunc(mouse11);
	}
	else {
		cout << "Fudgey poo... something's wrong!" << endl;
	}
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouse_motion);
	glutPassiveMotionFunc(mouse_motion);

	glutPostWindowRedisplay(new_win.get_current_id());
	glutSetWindow(new_win.get_current_id());
}

void init(void) {
	if (WinArray[0]->has_texture()) {
		WinArray[0]->set_background_id(loadTexture(WinArray[0]->get_texture_file().c_str()));
	}
	else {
		glClearColor(WinArray[0]->get_color().red/255.0, WinArray[0]->get_color().green/255.0, WinArray[0]->get_color().blue/255.0, 1.0);
	}
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0., WinArray[0]->get_width()-1, WinArray[0]->get_height()-1, 0., -1.0, 1.0);
}

void init_gl_window() {
	char * argv[] = { (char *)WinArray[0]->get_name().c_str() };
	int argc = sizeof(argv) / sizeof(argv[0]);
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );

	glutInitWindowSize(WinArray[0]->get_width(), WinArray[0]->get_height());
	glutInitWindowPosition(WinArray[0]->get_x_pos(), WinArray[0]->get_y_pos());
	WinArray[0]->set_current_id(glutCreateWindow(WinArray[0]->get_name().c_str()));
	// cout << "Start Window ID: " << WinArray[0]->get_current_id() << endl;
	init();

	glutDisplayFunc(drawWindow0);
	glutMouseFunc(mouse0);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouse_motion);
	glutPassiveMotionFunc(mouse_motion);
	glutMainLoop();
}

int main() {
	loadWindows("txt/windows.txt");
	init_gl_window();

	return 0;
}