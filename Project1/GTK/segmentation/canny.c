#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pixel_operations.h"

#define highThresholdingRatio 0.075
#define lowThresholdingRatio 0.175

double* init_matrix(size_t len)
{
    double* m = malloc(len * sizeof(double));

    for (size_t i = 0; i < len; i++)
        m[i] = 0;

    return m;
}
void print_matrix(char s[], double m[], size_t rows, size_t cols)
{
    printf("%s =\n", s);
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            printf("%4g ", m[i * cols + j]);
        }
        printf("\n");
    }
}

void image_to_matrix(SDL_Surface *image_surface, double m[], size_t w, size_t h)
{
    Uint8 r, g, b;
    for(size_t i = 0; i < w; i++)
    {
        for(size_t j = 0; j < h; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            m[i * h + j] = r;
        }
    }
}

void matrix_to_image (SDL_Surface *image_surface, size_t w, size_t h, double m[])
{
    for(size_t i = 0; i < w; i++)
    {
        for(size_t j = 0; j < h; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            Uint8 average = (Uint8)m[i*h+j] % 256;

            pixel = SDL_MapRGB(image_surface->format,
                      average, average, average);

            put_pixel(image_surface, i, j, pixel);
        }
    }
}

void convolution(double m[], double k[], int w, int h, int k_w, int k_h, double r[])
{
    int kx_m = k_w / 2;
    int ky_m = k_h / 2;

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            for (int i = 0; i < k_w; i++)
            {
                for (int j = 0; j < k_h; j++)
                {
                    int ix = x + i - kx_m;
                    int iy = y + j - ky_m;

                    if (ix < 0 || iy < 0 || ix > w - 1 || iy > h - 1)
                        continue;
                    r[x * h + y] += k[i * k_h + j] * m[ix * h + iy];
                }
            }
        }
    }
}

void gradient(double gx[], double gy[], size_t w, size_t h, double g[])
{
    for(size_t i = 0; i < w; i++)
    {
        for(size_t j = 0; j < h; j++)
        {
            g[i*h+j] = sqrt(gx[i*h+j] * gx[i*h+j] + gy[i*h+j] * gy[i*h+j]);
        }
    }
}

void non_maximum_suppression(double g[], double gx[], double gy[], size_t w, size_t h, double nm[])
{
    double alpha;

    for (size_t i = 1; i < w - 1; i++)
    {
        for (size_t j = 1; j < h - 1; j++)
        {
            if (gx[i * h + j] == 0)
                alpha = 90;
            else
                alpha = (atan(gy[i * h + j] / gx[i * h + j]) * 180 / M_PI);

            if (alpha < 0)
                alpha += 180;

            //angle 0
            if ((0 <= alpha && alpha < 22.5) || (157.5 <= alpha && alpha <= 180))
            {
                if ((g[i * h + j] < g[i * h + j+1]) || (g[i * h + j] < g[i * h + j-1]))
                    nm[i * h +j] = 0;
            }
            //angle 45
            else if (22.5 <= alpha && alpha < 67.5)
            {
                if ((g[i * h + j] < g[(i+1) * h + j-1]) || (g[i * h + j] < g[(i-1) * h + j+1]))
                    nm[i * h +j] = 0;
            }
            //angle 90
            else if (67.5 <= alpha && alpha < 112.5)
            {
                if ((g[i * h + j] < g[(i+1) * h + j]) || (g[i * h + j] < g[(i-1) * h + j]))
                    nm[i * h +j] = 0;
            }
            //angle 135
            else if (112.5 <= alpha && alpha < 157.5)
            {
                if ((g[i * h + j] < g[(i-1) * h + j-1]) || (g[i * h + j] < g[(i+1) * h + j+1]))
                    nm[i * h +j] = 0;
            }
        }
    }
}

void double_thresholding(double g[], size_t w, size_t h)
{
    double t_max = 0;

    for(size_t i = 0; i < (w * h); i++)
    {
        if (t_max < g[i])
            t_max = g[i];
    }

    double T_High = t_max * highThresholdingRatio;
    double T_Low = T_High * lowThresholdingRatio;

    for(size_t i = 1; i < w - 1; i++)
    {
        for(size_t j = 1; j < h - 1; j++)
        {
            if (g[i*h+j] < T_Low)
                g[i*h+j] = 0; 
            else if (g[i*h+j] > T_High)
                g[i*h+j] = 255;
            else
                g[i*h+j] = 128;
        }
    }
}

void hysteresis_thresholding(double g[], size_t w, size_t h)
{
    for(size_t i = 1; i < w - 1; i++)
    {
        for(size_t j = 1; j < h - 1; j++)
        {
            if (g[i * h + j] == 128)
            {
                if (g[(i+1) * h + j] == 255 || g[(i-1) * h + j] == 255 ||
                      g[i * h + j+1] == 255 || g[i * h + j-1] == 255 ||
                      g[(i-1) * h + j-1] == 255 || g[(i-1) * h + j+1] == 255 ||
                      g[(i+1) * h + j+1] == 255 || g[(i+1) * h + j-1] == 255)
                    g[i*h+j] = 255;
                else
                    g[i*h+j] = 0;
            }
        }
    }
}

void canny_edge_detection(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    double *m = init_matrix(width * height);
    image_to_matrix(image_surface, m, width, height);

    double B[] =
    {
        2, 4, 5, 4, 2,
        4, 9, 12, 9, 4,
        5, 12, 15, 12, 5,
        4, 9, 12, 9, 4,
        2, 4, 5, 4, 2
    };

    for (size_t i = 0; i < 25; i++)
        B[i] = B[i] / 159;

    double *mGauss = init_matrix(width * height);
    convolution(m, B, width, height, 5, 5, mGauss);

    double KGx[] =
    {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };

    double KGy[] =
    {
        1, 2, 1,
        0, 0, 0,
        -1, -2, -1
    };
    double *mX = init_matrix(width * height);
    double *mY = init_matrix(width * height);
    double *G = init_matrix(width * height);

    convolution(mGauss, KGx, width, height, 3, 3, mX);
    convolution(mGauss, KGy, width, height, 3, 3, mY);
    gradient(mX, mY, width, height, G);
    gradient(mX, mY, width, height, m);
    non_maximum_suppression(G, mX, mY, width, height, m);
    double_thresholding(m, width, height);
    hysteresis_thresholding(m, width, height);
    matrix_to_image(image_surface, width, height, m);
//    print_matrix("m", m, width, height);

    free(m);
    free(mGauss);
    free(mX);
    free(mY);
    free(G);
}
