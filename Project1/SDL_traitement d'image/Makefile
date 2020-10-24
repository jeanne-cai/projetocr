CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main

main: sdl_base.o main.o

main.o: sdl_base.h

demo: sdl_base.o demo.o

demo.o: sdl_base.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} main
	${RM} demo
	${RM} image/*.png
