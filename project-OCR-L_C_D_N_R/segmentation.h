#ifndef SEGMENTATION_H
#define SEGMENTATION_H

SDL_Surface* resize(SDL_Surface *letter_surface, int w, int h);
void GrayScale(SDL_Surface *image_surface);
void Segmentation(SDL_Surface *image_surface, SDL_Surface *copy_surface,char string0[]);
void string_double_capacity(char **string,int *size);

#endif
