#include "../headers/Window.h"
using namespace std;

const int Window::DEFAULT_WIDTH = 500;
const int Window::DEFAULT_HEIGHT = 500;
const int Window::DEFAULT_X_POS = 100;
const int Window::DEFAULT_Y_POS = 100;
const Color Window::DEFAULT_COLOR(0, 0, 0);
const string Window::DEFAULT_NAME = "Window";

//int Window::ID = 0;

Window::Window() {
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
	x_pos = DEFAULT_X_POS;
	y_pos = DEFAULT_Y_POS;
	name = DEFAULT_NAME;
	color = DEFAULT_COLOR;
	// current_id = ++ID;
	previous_id = 0;
	num_buttons = 0;
	exists = 0;
	texture = 0;
	background = 0;
	buttons = new Button*[1];
	buttons[0] = new Button;
	t_boxes = new TextboxArray;
}

Window::Window(istream &g) {
	g.get();
	getline(g, name);
	// cout << "name: " << name << endl;
	g >> width >> height >> x_pos >> y_pos >> color.red >> color.green >> color.blue;

	// current_id = glutCreateWindow(name.c_str());
	current_id = 0;
	previous_id = 0;
	exists = 0;
	string button_file, textbox_file;
	g >> texture_file >> button_file >> textbox_file;
	// g.close();
	// cout << "texture_file: " << texture_file << endl;
	// cout << " button_file: " << button_file << endl;
	// cout << " textbox_file: " << textbox_file << endl;
	if (texture_file != "none") {
		texture = 1;
		// cout << "here" << endl;
		// cout << "texture loaded" << endl;
	}
	else {
		// cout << "here" << endl;
		texture = 0;
	}
	if (button_file != "none") {
		loadButtons(button_file.c_str());
	}
	else {
		num_buttons = 0;
		buttons = new Button*[1];
		buttons[0] = new Button;
	}
	if (textbox_file != "none") {
		t_boxes = new TextboxArray(textbox_file.c_str());
	}
	else {
		t_boxes = new TextboxArray;
	}
}

// Window::~Window() {

// }

int Window::get_width() {
	return width;
}

int Window::get_height() {
	return height;
}

int Window::get_x_pos() {
	return x_pos;
}

int Window::get_y_pos() {
	return y_pos;
}

int Window::get_current_id() {
	return current_id;
}

int Window::get_previous_id() {
	return previous_id;
}

string Window::get_name() {
	return name;
}

void Window::set_current_id(int id) {
	current_id = id;
}

void Window::set_previous_id(int id) {
	previous_id = id;
}

bool Window::get_exists() {
	return exists;
}

void Window::set_exists(bool b) {
	exists = b;
}

void Window::set_x_pos(int x) {
	x_pos = x;
}

void Window::set_y_pos(int y) {
	y_pos = y;
}

Color Window::get_color() {
	return color;
}

void Window::set_background_id(int id) {
	background = id;
}

string Window::get_texture_file() {
	return texture_file;
}

bool Window::has_texture() {
	return texture;
}

void Window::reset_clicks() {
	for (int i = 0; i < num_buttons; ++i) {
		buttons[i]->setClicked(0);
		buttons[i]->setHover(0);
		buttons[i]->setPreClicked(0);
	}
}

string Window::get_clicked() {
	for (int i = 0; i < num_buttons; ++i) {
		if (buttons[i]->getClicked())
			return buttons[i]->getLabel();
	}
	// reset_clicks();
	return "";
}

void Window::buttons_mousemotion_notdragging(int x, int y) {
	for (int i = 0; i < num_buttons; ++i) {
		if (buttons[i]->containsPoint(x, y)) {
			// buttons[i]->display();
			buttons[i]->setHover(true);
		}
		else {
			buttons[i]->setHover(false);
		}
	}
	// reset_clicks();
}

void Window::buttons_mousemotion_dragging(int x, int y) {
	for (int i = 0; i < num_buttons; ++i) {
		if (buttons[i]->containsPoint(x, y)) {
			// buttons[i]->display();
			buttons[i]->setHover(true);
			buttons[i]->setClicked(true);
		}
		else {
			buttons[i]->setHover(false);
			buttons[i]->setClicked(false);
		}
	}
	// reset_clicks();
}

void Window::t_box_mousemotion(int x, int y) {
	t_boxes->handleMouseMovement(x, y);
}

void Window::buttons_mousepress(int mouseButton, int state, int x, int y) {
	for (int i = 0; i < num_buttons; ++i) {
		if (buttons[i]->containsPoint(x, y)) {
			// buttons[i]->display();
			buttons[i]->setClicked(true);
		}
		else {
			buttons[i]->setClicked(false);
		}
	}
}

int Window::t_box_keypress(unsigned char c, int x, int y) {
	return t_boxes->handleKeyPress(c, x, y);
}

void Window::t_box_mousepress(int mouseButton, int state, int x, int y) {
	t_boxes->handleMousePress(mouseButton, state, x, y);
}

void Window::undraw() {
	if (texture) {
		// cout << "hi" << endl;
		// cout << "undraw texture ID: " << background << endl;
		glDeleteTextures(1, &background);
		// cout << "hi2" << endl;
	}
	glutDestroyWindow(current_id);
	set_current_id(0);
}

void Window::drawWindow() {
	glClear(GL_COLOR_BUFFER_BIT);
	if (texture) {
		// cout << "why am I here?" << endl;
		// cout << "draw texture ID: " << background << endl;
		drawTexture(background, 0,0, width, height);//texNum, x,y, width, height, alpha, rotation (in radians)
	}
	for(int i = 0; i < num_buttons; ++i) {
		buttons[i]->draw();
	}
	t_boxes->draw();
	glutSwapBuffers();
}

void Window::loadButtons(const char * filename ) {
  ifstream g (filename);
  if( !g.good() ) {
	cerr << "Warning: Unable to open " << filename << ", ignoring it." << endl;
	return;
  }
  g >> num_buttons;
  buttons = new Button* [num_buttons];
  for(int i = 0; i < num_buttons; i++) {
	buttons[i] = new Button(g);
	// buttons[i]->display();
  } 
  // cout << "Number of buttons: " << num_buttons << endl;
}

string Window::get_text_in_box(int t_box) {
	if (t_box < t_boxes->getNumTextboxes()) {
		return t_boxes->getText(t_box);
	}
	else
		return 0;
}

char Window::get_new_text_in_box(int t_box) {
	if (t_box < t_boxes->getNumTextboxes()) {
		string t = t_boxes->getText(t_box);
		return t.back();
	}
	else
		return 0;
}

void Window::add_new_textbox(Textbox & t) {
	// cout << t_boxes->getNumTextboxes() << endl;
	// Textbox ** t = new Textbox*[1];
	// t[0] = new Textbox(p, width, height, start_text);
	// TextboxArray temp(t, 1);
	// t_boxes += temp;
	// Textbox t(p, width, height, start_text);
	t_boxes->addTextbox(t);
}

void Window::remove_textbox() {
	t_boxes->removeTextbox();
}

int Window::get_textbox_clicked(int i) {
	return t_boxes->getClicked(i);
}

Point2 Window::get_previous_textbox_position() {
	int last_textbox = t_boxes->getNumTextboxes()-1;
	return t_boxes->getPosition(last_textbox);
}

int Window::get_previous_textbox_width() {
	int last_textbox = t_boxes->getNumTextboxes()-1;
	return t_boxes->getWidth(last_textbox);
}

int Window::get_previous_textbox_height() {
	int last_textbox = t_boxes->getNumTextboxes()-1;
	return t_boxes->getHeight(last_textbox);
}