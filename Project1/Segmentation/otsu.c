#include "sdl_base.h"


// ---- Tools histogram

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


// ---- Otsu's method

Uint8 otsu_threshold(float *hist)
{
    float w0 = 0, w1 = 0, wT = 0;
    float u0 = 0, u1 = 0, uT = 0;
    float sum = 0, vk = 0, v_max = 0;
    float threshold = 0;

    for (size_t i = 0; i < 256; i++)
    {
        uT += i * hist[i];
        wT += hist[i];
    }

    for (size_t i = 0; i < 256; i++)
    {
        w0 += hist[i];
        w1 = wT - w0;

        sum += i * hist[i];
        u0 = sum / w0;
        u1 = (uT - sum) / w1;

        // Maximizing inter-class variance
        vk = w0 * w1 * (u0 - u1) * (u0 - u1);

        // Find max vk = Find threshold
        if (vk > v_max)
        {
            threshold = i;
            v_max = vk;
        }
    }

    return (Uint8)threshold;
}

// Main : Otsu's method

void Otsu(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    float hist[256] = {0};
    init_hist(image_surface, width, height, hist);
    normalized(hist, width * height);

    Uint8 threshold = otsu_threshold(hist);
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
