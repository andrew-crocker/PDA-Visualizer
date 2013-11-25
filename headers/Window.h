#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#ifndef _Window_
#define _Window_

#include "Button.h"
#include "TextboxArray.h"

class Window {
	static const int DEFAULT_WIDTH, DEFAULT_HEIGHT;
	static const int DEFAULT_X_POS, DEFAULT_Y_POS;
	static const Color DEFAULT_COLOR;
	static const string DEFAULT_NAME;
	//static int ID;
	Color color;
	string name;
	int width;
	int height;
	int x_pos;
	int y_pos;
	int current_id;
	int previous_id;
	int num_buttons;
	bool texture;
	unsigned background;
	string texture_file;
	bool exists;
	Button ** buttons;
	TextboxArray * t_boxes;
public:
	Window();
	Window(istream &);
	// ~Window();
	void loadButtons(const char *);
	void drawWindow();
	int get_width();
	int get_height();
	int get_current_id();
	int get_previous_id();
	int get_x_pos();
	int get_y_pos();
	void set_x_pos(int);
	void set_y_pos(int);
	bool get_exists();
	void set_exists(bool);
	string get_clicked();
	Color get_color();
	void set_current_id(int);
	void set_previous_id(int);
	void reset_clicks();
	string get_name();
	void set_background_id(int);
	string get_texture_file();
	bool has_texture();
	void undraw();
	void t_box_keypress(unsigned char, int, int);
	void t_box_mousepress(int, int, int, int);
	void buttons_mousepress(int, int, int, int);
	void t_box_mousemotion(int, int);
	void buttons_mousemotion_dragging(int, int);
	void buttons_mousemotion_notdragging(int, int);
};

#endif