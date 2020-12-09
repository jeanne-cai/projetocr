#include <err.h>
#include "sdl_base.h"


static inline
Uint8* pixel_ref(SDL_Surface *surface, unsigned x, unsigned y)
{
    int bpp = surface->format->BytesPerPixel;
    return (Uint8 *)surface->pixels + y *surface->pitch + x * bpp;
}

void init_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Unable to initialize SDL: %s.\n", SDL_GetError());
}

void SDL_FreeSurface(SDL_Surface *surface);

SDL_Surface* load_image(char *path)
{
    SDL_Surface *image;
    image = IMG_Load(path);

    if (!image)
        errx(3, "error load %s: %s", path, IMG_GetError());

    return image;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if (SDL_BIG_ENDIAN == SDL_BYTEORDER)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;
    }
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if (SDL_BIG_ENDIAN == SDL_BYTEORDER)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1]= (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}
