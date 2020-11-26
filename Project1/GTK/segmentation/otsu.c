#include "otsu.h"

void init_hist(SDL_Surface *image_surface, size_t w, size_t h, float *hist)
{
    Uint8 r, g, b;
    for(size_t i = 0; i < w; i++)
    {
        for(size_t j = 0; j < h; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            hist[r]++;
        }
    }
}

void normalized(float *hist, size_t nbPixel)
{
    for (size_t i = 0; i < 256; i++)
        hist[i] = hist[i] / nbPixel;
}

Uint8 findThreshold(float *hist)
{
    float wk = 0, uk = 0, uT = 0;
    float vk = 0, v_max = 0, threshold = 0;

    for (size_t i = 0; i < 256; i++)
        uT += i * hist[i];

    for(size_t i = 0; i < 256; i++)
    {
        wk += hist[i];
        uk += i * hist[i];

        // Variance inter
        vk = ((uT * wk - uk) * (uT * wk - uk)) / (wk * (1 - wk));

        if (vk > v_max)
        {
            threshold = i;
            v_max = vk;
        }
    }

    return (Uint8)threshold;
}

void Otsu(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    float hist[256] = {0};
    init_hist(image_surface, width, height, hist);
    normalized(hist, width * height);

    Uint8 threshold = findThreshold(hist);
    Uint8 r, g, b;

    for(size_t i = 0; i < width; i++)
    {
        for(size_t j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            if (r > threshold)
                r = 255;
            else
                r = 0;

            pixel = SDL_MapRGB(image_surface->format, r, r, r);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}
