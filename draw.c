#include "draw.h"
#include <string.h>

static drawbuffer_t buffer1;
static drawbuffer_t buffer2;
static drawbuffer_t *read_buffer;
drawbuffer_t *back_buffer;

void draw_init(int points, point_t* data){
	buffer1.num_points = points;
	buffer2.num_points = points;
	memcpy(buffer1.buffer, data, points*sizeof(point_t));
	memcpy(buffer2.buffer, data, points*sizeof(point_t));
	read_buffer = &buffer1;
	back_buffer = &buffer2;
}

point_t* draw_get_back_buffer(){
	return back_buffer->buffer;
}

point_t* draw_next_point(){
	static int index = 0;
	index = (index+1) % read_buffer->num_points;
	return &(read_buffer->buffer)[index];
}

void draw_swap_buffers(){
	drawbuffer_t* temp = read_buffer;
	read_buffer = back_buffer;
	back_buffer = temp;
}

void draw_copy_buffers(){
	memcpy(back_buffer->buffer, read_buffer->buffer, (back_buffer->num_points)*sizeof(point_t));
}

