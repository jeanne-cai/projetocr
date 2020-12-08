# Makefile
  
CC= gcc

CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -Wpointer-arith -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lm -g

SRC = main.c gtk.c segmentation.c sdl_base.c grayscale.c otsu.c canny.c
OBJ = ${SRC:.c=.o}
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJ}

clean:
	${RM} main
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} image/seg_*.bmp image/image_resized.bmp

-include ${DEP}

# END
