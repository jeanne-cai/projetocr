#include "sdl_base.h"

//############################ Horizontal traitement ############################

int HlineIsEmpty(SDL_Surface *image_surface, int height)
{
    int width = image_surface->w;
    for (int i = 0; i < width; i++)
    {
        Uint32 pixel = get_pixel(image_surface, i, height);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        if (r < 127)
        {
            return 0;
        }
    }
    return 1;
}


//############################ Snap ############################

void Snap(SDL_Surface *image_surface,int x,int y, int x1, int y1, const char* filename)
{
	int w = x1 - x;
	int h = y1 - y;
	SDL_Surface* letter_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0, 0, 0, 0);
	SDL_Rect position;
	position.x = x;
	position.y = y;
	position.w = w;
	position.h = h;
	SDL_BlitSurface(image_surface,&position,letter_surface,NULL);
	SDL_SaveBMP(letter_surface, filename);
}


//############################ Vertical traitement ############################

int VlineIsEmpty(SDL_Surface *image_surface, int xpos, int h1, int h2)
{
    for (int i = h1; i < h2; i++)
    {
        Uint32 pixel = get_pixel(image_surface, xpos, i);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        if (r < 127)
        {
            return 0;
        }
    }
    return 1;
}


int nb_image = 0;

void VContour(SDL_Surface *image_surface, int h1, int h2)
{
    int width = image_surface->w;
    int count[2] = {0, 0};
    Uint8 r, g, b;
    
	for (int k = 0; k < width; k++)
    {	
    	if (count[1])
    	{
    		nb_image++;
    		char s[20];
			sprintf(s, "image/%d", nb_image);
    		Snap(image_surface, count[0]+1, h1+1, count[1], h2, strcat(s,".png"));
    		count[1] = 0;
    	}
    	
        for (int l = h1; l < h2; l++)
        {
            Uint32 pixel = get_pixel(image_surface, k, l);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if (r < 127)
            {
                if(VlineIsEmpty(image_surface, k-1, h1, h2))
                {
					count[0] = k-1;
                }
                if(VlineIsEmpty(image_surface, k+1, h1, h2))
                {
                    count[1] = k+1;
                }
            }
        }
    } 
}

//############################ Main contouring function ############################

void WhiteCountouring(SDL_Surface *image_surface)
{
    int width = image_surface->w;
    int height = image_surface->h;
    Uint32 pixel;
    for (int i = 0; i < width; i++)
    {
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
        put_pixel(image_surface, i, 0, pixel);
        put_pixel(image_surface, i, height-1, pixel);
    }
    for (int j = 0; j < height; j++)
    {
        pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
        put_pixel(image_surface, 0, j, pixel);
        put_pixel(image_surface, width-1, j, pixel);
    }
}


void Contour(SDL_Surface *image_surface)
{
    int width = image_surface->w;
    int height = image_surface->h;
    WhiteCountouring(image_surface);
    int count[2] = {0, 0};
    Uint8 r, g, b;
    
	for (int k = 0; k < height; k++)
    {
    	if (count[1])
    	{
    		VContour(image_surface, count[0], count[1]);
    		count[1] = 0;
    	}
    	
        for (int l = 0; l < width; l++)
        {
            Uint32 pixel = get_pixel(image_surface, l, k);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if (r < 127)
            {
                if(HlineIsEmpty(image_surface, k-1))
                {
					count[0] = k-1;
                }
                if(HlineIsEmpty(image_surface, k+1))
                {
                    count[1] = k+1;
                }
            }
        }
    } 
}

//############################ GrayScale ############################

void GrayScale(SDL_Surface *image_surface)
{
    int width = image_surface->w;
    int height = image_surface->h;
    Uint8 r, g, b;
    
	for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            Uint8 average = 0.3*r + 0.59*g + 0.11*b;
            pixel = SDL_MapRGB(image_surface->format, average, average, average);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}


//########################################################



int main()
{
	//INIT
    SDL_Surface* image_surface;
    image_surface = load_image("image/0image.jpg");
    
    init_sdl();
    
    GrayScale(image_surface);
    
    Contour(image_surface);
    
    //END
    SDL_FreeSurface(image_surface);

    return 0;
}