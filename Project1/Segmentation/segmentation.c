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

        if (r < 210)
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

        if (r < 210)
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
void Stretch_Nearest(SDL_Surface *src, SDL_Surface *dest)
{
    double rx, ry;

    rx = dest->w * 1.0 / src->w;
    ry = dest->h * 1.0 / src->h;

    for (int i = 0; i < dest->w; i++)
    {
        for (int j = 0; j < dest->h; j++)
        {
                Uint32 pixel = get_pixel(src, (int)(i / rx), (int)(j / ry));
                put_pixel(dest, i, j, pixel);
        }
    }
}

SDL_Surface* Strechblit(SDL_Surface *src, size_t w, size_t h)
{
    SDL_Surface* img;

    img = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
    Stretch_Nearest(src, img);

    return img;
}

SDL_Surface* resize(SDL_Surface *letter_surface, size_t w, size_t h)
{
    return Strechblit(letter_surface, w, h);
}


// ---- Letter

SDL_Surface* Wedge(SDL_Surface *letter_surface, size_t addw, size_t nh)
{
    SDL_Surface* dest;
    size_t x = 0;
    size_t w=letter_surface->w;
    dest = SDL_CreateRGBSurface(SDL_SWSURFACE, 28, nh, 32, 255, 255, 255, 0);
    for (size_t i = 0; i < 28; i++)
    {
        for (size_t j = 0; j < nh; j++)
        {
            if (i >= addw && i<28-addw-1)
            {
                Uint32 pixel = get_pixel(letter_surface, x, j);
                put_pixel(dest, i, j, pixel);
            }
            else
            {
                Uint32 pixel = SDL_MapRGB(dest->format, 255, 255, 255);
                put_pixel(dest, i, j, pixel);
            }
        }

        if (i >= addw && x<w){
          x++;
        }
    }
    return dest;
}

SDL_Surface* Hedge(SDL_Surface *letter_surface, size_t addh, size_t nw)
{
    SDL_Surface* dest;
    size_t y = 0;
    size_t h=letter_surface->h;
    dest = SDL_CreateRGBSurface(SDL_SWSURFACE, 28, 28, 32, 255, 255, 255, 0);
    for (size_t i = 0; i < nw; i++)
    {
        for (size_t j = 0; j < 28; j++)
        {
            if (j >= addh && y<h)
            {
                    Uint32 pixel = get_pixel(letter_surface, i, y);
                    put_pixel(dest, i, j, pixel);
                    /*unsigned char pix;
                    pix = GetPixelComp32(letter_surface, x, j, k);
                    PutPixelComp32(dest, i, j, k, pix);*/
                    y++;
            }
            else
            {
                Uint32 pixel = SDL_MapRGB(dest->format, 255, 255, 255);
                put_pixel(dest, i, j, pixel);
            }
        }
        y=0;
    }
    return dest;
}

SDL_Surface* center(SDL_Surface *letter_surface)
{
  size_t w=letter_surface->w;
  size_t h=letter_surface->h;
  //rx = dest->w * 1.0 / src->w;
  //ry = dest->h * 1.0 / src->h;
  if(w>h){
    double dnh =  h * 1.0 / w;
    printf("%lu\n", w);
    printf("%lu\n", h);
    printf("%f\n", dnh);
    int nh=(int) (dnh*26);
    letter_surface = resize(letter_surface, 26, nh);
    printf("%s\n", "oui");
    letter_surface = Wedge(letter_surface, 1,nh);
    return Hedge(letter_surface, (28 - nh)/2, 28);
  }
  else
  {
    double dnw = w * 1.0 / h;
    printf("%f\n", dnw);
    int nw=(int) (dnw*26);
    letter_surface = resize(letter_surface,nw, 26);
    printf("%s\n", "ui");
    letter_surface = Hedge(letter_surface, 1,nw);
    return Wedge(letter_surface, (28-nw)/2,28);
  }
}

size_t Minh(SDL_Surface *letter_surface, size_t w,size_t h)
{
  for (size_t i = 0; i < h; i++){
    if (!line_isempty(letter_surface,w,i)){
      return i;
    }
  }
  return h;
}
size_t Minw(SDL_Surface *letter_surface, size_t w,size_t h)
{
  for (size_t i = 0; i < w; i++){
    if (!column_isempty(letter_surface,i,0,h)){
      return i;
    }
  }
  return w;
}
size_t Maxh(SDL_Surface *letter_surface, size_t w,size_t h)
{
  for (size_t i = h-1; 0 < i; i--){
    if (!line_isempty(letter_surface,w,i)){
      return i;
    }
  }
  return 0;
}
size_t Maxw(SDL_Surface *letter_surface, size_t w,size_t h)
{
  for (size_t i = w; 0 < i; i--){
    if (!column_isempty(letter_surface,i,0,h)){
      return i;
    }
  }
  return 0;
}
SDL_Surface* cut(SDL_Surface *letter_surface)
{
  size_t w = letter_surface->w;
  size_t h = letter_surface->h;
  size_t minh=Minh(letter_surface,w,h);
  size_t maxh=Maxh(letter_surface,w,h);
  size_t minw=Minw(letter_surface,w,h);
  size_t maxw=Maxw(letter_surface,w,h);

  w = (maxw - minw);
  h = (maxh - minh)+1;
    SDL_Surface *nletter_surface;
    SDL_Rect position;

    nletter_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32,
                        255, 255, 255, 0);
	position.x = minw;
	position.y = minh;
	position.w = w;
	position.h = h;

	SDL_BlitSurface(letter_surface, &position, nletter_surface, NULL);
  return nletter_surface;
}

