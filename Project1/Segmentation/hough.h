#ifndef HOUGH_H
#define HOUGH_H

void Gauss(SDL_Surface *image_surface);
void Contraste(SDL_Surface *image_surface);
int Hough_Transform(SDL_Surface *image_surface);
SDL_Surface* Rotate(SDL_Surface* origine, float angle);
void image_to_matrix(SDL_Surface *image_surface, double m[], size_t w, size_t h);
#endif
