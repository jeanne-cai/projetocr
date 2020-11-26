# Makefile
  
CC= gcc

CPPFLAGS= `pkg-config --cflags sdl gtk+-3.0` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image

SRC = main.c GUI/gtk.c segmentation/segmentation.c segmentation/sdl_base.c segmentation/pixel_operations.c segmentation/otsu.c
OBJ = ${SRC:.c=.o}
DEP = ${OBJ:.o=.d}

all: main

main: ${OBJ}

clean:
	${RM} main
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} image/seg_*.bmp

-include ${DEP}

# END