//------Snap-------------

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

  letter_surface =cut(letter_surface);
  letter_surface = center(letter_surface);
  letter_grayscale(letter_surface, 28, 28);

    SDL_SaveBMP(letter_surface, filename);

    image_to_matrix(letter_surface, matrix_image, 28, 28);
    c = arr_to_char(matrix_image, network);
    //printf("%c\n", c);
    // for (size_t j = 0; j < 28; j++)
    // {
    //     for (size_t k = 0; k < 28; k++)
    //     {
    //         float f = matrix_image[j * 28 + k] / 255;
    //         if (f > 0.1)
    //         {
    //             printf("\033[41m");
    //         }
    //         printf("%.1f \033[00m",f);
    //     }
    //     printf("\n");
    // }

    SDL_FreeSurface(letter_surface);

    return c;
}

void string_double_capacity(char **string, int *size)
{
    *size *= 2;
    *string = realloc(*string, *size * sizeof(char));
}


// ---- Segmentation

int find_space(SDL_Surface *image_surface, size_t width,
    size_t h1, size_t h2)
{
    int w1 = 0;
    int w2 = 0;
    int w3 = 0;

    int ecart = -1;
    int space_min = 1000;
    int space_max = 0;

    for (size_t w = 0; w < width; w++)
    {
        if (w2)
        {
            if (w3)
            {
                ecart = w1 - w3;
                if (ecart > space_max)
                    space_max = ecart;
                if (ecart < space_min && ecart != 0)
                    space_min = ecart;
            }
            w3 = w2;
            w2 = 0;
        }

        if (!column_isempty(image_surface, w, h1, h2))
        {
            if (w - 1 > 0 && column_isempty(image_surface, w - 1, h1, h2))
                w1 = w - 1;

            if (w + 1 < width && column_isempty(image_surface, w + 1, h1, h2))
                w2 = w + 1;
        }
    }

    if (space_max - space_min > 5)
        return space_min;
    return -1;
}

char *drawallcolumn_and_cut(SDL_Surface *image_surface, SDL_Surface *copy_surface, size_t width,
    size_t h1, size_t h2,Network *network)
{
    int w1 = 0;
    int w2 = 0;

    int w3 = 0;
    int space = find_space(image_surface, width, h1, h2);
    int ecart = 0;

    char c;
    int index = 0, size = 1;
    char *string = malloc(sizeof(char));

	for (size_t w = 0; w < width; w++)
    {
    	if (w2)
    	{
            if (w3)
            {
                if (space > 0)
                {
                    ecart = w1 - w3;
                    if (ecart > space * 2)
                    {
                        c = ' ';
                        if (index == size - 1)
                            string_double_capacity(&string, &size);
                        string[index] = c;
                        index++;
                    }
                }
            }
            char s[20];
            sprintf(s, "image/seg_%d", nb_image++);

            c = Snap(copy_surface, w1 + 1, h1 + 1, w2, h2,
                strcat(s,".bmp"), network);
            if (index == size - 1)
                string_double_capacity(&string, &size);
            string[index] = c;
            index++;

            w3 = w2;
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
    string[index]='\n';
    string[index+1] = '\0';
    return string;
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

// void concat(char **string0,char *string,int *size)
// {
//     int i=0;
//     for(; *((*string0)+i)!='\0'; i++);
  
//     printf("i=%d\n",i);
//     for(int j = 0;string[j]!='\0'; j++, i++)
//     {
//         printf("suspens\n");
//         if(j == *size - 1){
//             printf("danslaboucle\n");
//             string_double_capacity(string0, size);
//         }
//         *((*string0)+i) = string[j];
//     }
//     *((*string0)+i) = '\0';
// }
// Main : Segmentation

void Segmentation(SDL_Surface *image_surface,SDL_Surface *copy_surface,char string0[])
{
    Network network;

    size_t width = image_surface->w;
    size_t height = image_surface->h;


    int h1 = 0, h2 = 0;

    char *string;

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
            string=drawallcolumn_and_cut(image_surface, copy_surface, width, h1, h2,&network);
            strcat(string0,string);
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
