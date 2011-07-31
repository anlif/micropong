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
		printf("Drawing outside surface in drawRect! x:%i, y:%i, width:%i, height:%i\n", 
		x, y, width, height);
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

	if(SDL_EnableKeyRepeat(1, 1)){
		printf("Error in enabling key repeat");
	}

	ticks_last_draw = SDL_GetTicks();

}

void hw_draw(){
	static int print = 0;

	// clear screen to black
	drawRect(screen, 0,0,XRES*SCALE_FACTOR,YRES*SCALE_FACTOR,0);

	point_t *first_point = draw_next_point();
	hw_set_pixel(*first_point);
	
	if(print) printf("Point: {%i, %i}\n", first_point->x, first_point->y);

	point_t *current_point = draw_next_point();
	while(!(current_point->x == first_point->x && current_point->y == first_point->y)){
		hw_set_pixel(*current_point);
		current_point = draw_next_point();
		if(print) printf("Point: {%i, %i}\n", current_point->x, current_point->y);
	}

	print = 0;	
	SDL_UpdateRect(screen, 0, 0, 0, 0); // update entire screen

	int current_ticks = SDL_GetTicks();
	int delta = (current_ticks - ticks_last_draw) % (1000/DRAW_RATE);
	int delay = (1000/DRAW_RATE) - delta;
	SDL_Delay(delay);
}

static enum HW_INPUT_KEYS get_key(SDL_Event* event){
	switch(event->key.keysym.sym){
		case SDLK_UP:
			return TWO_UP;
		case SDLK_DOWN:
			return TWO_DOWN;
		case SDLK_w:
			return ONE_UP;
		case SDLK_s:
			return ONE_DOWN;
		default:
			// no relevant key
			return NO_INPUT;
	}
	// shouldnt really get here
	return NO_INPUT;
}

enum HW_INPUT_KEYS hw_get_input(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_KEYDOWN:
				return get_key(&event);
			case SDL_QUIT:
				return EXIT;
			default:
				// no relevant input
				break;
		}
	}
	return NO_INPUT;
}



void hw_deinit(){
	SDL_Quit();
}

