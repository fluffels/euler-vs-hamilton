CC=g++

LIBS=glew glfw3
LIB_CFLAGS=$(foreach lib,${LIBS},$(shell pkg-config --cflags $(lib)))
LDLIBS=$(foreach lib,${LIBS},$(shell pkg-config --static --libs $(lib)))

WARNING_FLAGS=-Wall -Wextra -Weffc++ -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-parameter -Wstrict-overflow=5 -Wfloat-equal -Wshadow -Wc++0x-compat -Wconversion -Wsign-conversion -Wmissing-declarations -Woverloaded-virtual -Wsign-promo -pedantic
FORMATTING_FLAGS=-fmessage-length=80 -fdiagnostics-show-option
CFLAGS= ${WARNING_FLAGS} ${FORMATTING_FLAGS} ${LIB_CFLAGS} -g -std=c++1y -pipe

LDFLAGS=-g

all: main

main: shader.o main.o
	${CC} $^ ${LDFLAGS} -o $@ ${LDLIBS}

%.o:
	${CC}  ${CFLAGS} -c $< -o $@

main.o: main.C main.h shader.h camera.h include.h
shader.o: shader.C shader.h include.h

clean:
	rm -rf *.o main

