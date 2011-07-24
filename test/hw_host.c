#include <SDL/SDL.h>
#include <stdlib.h>
#include "../draw.h"
#include "hw_host.h"


static SDL_Surface* screen;
static int ticks_last_draw;


static void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

static void drawRect(SDL_Surface* surface, int x, int y, int width, int height,  Uint8 color)
{
	// lock surface before drawing
	SDL_LockSurface( surface );
	if( x >= surface->w || y >= surface->h || x < 0 || y < 0)
	{
		printf("Drawing outside surface in drawRect!\n");
		exit(1);
	}
	// just be nice and resize rectangle if it's too big
	if( x + width >= surface->w )
		width = surface->w - x;
	if( y + height >= surface->h )
		height = surface->h - y;

	
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			putPixel(surface, x + j, y + i, color);
		}
	}

	// Unlock surface, done drawing
	SDL_UnlockSurface( surface );
}

static void hw_set_pixel(point_t point){
	drawRect(screen, point.x*SCALE_FACTOR, point.y*SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR, COLOR_GREEN);
}

void hw_init(){
	// init SDL 
	if(SDL_Init(SDL_INIT_VIDEO)){
		printf("Error in init video: %s\n", SDL_GetError());
		exit(1);
	}

	screen  = SDL_SetVideoMode(XRES*SCALE_FACTOR, YRES*SCALE_FACTOR, 8, SDL_SWSURFACE);
	if(screen == NULL){
		printf("Error in getting surface: %s\n", SDL_GetError());
		exit(1);
	}

	ticks_last_draw = SDL_GetTicks();

}

void hw_draw(){
	// clear screen to black
	SDL_Color black = {0, 0, 0, 0};	
	SDL_SetColors(screen, &black, 0, 1);	


	point_t first_point = draw_next_point();
	hw_set_pixel(first_point);

	point_t current_point = draw_next_point();
	while(!(current_point.x == first_point.x && current_point.y == first_point.y)){
		hw_set_pixel(current_point);
		current_point = draw_next_point();
	}

	
	SDL_UpdateRect(screen, 0, 0, 0, 0); // update entire screen

	int current_ticks = SDL_GetTicks();
	int delta = (current_ticks - ticks_last_draw) % (1000/DRAW_RATE);
	int delay = (1000/DRAW_RATE) - delta;
	SDL_Delay(delay);

}

int hw_quit(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT) return 1;
	}
	return 0;
}

void hw_deinit(){
	SDL_Quit();
}

