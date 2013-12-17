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

TextboxArray::TextboxArray(Textbox **t, int num) {
	numTextboxes = num;
	TboxArray = new Textbox*[num];
	for (int i = 0; i < numTextboxes; ++i) {
		TboxArray[i] = new Textbox(*t[i]);
	}
}

TextboxArray::~TextboxArray() {
	for (int i = 0; i < numTextboxes; ++i) {
		delete TboxArray[i];
	}
	delete TboxArray;
}

TextboxArray & TextboxArray::operator=(const TextboxArray t) {
	numTextboxes = t.numTextboxes;
	TboxArray = t.TboxArray;
	return *this;
}

// TextboxArray *& TextboxArray::operator+=(const TextboxArray t) {
// 	numTextboxes+=t.numTextboxes;
// 	Textbox ** temp;
// 	temp = new Textbox*[numTextboxes];
// 	int i;
// 	for (i = 0; i < numTextboxes-t.numTextboxes; ++i) {
// 		temp[i] = TboxArray[i];
// 	}
// 	for (; i < numTextboxes; ++i) {
// 		temp[i] = t.TboxArray[i];
// 	}
// 	TboxArray = temp;
// 	return *this;
// }

// TextboxArray & TextboxArray::operator+=(const Textbox t) {
// 	TextboxArray temp;
// 	temp.numTextboxes = numTextboxes+1;
// 	temp.TboxArray = new Textbox*[temp.numTextboxes];

// 	for (int i = 0; i < numTextboxes; ++i) {
// 		temp.TboxArray[i] = TboxArray[i];
// 	}
// 	temp.TboxArray[numTextboxes-1] = &t;
// 	return temp;
// }

void TextboxArray::draw() {
	for (int i = 0; i < numTextboxes; ++i) {
		TboxArray[i]->draw();
	}
}

int TextboxArray::handleKeyPress(unsigned char c, int x, int y) {
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
		  //   if ( 13==c ) { // Enter/Return
				// // cout << "textBox content was: " << TboxArray[i]->getTextInBox() << endl;
				// // int hours = atoi(TboxArray[0]->getTextInBox().c_str());
				// // int minutes = atoi(TboxArray[1]->getTextInBox().c_str());
				// // double seconds = atoi(TboxArray[2]->getTextInBox().c_str());
				// // cout << "hours: " << hours << endl;
				// // cout << "minutes: " << minutes << endl;
				// // cout << "seconds: " << seconds << endl;
				// // c_time T(hours, minutes, seconds);
				// // T.display();
				// for (int i = 0; i < numTextboxes; ++i) {
				// 	TboxArray[i]->setTextInBox("");
				// }
		  //   }
		    else if ( '\b'==c || 127==c ) { // handle backspace
				if (TboxArray[i]->getTextInBoxLength() > 0)
					TboxArray[i]->deleteTextInBox();
				return -1;
		    }
		    else if ( c >= 32 && c <= 126 ) { // check for printable character
				// check that we don't overflow the box
				if (TboxArray[i]->getTextInBoxLength() < TboxArray[i]->getMaxNumChars() )
					TboxArray[i]->addTextInBox(c);
		    }
		    else if (13 == c) {
		    	break;
		    }
		    return 1;
		}
	}
	return 0;
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

int TextboxArray::getNumTextboxes() {
	return numTextboxes;
}

string TextboxArray::getText(int index) {
	return TboxArray[index]->getTextInBox();
}

void TextboxArray::addTextbox(Textbox &t) {
	// cout << "hi" << endl;
	numTextboxes++;
	Textbox ** temp;
	temp = new Textbox*[numTextboxes];
	for (int i = 0; i < numTextboxes-1; ++i) {
		temp[i] = TboxArray[i];
	}
	temp[numTextboxes-1] = &t;
	TboxArray = temp;
}

void TextboxArray::removeTextbox() {
	if (numTextboxes > 3) {
		numTextboxes--;
	}
}

int TextboxArray::getClicked(int i) {
	if (TboxArray[i]->getClicked())
		return 1;
	else {
		return 0;
	}
}

Point2 TextboxArray::getPosition(int i) {
	return TboxArray[i]->getPosition();
}

int TextboxArray::getWidth(int i) {
	return TboxArray[i]->getWidth();
}

int TextboxArray::getHeight(int i) {
	return TboxArray[i]->getHeight();
}





//