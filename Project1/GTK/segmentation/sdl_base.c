#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


//---- SDL base

void init_sdl()
{
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}


void SDL_FreeSurface(SDL_Surface *surface);


SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;
    img = IMG_Load(path);

    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

