#ifndef GTK_H
#define GTK_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../segmentation/sdl_base.h"
#include "../segmentation/segmentation.h"
#include "../segmentation/otsu.h"

int gtk_init_window(int argc, char **argv);

#endif
