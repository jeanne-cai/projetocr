#include "segmentation.h"

//---- Main Grayscale

// Main

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


//---- Main Contour

// IsEmpty

int line_isempty(SDL_Surface *image_surface, size_t width, size_t h_pos)
{
    Uint8 r, g, b;
    for (size_t i = 0; i < width; i++)
    {
        Uint32 pixel = get_pixel(image_surface, i, h_pos);
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

        if (!r && !b && !g)
            return 0;
    }
    return 1;
}

int column_isempty(SDL_Surface *image_surface, size_t w_pos,
    size_t h1, size_t h2)
{
    Uint8 r, g, b;
    for (size_t h = h1; h < h2; h++)
    {
        Uint32 pixel = get_pixel(image_surface, w_pos, h);
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

        if (!r && !b && !g)
            return 0;
    }
    return 1;
}

// Draw

void drawline(SDL_Surface *image_surface, size_t width, size_t h_pos)
{
    for (size_t i = 0; i < width; i++)
    {
        Uint32 pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
        put_pixel(image_surface, i, h_pos, pixel);
    }
}

void drawcolumn(SDL_Surface *image_surface, size_t w_pos,
    size_t h1, size_t h2)
{
    for (size_t h = h1; h < h2; h++)
    {
        Uint32 pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
        put_pixel(image_surface, w_pos, h, pixel);
    }
}

// Split

void Snap(SDL_Surface *image_surface, size_t x1, size_t y1,
    size_t x2, size_t y2, const char *filename)
{
    size_t w = x2 - x1;
    size_t h = y2 - y1;

    SDL_Surface *letter_surface;
    SDL_Rect position;

    letter_surface = SDL_CreateRGBSurface(0, w, h, 32,
                        0, 0, 0, 0);
    position.x = x1;
    position.y = y1;
    position.w = w;
    position.h = h;

    SDL_BlitSurface(image_surface,  &position, letter_surface, NULL);
    SDL_SaveBMP(letter_surface, filename);

    SDL_FreeSurface(letter_surface);
}

int nb_image = 0;

void drawallcolumn_and_cut(SDL_Surface *image_surface, size_t width,
    size_t h1, size_t h2)
{
    int w1 = 0;
    int w2 = 0;

    for (size_t w = 0; w < width; w++)
    {
        if (w2)
        {
            char s[20];
            sprintf(s, "image/seg_%d", nb_image++);
            Snap(image_surface, w1+1, h1+1, w2, h2, strcat(s,".bmp"));
            w2 = 0;
        }

        if (!column_isempty(image_surface, w, h1, h2))
        {
            if (w - 1 > 0 && column_isempty(image_surface, w - 1, h1, h2))
            {
                drawcolumn(image_surface, w - 1, h1, h2);
                w1 = w - 1;
            }

            if (w + 1 < width && column_isempty(image_surface, w + 1, h1, h2))
            {
                drawcolumn(image_surface, w + 1, h1, h2);
                w2 = w + 1;
            }
        }
    }
}

// Border

void WhiteContouring(SDL_Surface *image_surface, size_t width, size_t height)
{
    Uint32 pixel;

    for (size_t i = 0; i < width; i++)
    {
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
        put_pixel(image_surface, i, 0, pixel);
        put_pixel(image_surface, i, height - 1, pixel);
    }

    for (size_t j = 0; j < height; j++)
    {
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
        put_pixel(image_surface, 0, j, pixel);
        put_pixel(image_surface, width - 1, j, pixel);
    }
}


// Main

void Contour(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    int h1 = 0;
    int h2 = 0;

    WhiteContouring(image_surface, width, height);

    for (size_t j = 0; j < height; j++)
    {
        if (h2)
        {
            drawallcolumn_and_cut(image_surface, width, h1, h2);
            h2 = 0;
        }

        if (!line_isempty(image_surface, width, j))
        {
            if (j - 1 > 0 && line_isempty(image_surface, width, j - 1))
            {
                drawline(image_surface, width, j - 1);
                h1 = j - 1;
            }
            if (j + 1 < height && line_isempty(image_surface, width, j + 1))
            {
                drawline(image_surface, width, j + 1);
                h2 = j + 1;
            }
        }
    }
}
