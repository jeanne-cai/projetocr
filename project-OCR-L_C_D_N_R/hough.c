#include <math.h>
#include "sdl_base.h"

#define highThresholdingRatio 0.075
#define lowThresholdingRatio 0.175


// ---- Tools matrix

double* init_matrix(size_t len)
{
    double* m = malloc(len * sizeof(double));

    for (size_t i = 0; i < len; i++)
        m[i] = 0;

    return m;
}

void image_to_matrix(SDL_Surface *image_surface, double m[], size_t w, size_t h)
{
    Uint8 r, g, b;
    for(size_t i = 0; i < h; i++)
    {
        for(size_t j = 0; j < w; j++)
        {
            Uint32 pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            m[i * w + j] = r;
        }
    }
}

void matrix_to_image(SDL_Surface *image_surface, size_t w, size_t h, double m[])
{
    for (size_t i = 0; i < h; i++)
    {
        for(size_t j = 0; j < w; j++)
        {
            Uint32 pixel = get_pixel(image_surface, j, i);
            Uint8 average = (Uint8)m[i * w + j] % 256;

            pixel = SDL_MapRGB(image_surface->format,
                      average, average, average);

            put_pixel(image_surface, j, i, pixel);
        }
    }
}

void convolution(double m[], double k[], int w, int h, int k_w, int k_h, double r[])
{
    int kx_m = k_h / 2;
    int ky_m = k_w / 2;

    for (int x = 0; x < h; x++)
    {
        for (int y = 0; y < w; y++)
        {
            for (int i = 0; i < k_h; i++)
            {
                for (int j = 0; j < k_w; j++)
                {
                    int ix = x + i - kx_m;
                    int iy = y + j - ky_m;

                    if (ix < 0 || iy < 0 || ix > h - 1 || iy > w - 1)
                        continue;
                    r[x * w + y] += k[i * k_w + j] * m[ix * w + iy];
                }
            }
        }
    }
}

// ---- Gauss
void Gauss(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    double *m = init_matrix(width * height);
    double *mGauss = init_matrix(width * height);

    image_to_matrix(image_surface, m, width, height);

    double G[] =
    {
        1, 2, 1,
        2, 4, 2,
        1, 2, 1
    };

    // Apply Gaussian filter : smooth, remove the noise
    convolution(m, G, width, height, 3, 3, mGauss);

    matrix_to_image(image_surface, width, height, mGauss);

    free(mGauss);
    free(m);
}

// ---- Contraste
void Contraste(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    double *m = init_matrix(width * height);
    double *mCon = init_matrix(width * height);

    image_to_matrix(image_surface, m, width, height);

/*    double C[] =
    {
        -1, -1, -1,
        -1, 3, -1,
        -1, -1, -1
    };
*/

    double C[] =
    {
        0, -1, 0,
        -1, 5, -1,
        0, -1, 0
    };
/*   double C[] =
   {
       1, 4, 6, 4, 1,
       4, 16, 24, 16, 4,
       6, 24, -476, 24, 6,
       4, 16, 24, 16, 4,
       1, 4, 6, 4, 1
   };

    for (size_t i = 0; i < width * height; i++)
        mCon[i] = mCon[i] * (-1.0 / 256.0);

    convolution(m, C, width, height, 5, 5, mCon);
*/
    convolution(m, C, width, height, 3, 3, mCon);
    matrix_to_image(image_surface, width, height, mCon);

    free(mCon);
    free(m);
}

// ---- Canny edge detector

void gradient(double gx[], double gy[], size_t w, size_t h, double g[])
{
    for (size_t i = 0; i < (w * h); i++)
    {
        g[i] = sqrt(gx[i] * gx[i] + gy[i] * gy[i]);
    }
}

