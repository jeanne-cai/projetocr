#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "pixel_operations.h"

//############################ Base ############################

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

void SDL_FreeSurface(SDL_Surface *surface);

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

void HdrawLine(SDL_Surface *image_surface, int height)
{
    int width = image_surface->w;
    Uint32 pixel;
    for (int i = 0; i < width; i++)
    {
        pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
        put_pixel(image_surface, i, height, pixel);
    }
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

void VdrawLine(SDL_Surface *image_surface, int xpos, int h1, int h2)
{
    Uint32 pixel;
    for (int i = h1; i < h2; i++)
    {
        pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
        put_pixel(image_surface, xpos, i, pixel);
    }
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
					VdrawLine(image_surface, k-1, h1, h2);
					count[0] = k-1;
                }
                if(VlineIsEmpty(image_surface, k+1, h1, h2))
                {
                    VdrawLine(image_surface, k+1, h1, h2);
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
					HdrawLine(image_surface, k-1);
					count[0] = k-1;
                }
                if(HlineIsEmpty(image_surface, k+1))
                {
                    HdrawLine(image_surface, k+1);
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
    SDL_Surface* screen_surface;
    image_surface = load_image("my_image.jpg");
    screen_surface = display_image(image_surface);
    
    init_sdl();

	//MAIN
    wait_for_keypressed(); //wait
    
    GrayScale(image_surface);
    
    update_surface(screen_surface, image_surface);
    
    wait_for_keypressed(); //wait
    
    Contour(image_surface);
    update_surface(screen_surface, image_surface);
    
    wait_for_keypressed(); //wait
    
    //END
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}
