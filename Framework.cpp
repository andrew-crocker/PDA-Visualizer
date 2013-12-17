#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "headers/Window.h"

using namespace std;

bool mouseIsDragging = false;

int numWindows;
Window ** WinArray;
int num_new_textboxes = 0;
Textbox * new_textboxes[6];

void createNewWindow(Window &, Window);

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
	WinArray[0]->drawWindow();
}

void drawWindow1() {
	WinArray[1]->drawWindow();
}

void drawWindow2() {
	WinArray[2]->drawWindow();
}

void drawWindow3() {
	WinArray[3]->drawWindow();
}

void drawWindow4() {
	WinArray[4]->drawWindow();
}

void drawWindow5() {
	WinArray[5]->drawWindow();
}

void drawWindow6() {
	WinArray[6]->drawWindow();
}

void drawWindow7() {
	WinArray[7]->drawWindow();
}

void drawWindow8() {
	WinArray[8]->drawWindow();
}

void drawWindow9() {
	WinArray[9]->drawWindow();
}

void drawWindow10() {
	WinArray[10]->drawWindow();
}

void drawWindow11() {
	WinArray[11]->drawWindow();
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
				else if (s != 0 && s != 32) { // s is a new character
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
					productions.push_back(WinArray[3]->get_text_in_box(i));
				}
				cout << variables << endl << terminals << endl << start_state << endl;
				for (int i = 0; i < num_new_textboxes; ++i) {
					cout << productions[i] << endl;
				}
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

void mouse5(int mouseButton, int state, int x, int y) {

	mouseCoords(x, y);
	WinArray[5]->t_box_mousepress(mouseButton, state, x, y);
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
			mouseIsDragging = true;
			WinArray[5]->buttons_mousepress(mouseButton, state, x, y);
		}
		else {
			// the user just let up on the button-- do something
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
	}
	glutPostRedisplay();
}

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
				cout << "Saving" << endl;
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
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[1]->get_current_id()) {
		glutDisplayFunc(drawWindow1);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse1);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[2]->get_current_id()) {
		glutDisplayFunc(drawWindow2);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse2);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[3]->get_current_id()) {
		glutDisplayFunc(drawWindow3);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse3);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[4]->get_current_id()) {
		glutDisplayFunc(drawWindow4);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse4);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[5]->get_current_id()) {
		glutDisplayFunc(drawWindow5);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse5);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[6]->get_current_id()) {
		glutDisplayFunc(drawWindow6);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse6);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[7]->get_current_id()) {
		glutDisplayFunc(drawWindow7);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse7);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[8]->get_current_id()) {
		glutDisplayFunc(drawWindow8);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse8);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[9]->get_current_id()) {
		glutDisplayFunc(drawWindow9);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse9);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[10]->get_current_id()) {
		glutDisplayFunc(drawWindow10);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse10);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else if (win == WinArray[11]->get_current_id()) {
		glutDisplayFunc(drawWindow11);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse11);
		glutMotionFunc(mouse_motion);
		glutPassiveMotionFunc(mouse_motion);		
	}
	else {
		cout << "Fudgey poo... something's wrong!" << endl;
	}

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