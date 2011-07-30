#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

// Screen parameters
// NOTE: both XRES and YRES
#define XRES 	64
#define YRES 	64
#define BUFFER_MAX_POINTS ((XRES)*(YRES))

typedef struct {
	uint8_t x;
	uint8_t y;
} point_t;

typedef struct {
	point_t *buffer;
	int num_points;
} drawbuffer_t;

void draw_init(int num_points, point_t* data);

point_t* draw_next_point();

point_t* draw_get_back_buffer();

void draw_swap_buffers();

#endif
