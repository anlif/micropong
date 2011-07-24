#include "pong.h"
#include "draw.h"

void pong_init(){
	point_t paddle1_origin = {2, YRES/2 - 6};
	point_t paddle2_origin = {XRES - 4, YRES/2 - 6};
	point_t ball_origin = {XRES/2-3, YRES/2-3};
	point_t points[PONG_BUFFER_SIZE];

	for(int i = 0; i < PADDLE_WIDTH; ++i){
		for(int j = 0; j < PADDLE_HEIGHT; j++){
			int x1 = paddle1_origin.x + i;
			int y1 = paddle1_origin.y + j;
			int x2 = paddle2_origin.x + i;
			int y2 = paddle2_origin.y + j;
			points[i*PADDLE_HEIGHT+j].x = x1;
			points[i*PADDLE_HEIGHT+j].y = y1;
			points[i*PADDLE_HEIGHT+j + PADDLE_SIZE].x = x2;
			points[i*PADDLE_HEIGHT+j + PADDLE_SIZE].y = y2;
		}
	}

	for(int i = 0; i < BALL_WIDTH; ++i){
		for(int j = 0; j < BALL_HEIGHT; j++){
			int x = ball_origin.x + i;
			int y = ball_origin.y + j;
			points[2*PADDLE_SIZE+i*BALL_HEIGHT + j].x = x;
			points[2*PADDLE_SIZE+i*BALL_HEIGHT + j].y = y;
		}
	}

	draw_init(PONG_BUFFER_SIZE,  points);
}
