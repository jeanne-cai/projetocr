#include "sdl_base.h"

void GrayScale(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    Uint8 r, g, b;

    for(size_t i = 0; i < width; i++)
    {
        for(size_t j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            Uint8 average = 0.3*r + 0.59*g + 0.11*b;
            pixel = SDL_MapRGB(image_surface->format,
                        average, average, average);

            put_pixel(image_surface, i, j, pixel);
        }
    }
}