void non_maximum_suppression(double g[], double gx[], double gy[], size_t w, size_t h, double nm[])
{
    double alpha;

    for (size_t i = 1; i < h - 1; i++)
    {
        for (size_t j = 1; j < w - 1; j++)
        {
            if (gx[i * w + j] == 0)
                alpha = 90;
            else
                alpha = (atan(gy[i * w + j] / gx[i * w + j]) * 180 / M_PI);

            if (alpha < 0)
                alpha += 180;

            //angle 0
            if ((0 <= alpha && alpha < 22.5) || (157.5 <= alpha && alpha <= 180))
            {
                if ((g[i * w + j] < g[i * w + j+1]) || (g[i * w + j] < g[i * w + j-1]))
                    nm[i * w +j] = 0;
            }
            //angle 45
            else if (22.5 <= alpha && alpha < 67.5)
            {
                if ((g[i * w + j] < g[(i+1) * w + j-1]) || (g[i * w + j] < g[(i-1) * w + j+1]))
                    nm[i * w +j] = 0;
            }
            //angle 90
            else if (67.5 <= alpha && alpha < 112.5)
            {
                if ((g[i * w + j] < g[(i+1) * w + j]) || (g[i * w + j] < g[(i-1) * w + j]))
                    nm[i * w +j] = 0;
            }
            //angle 135
            else if (112.5 <= alpha && alpha < 157.5)
            {
                if ((g[i * w + j] < g[(i-1) * w + j-1]) || (g[i * w + j] < g[(i+1) * w + j+1]))
                    nm[i * w +j] = 0;
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

    for(size_t i = 1; i < h - 1; i++)
    {
        for(size_t j = 1; j < w - 1; j++)
        {
            if (g[i * w + j] < T_Low)
                g[i * w + j] = 0;
            else if (g[i * w + j] > T_High)
                g[i * w + j] = 255;
            else
                g[i * w + j] = 128;
        }
    }
}

void hysteresis_thresholding(double g[], size_t w, size_t h)
{
    for (size_t i = 1; i < h - 1; i++)
    {
        for(size_t j = 1; j < w - 1; j++)
        {
            if (g[i * w + j] == 128)
            {
                if (g[(i+1) * w + j] == 255 || g[(i-1) * w + j] == 255 ||
                      g[i * w + j+1] == 255 || g[i * w + j-1] == 255 ||
                      g[(i-1) * w + j-1] == 255 || g[(i-1) * w + j+1] == 255 ||
                      g[(i+1) * w + j+1] == 255 || g[(i+1) * w + j-1] == 255)
                    g[i * w + j] = 255;
                else
                    g[i * w + j] = 0;
            }
        }
    }
}

// Main : Canny filter

void canny_edge_detection(double *m, size_t width, size_t height)
{
    double *mGauss = init_matrix(width * height);
    double *mX = init_matrix(width * height);
    double *mY = init_matrix(width * height);
    double *G = init_matrix(width * height);

    double B[] =
    {
        2, 4, 5, 4, 2,
        4, 9, 12, 9, 4,
        5, 12, 15, 12, 5,
        4, 9, 12, 9, 4,
        2, 4, 5, 4, 2
    };

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

    for (size_t i = 0; i < 25; i++)
        B[i] = B[i] / 159;

    // Apply Gaussian filter : smooth, remove the noise
    convolution(m, B, width, height, 5, 5, mGauss);

    // Find the intensity gradients of the image
    convolution(mGauss, KGx, width, height, 3, 3, mX);
    convolution(mGauss, KGy, width, height, 3, 3, mY);

    gradient(mX, mY, width, height, G);
    gradient(mX, mY, width, height, m);

    // Apply non-maximum suppression
    non_maximum_suppression(G, mX, mY, width, height, m);

    // Apply double threshold
    double_thresholding(m, width, height);

    // Apply edge tracking by hysteresis
    hysteresis_thresholding(m, width, height);

    free(mGauss);
    free(mX);
    free(mY);
    free(G);
}


// ---- Hough Transform

// Main : Hough Transform

// Indentification of lines in the image
// Returns the angle in degrees
int Hough_Transform(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    int RHO = sqrt(width * width + height * height) * 2;
    int THETA = 180;

    double rho = 0, theta_max = 0, f_max = 0;
    double *f = init_matrix(RHO * THETA);
    double *m = init_matrix(width * height);

    // m : matrix that represents the image
    image_to_matrix(image_surface, m, width, height);

    // Apply Canny filter
    canny_edge_detection(m, width, height);
//    matrix_to_image(image_surface, width, height, m);

    // Uses the parametric representation of a line
    // rho : distance perpendicular to the line
    // theta : angle of (x, rho)
    for (size_t x = 1; x < height - 1; x++)
    {
        for (size_t y = 1; y < width - 1; y++)
        {
            if (m[x * width + y] > 0)
            {
                for (int theta = 0; theta < THETA; theta++)
                {
                    rho = y * cos(theta * M_PI / 180) + x * sin(theta * M_PI / 180);
                    f[(int)(theta * RHO + rho)]++;
                }
            }
        }
    }

    // Find theta by looking for the maximum in the histogram of f[]
    for (int t = 0; t < THETA; t++)
    {
        for (int r = 0; r < RHO; r++)
        {
            if ((int)f[t * RHO + r] > f_max)
            {
                f_max = (int)f[t * RHO + r];
                theta_max = t;
            }
        }
    }

    // -90 <= theta < 90
    theta_max -= 90;

    free(m);
    free(f);

//    printf("theta_max = %d\n", (int)theta_max);
    return (int)theta_max;
}

/*// Unsharp maskinig 5x5
void Unsharp(SDL_surface *image_surface)
{
    double *m = init_matrix(width * height);
    double *m_final = init_matrix(width * height);

    double M =
    {
        1, 4, 6, 4, 1,
        4, 16, 24, 16, 4,
        6, 24, -476, 24, 6,
        4, 16, 24, 16, 4,
        1, 4, 6, 4, 1
    };

    for (size_t i = 0; i < 25; i++)
        M[i] = M[i] * (-1 / 256);

    image_to_matrix(image_surface, m, width, height);
    convolution(m, M, width, height, 5, 5, m_final);
    matrix_to_image(image_surface, width, height, m_final);

    free(m_final);
    free(m);
}*/

// ---- Rotation

// Main : Rotate

// Make a central rotation of angle in degrees
SDL_Surface* Rotate(SDL_Surface* origine, float angle)
{
    Uint32 pixel;
    int bx, by;

    // Angle in radians
    float angle_radian = -angle * M_PI / 180.0;

    // Find the size of the image dest
    float tcos = cos(angle_radian);
    float tsin = sin(angle_radian);

    double width_dest = ceil(origine->w * fabs(tcos) + origine->h * fabs(tsin));
    double height_dest = ceil(origine->w * fabs(tsin) + origine->h * fabs(tcos));

    SDL_Surface* dest = SDL_CreateRGBSurface(SDL_HWSURFACE, width_dest,
                            height_dest, origine->format->BitsPerPixel,
                            origine->format->Rmask, origine->format->Gmask,
                            origine->format->Bmask, origine->format->Amask);

    if(dest == NULL)
        return NULL;

    int mxdest = dest->w / 2;
    int mydest = dest->h / 2;
    int mx = origine->w / 2;
    int my = origine->h / 2;

    for (int j = 0; j < dest->h; j++)
    {
        for (int i = 0; i < dest->w; i++)
        {
            // Find the best position on the orignal surface
            // by applying a reverse rotation matrix
            bx = ceil(tcos * (i - mxdest) + tsin * (j - mydest) + mx);
            by = ceil(-tsin * (i - mxdest) + tcos * (j - mydest) + my);

            if (bx >= 0 && bx < origine->w && by >= 0 && by < origine->h)
            {
                pixel = get_pixel(origine, bx, by);
                put_pixel(dest, i, j, pixel);
            }
            else
            {
              pixel = SDL_MapRGB(dest->format, 255, 255, 255);
              put_pixel(dest, i, j, pixel);
            }
        }
    }

    return dest;
}
