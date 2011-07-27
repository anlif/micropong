#include "pong.h"
#include "draw.h"


void pong_init(){
	point_t paddle1_origin = {PADDLE_ONE_X, 
					YRES/2 - 6};
	point_t paddle2_origin = {PADDLE_TWO_X, 
					YRES/2 - 6};
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

// TODO: handle win/lose state?
int pong_move(enum PONG_PIECE piece, enum PONG_MOVE move){
	int offset;
	int num_points;
	int delta;
	point_t* piece_buffer = draw_get_back_buffer(); 

	switch(piece){
		case PADDLE_LEFT:
			offset = 0;
			num_points = PADDLE_SIZE;
			break;
		case PADDLE_RIGHT:
			offset = PADDLE_SIZE;
			num_points = PADDLE_SIZE;
			break;
		case BALL:
			offset = 2*PADDLE_SIZE;
			num_points = BALL_SIZE;
			break;
		default:
			return 1;
	}
	switch(move){
		case BALL_LEFT:
		case PADDLE_UP:
			delta = -1;
			break;
		case BALL_RIGHT:
		case PADDLE_DOWN:
			delta = 1;
			break;
		default:
			return 1;
	}
	

	for(int i = offset; i < offset + num_points; ++i){
		if(piece == BALL){
			if( IN_BOUNDS(piece_buffer[i].x + delta) ){
				piece_buffer[i].x += delta;
			}
			else{
				return 1;
			}
		}
		else{
			if( IN_BOUNDS(piece_buffer[i].y + delta) ){
				piece_buffer[i].y += delta;
			}
			else{
				return 1;
			}
		}
	}

	return 0;
}
