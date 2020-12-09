#ifndef SDL_BASE_H_
#define SDL_BASE_H_

#include <SDL.h>
#include "SDL/SDL_image.h"

void init_sdl();
void SDL_FreeSurface(SDL_Surface *surface);

SDL_Surface* load_image(char *path);

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

#endif
