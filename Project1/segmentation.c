#include "sdl_base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hough.h"
#include "network.h"

int nb_image = 0;


// ---- Isempty

int line_isempty(SDL_Surface *image_surface, size_t width, size_t h_pos)
{
    Uint8 r, g, b;
    for (size_t i = 0; i < width; i++)
    {
        Uint32 pixel = get_pixel(image_surface, i, h_pos);
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

        if (r < 200)
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

        if (r < 200)
            return 0;
    }
    return 1;
}


// ---- Draw

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


// ---- Resize

// Get the pixel x, y of the surface "surface", the component c (3 components RBG)
unsigned char GetPixelComp32(SDL_Surface* surface,int x,int y,int c)
{
    unsigned char *p;
    p = ((unsigned char*)surface->pixels) + y * surface->pitch + x * 4;
    return p[c];
}

// Write the component c (3 component RGB) to the pixel x, y of the surface "surface"
void PutPixelComp32(SDL_Surface* surface, size_t x, size_t y,
    size_t c, unsigned char val)
{
    unsigned char *p;
    p = ((unsigned char*)surface->pixels) + y * surface->pitch + x * 4;
    p[c] = val;
}

void Stretch_Nearest(SDL_Surface *src, SDL_Surface *dest)
{
    double rx, ry;

    rx = dest->w * 1.0 / src->w;
    ry = dest->h * 1.0 / src->h;

    for (int i = 0; i < dest->w; i++)
    {
        for (int j = 0; j < dest->h; j++)
        {
            for (size_t k = 0; k < 3; k++)
            {
                Uint32 pixel = get_pixel(src, (int)(i / rx), (int)(j / ry));
                put_pixel(dest, i, j, pixel);
                /*unsigned char pix;
                pix = GetPixelComp32(src, (int)(i / rx), (int)(j / ry), k);
                PutPixelComp32(dest, i, j, k, pix);*/
			}
        }
    }
}

SDL_Surface* Strechblit(SDL_Surface *src, size_t w, size_t h)
{
    SDL_Surface* img;

    img = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
    Stretch_Nearest(src, img);
    //Stretch_Linear(src, img);

    return img;
}

SDL_Surface* resize(SDL_Surface *letter_surface, size_t w, size_t h)
{
    return Strechblit(letter_surface, w, h);
}


// ---- Letter

SDL_Surface* edge(SDL_Surface *letter_surface, size_t addw)
{
    SDL_Surface* dest;
    size_t x = 0;

    dest = SDL_CreateRGBSurface(SDL_SWSURFACE, 28, 28, 32, 255, 255, 255, 0);
    for (size_t i = 0; i < 28; i++)
    {
        for (size_t j = 0; j < 28; j++)
        {
            if (i >= addw && i < 28 - addw - 1)
            {
                for (size_t k = 0; k < 3; k++)
                {
                    Uint32 pixel = get_pixel(letter_surface, x, j);
                    put_pixel(dest, i, j, pixel);
                    /*unsigned char pix;
                    pix = GetPixelComp32(letter_surface, x, j, k);
                    PutPixelComp32(dest, i, j, k, pix);*/
                }
            }
            else
            {
                Uint32 pixel = get_pixel(dest, i, j);
                pixel = SDL_MapRGB(dest->format, 255, 255, 255);
                put_pixel(dest, i, j, pixel);
            }
        }

        if (i >= addw && i < 28 - addw - 1)
            x++;
    }
    return dest;
}

SDL_Surface* center(SDL_Surface *letter_surface, size_t w)
{
    if (w > 28)
    {
        letter_surface = resize(letter_surface, 26, 28);
        return edge(letter_surface, 1);
    }
    else
    {
        letter_surface = resize(letter_surface, w, 28);
        return edge(letter_surface, (28 - w) / 2);
    }
}

