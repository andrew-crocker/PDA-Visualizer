# the following lines pertain to setting up compiler options and libraries
OPTS = -Wall -g -Wno-deprecated
LIBS = -lGL -lglut -lm
ARCH := $(shell uname)
ifeq ($(ARCH), Linux)
else
 MACOSX_DEFINE = -DMACOSX -I/sw/include
 LIBS = -I/usr/common/include -I/usr/include/GL -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework GLUT -framework OpenGL -lGL -lm -lobjc -lstdc++
endif

BINARIES = Framework

# here are the usual make rules

all: $(BINARIES)

Framework: objects/Framework.o
	g++ $(OPTS) -o Framework objects/Framework.o objects/texture.o objects/Button.o objects/Textbox.o objects/TextboxArray.o objects/Window.o objects/CFG.o objects/Shape.o objects/Circle.o $(LIBS)

objects/Framework.o: Framework.cpp objects/Window.o objects/CFG.o objects/Circle.o
	g++ $(OPTS) $(MACOSX_DEFINE) -c Framework.cpp -o objects/Framework.o

objects/Window.o: headers/Window.h definitions/Window.cpp objects/Button.o objects/TextboxArray.o
	g++ $(OPTS) -c definitions/Window.cpp -o objects/Window.o

objects/Button.o: headers/Button.h definitions/Button.cpp headers/ColorPoint2.h objects/texture.o
	g++ $(OPTS) -c definitions/Button.cpp -o objects/Button.o

objects/texture.o: definitions/texture.cpp headers/texture.h
	g++ $(OPTS) -c definitions/texture.cpp -o objects/texture.o

objects/Textbox.o: headers/Textbox.h definitions/Textbox.cpp headers/ColorPoint2.h
	g++ $(OPTS) -c definitions/Textbox.cpp -o objects/Textbox.o

objects/TextboxArray.o: headers/TextboxArray.h definitions/TextboxArray.cpp objects/Textbox.o
	g++ $(OPTS) -c definitions/TextboxArray.cpp -o objects/TextboxArray.o

objects/Circle.o: headers/Circle.h definitions/Circle.cpp objects/Shape.o
	g++ $(OPTS) -c definitions/Circle.cpp -o objects/Circle.o

objects/Shape.o: headers/Shape.h definitions/Shape.cpp
	g++ $(OPTS) -c definitions/Shape.cpp -o objects/Shape.o

objects/CFG.o: headers/CFG.h definitions/CFG.cpp
	g++ $(OPTS) -c definitions/CFG.cpp -o objects/CFG.o
	
clean:
	rm -f $(BINARIES) objects/*.o