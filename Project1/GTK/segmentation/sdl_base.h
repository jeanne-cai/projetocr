#ifndef SDL_BASE_H
#define SDL_BASE_H

void init_sdl();
void SDL_FreeSurface(SDL_Surface *surface);
SDL_Surface* load_image(char *path);

#endif
