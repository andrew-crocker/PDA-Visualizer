#include "../headers/TextboxArray.h"

using namespace std;

TextboxArray::TextboxArray() {
	numTextboxes = 0;
	TboxArray = new Textbox*[1];
	TboxArray[0] = new Textbox;
}

TextboxArray::TextboxArray(const char * filename) {
	ifstream g (filename);
	if( !g.good() ) {
		cout << "loadbutton issue" << endl;
		cerr << "Warning: Unable to open " << filename << ", ignoring it." << endl;
		return;
	}
	g >> numTextboxes;
	TboxArray = new Textbox* [numTextboxes];
	for(int i = 0; i < numTextboxes; i++) {
		TboxArray[i] = new Textbox(g);
		// TboxArray[i]->display();
	} 
}

TextboxArray::~TextboxArray() {
	for (int i = 0; i < numTextboxes; ++i)
		delete [] TboxArray[i];
	delete TboxArray;
}

TextboxArray & TextboxArray::operator=(const TextboxArray t) {
	numTextboxes = t.numTextboxes;
	TboxArray = t.TboxArray;
	return *this;
}

void TextboxArray::draw() {
	for (int i = 0; i < numTextboxes; ++i) {
		TboxArray[i]->draw();
	}
}

void TextboxArray::handleKeyPress(unsigned char c, int x, int y) {
	for (int i = 0; i < numTextboxes; ++i) {
		if (TboxArray[i]->getClicked() ) { // intercept keyboard press, to place in text box
		    if ( 27==c ) {
		    	// int win = glutGetWindow();
				// glutDestroyWindow(win);
				exit(0);
		    } // exitAll();  // escape terminates the program, even in textbox
		    if ( '\t'==c ) {
		    	TboxArray[i]->setClicked(false);
		    	if (i == (numTextboxes-1)) {
		    		i = -1;
		    	}
		    	TboxArray[++i]->setClicked(true);
		    }
		    if ( 13==c ) {
				// cout << "textBox content was: " << TboxArray[i]->getTextInBox() << endl;
				// int hours = atoi(TboxArray[0]->getTextInBox().c_str());
				// int minutes = atoi(TboxArray[1]->getTextInBox().c_str());
				// double seconds = atoi(TboxArray[2]->getTextInBox().c_str());
				// cout << "hours: " << hours << endl;
				// cout << "minutes: " << minutes << endl;
				// cout << "seconds: " << seconds << endl;
				// c_time T(hours, minutes, seconds);
				// T.display();
				for (int i = 0; i < numTextboxes; ++i) {
					TboxArray[i]->setTextInBox("");
				}
		    }
		    else if ( '\b'==c || 127==c ) { // handle backspace
				if (TboxArray[i]->getTextInBoxLength() > 0)
					TboxArray[i]->deleteTextInBox();
		    }
		    else if ( c >= 32 && c <= 126 ) { // check for printable character
				// check that we don't overflow the box
				if (TboxArray[i]->getTextInBoxLength() < TboxArray[i]->getMaxNumChars() )
					TboxArray[i]->addTextInBox(c);
		    }
		}
	}
}

void TextboxArray::handleMousePress(int mouseButton, int state, int x, int y) {
	if (GLUT_LEFT_BUTTON == mouseButton ) {
		if (GLUT_DOWN == state ) {
		// the user just pressed down on the mouse-- do something
		}
		else {
		// the user just let up on the button-- do something
			for (int i = 0; i < numTextboxes; ++i) {
				if (TboxArray[i]->getOverTextBox()) {
					TboxArray[i]->setClicked(true);
				}
				else {
					TboxArray[i]->setClicked(false);
				}
			}
		}
	}
	else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
		// whatever
	}
}

void TextboxArray::handleMouseMovement(int x, int y) {
	for (int i = 0; i < numTextboxes; ++i) {
		if (TboxArray[i]->containsPoint(x, y)) {
			TboxArray[i]->setOverTextBox(true);
		}
		else {
			TboxArray[i]->setOverTextBox(false);
		}
	}
}