// Change letter in white et backgroung in black
void letter_grayscale(SDL_Surface *letter_surface, size_t width, size_t height)
{
    Uint8 r, g, b;

    for (size_t i = 0; i < width; i++)
    {
        for (size_t j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(letter_surface, i, j);
            SDL_GetRGB(pixel, letter_surface->format, &r, &g, &b);
/*
            if (r == 255)
                r = 0;
            else
                r = 255;

            pixel = SDL_MapRGB(letter_surface->format, r, r, r);
            put_pixel(letter_surface, i, j, pixel);

            if (r + g - 255 < b)
            {
                pixel = SDL_MapRGB(letter_surface->format, 0, 0, 0);
                r = b;
                g = b;
            }*/
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;
            pixel = SDL_MapRGB(letter_surface->format, r, g, b);
            put_pixel(letter_surface, i, j, pixel);
        }
    }
}

char Snap(SDL_Surface *image_surface, size_t x, size_t y,
    size_t x1, size_t y1, const char* filename,Network *network)
{
	size_t w = x1 - x;
    size_t h = y1 - y;

	SDL_Surface *letter_surface;
    SDL_Surface *nletter_surface;
    SDL_Rect position;

    double matrix_image[784];
    char c;

    letter_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32,
                        255, 255, 255, 0);
	position.x = x;
	position.y = y;
	position.w = w;
	position.h = h;

	SDL_BlitSurface(image_surface, &position, letter_surface, NULL);

    nletter_surface = center(letter_surface, w);
    letter_grayscale(nletter_surface, 28, 28);

    SDL_SaveBMP(nletter_surface, filename);
    
    image_to_matrix(nletter_surface, matrix_image, 28, 28);
    c = arr_to_char(matrix_image, network);
    printf("%c\n", c);
    for (size_t j = 0; j < 28; j++)
    {
        for (size_t k = 0; k < 28; k++)
        {
            float f = matrix_image[j * 28 + k] / 255;
            if (f > 0.1)
            {
                printf("\033[41m");
            }
            printf("%.1f \033[00m",f);
        }
        printf("\n");
    }

    SDL_FreeSurface(letter_surface);
    SDL_FreeSurface(nletter_surface);

    return c;
}

void string_double_capacity(char **string, int *size)
{
    *size *= 2;
    *string = realloc(*string, *size * sizeof(char));
}


// ---- Segmentation

void drawallcolumn_and_cut(SDL_Surface *image_surface, size_t width,
    size_t h1, size_t h2,Network *network)
{
    int w1 = 0;
    int w2 = 0;
    
    char c;
    int index = 0, size = 1;
    char *string = malloc(sizeof(char));

	for (size_t w = 0; w < width; w++)
    {
    	if (w2)
    	{
    		char s[20];
            sprintf(s, "image/seg_%d", nb_image++);

    		c = Snap(image_surface, w1 + 1, h1 + 1, w2, h2,
                    strcat(s,".bmp"), network);
            if (index == size - 1)
                string_double_capacity(&string, &size);
            string[index] = c;
            index++;

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

    string[index] = '\0';
    printf("\n\nstring :\n%s\n", string);
}

// Border White
void WhiteCountouring(SDL_Surface *image_surface, size_t width, size_t height)
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


// Main : Segmentation

void Segmentation(SDL_Surface *image_surface)
{
    Network network;

    size_t width = image_surface->w;
    size_t height = image_surface->h;

    int h1 = 0, h2 = 0;

    size_t nbCorrect;
    Dataset data_set;
    initialiseDataSet(&data_set, "dataSetfile.csv");

    load_network(&network, "neuralnet82.2.net");
    nbCorrect = evaluateNetwork(&network, &data_set);
    printf("\n %ld éléments reconnus correct sur %ld, %.3f\n",
            nbCorrect,data_set.size, (float)nbCorrect/data_set.size);
    // exit(1);

    WhiteCountouring(image_surface, width, height);

    for (size_t j = 0; j < height; j++)
    {
        if (h2)
        {
            drawallcolumn_and_cut(image_surface, width, h1, h2,&network);
            printf("%s\n", " retour");
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
