CXXFLAGS=-g -m64 -Wno-deprecated -DEMULATE_STEREO -I/usr/X11R6/include -I/usr/local/include -std=c++11 -ggdb -Wall -Wextra -Wpedantic
LIBS=-L/usr/X11R6/lib64 -lglut -lGLU -lGL -lstdc++ -lXmu -lXi

all: main

clean:
	rm main

main: main.cpp
	g++ -o $@ $^ $(CXXFLAGS) $(LIBS)