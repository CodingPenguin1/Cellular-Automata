OBJECTS=wolfram-ca.o

DEFINES=-DEMULATE_STEREO
CXXFLAGS=-g -m64 -Wno-deprecated $(DEFINES) -I/usr/X11R6/include -I/usr/local/include -std=c++11 -ggdb -Wall -Wextra -Wpedantic

LIBS=-L/usr/X11R6/lib64 -lglut -lGLU -lGL -lstdc++ -lXmu -lXi

all: wolfram-ca

clean:
	rm $(OBJECTS)
	rm wolfram-ca

wolfram-ca: $(OBJECTS)
	g++ -o wolfram-ca $(OBJECTS) $(LIBS)
	rm -f *.o *~

.SUFFIXES: .cpp

%.o : %.ccp
	g++ -c $(CXXFLAGS) -o $@ $<
