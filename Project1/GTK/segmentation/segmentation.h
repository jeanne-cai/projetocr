#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"

void GrayScale(SDL_Surface *image_surface);
void Contour(SDL_Surface *image_surface);

#endif
