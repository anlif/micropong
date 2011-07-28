#include "pong.h"
#include "draw.h"
#include <stdio.h>

static int paddle_left_state_y;
static int paddle_right_state_y;
static point_t ball_state;

int pong_get_paddle_state(int piece){
	switch(piece){
		case PONG_PADDLE_LEFT:
			return paddle_left_state_y;
		case PONG_PADDLE_RIGHT:
			return paddle_right_state_y;
		default:
			return -1;
	}
}

void pong_init(){
	point_t paddle_left_origin = {PADDLE_LEFT_X,
					YRES/2 - PADDLE_HEIGHT/2};
	paddle_left_state_y = paddle_left_origin.y;

	point_t paddle_right_origin = {PADDLE_RIGHT_X, 
					YRES/2 - PADDLE_HEIGHT/2};
	paddle_right_state_y = paddle_right_origin.y;

	point_t ball_origin = {XRES/2-3, YRES/2-3};
	ball_state.x = ball_origin.x; ball_state.y = ball_origin.y;

	point_t points[PONG_BUFFER_SIZE];

	for(int i = 0; i < PADDLE_WIDTH; ++i){
		for(int j = 0; j < PADDLE_HEIGHT; j++){
			int x1 = paddle_left_origin.x + i;
			int y1 = paddle_left_origin.y + j;
			int x2 = paddle_right_origin.x + i;
			int y2 = paddle_right_origin.y + j;
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
			points[2*PADDLE_SIZE + i*BALL_HEIGHT + j].x = x;
			points[2*PADDLE_SIZE + i*BALL_HEIGHT + j].y = y;
		}
	}

	draw_init(PONG_BUFFER_SIZE,  points);
}

// Returns 
int pong_move_ball(){
	static int dx = 1;
	static int dy = 0;
	static int offset = 2*PADDLE_SIZE;

	point_t* piece_buffer = draw_get_back_buffer();	
	
	// If collision, calc new dx/dy
	// Left paddle collision:
	if(ball_state.x == PADDLE_LEFT_X+PADDLE_WIDTH && dx < 0){
		// One of two, paddle right wins or ball bounces to the right 
		if(ball_state.y >= paddle_left_state_y){
			dx = 1;
		}
		else{
			return PONG_WIN_RIGHT;
		}

		dy += MAGIC_DY_FUNCTION( ball_state.y, paddle_left_state_y );
	}
	
	if(ball_state.x == PADDLE_RIGHT_X-BALL_WIDTH && dx > 0){
		// One of two, paddle left wins or ball bounces to the left
		if(ball_state.y >= paddle_right_state_y){
			dx = -1;
		}
		else{
			return PONG_WIN_LEFT;
		}
		
		dy += MAGIC_DY_FUNCTION( ball_state.y, paddle_right_state_y );
	}
	
	if(ball_state.y == 0){
		dy = -dy;
	}
	if(ball_state.y+BALL_HEIGHT == YRES){
		dy = -dy;
	}

	if(!IN_BOUNDS_Y(ball_state.y+dy)){
		dy = 0;
	}

	ball_state.x = piece_buffer[offset].x + dx;
	ball_state.y = piece_buffer[offset].y + dy;
	for(int i = offset; i < offset+BALL_SIZE; ++i){
		piece_buffer[i].x += dx;
		piece_buffer[i].y += dy;
	}
}

int pong_move_paddle(int piece, int y){
	int offset;
	int delta;
	point_t* piece_buffer = draw_get_back_buffer(); 		

	if(!IN_BOUNDS_Y(y) || !IN_BOUNDS_Y(y+PADDLE_HEIGHT)){ 
		return 1;
	}

	switch(piece){
		case PONG_PADDLE_LEFT:
			offset = 0;
			paddle_left_state_y = y;
			break;
		case PONG_PADDLE_RIGHT:
			offset = PADDLE_SIZE;
			paddle_right_state_y = y;
			break;
		default:
			// error
			return 1;
	}



	for(int i = 0; i < PADDLE_WIDTH; ++i){
		for(int j = 0; j < PADDLE_HEIGHT; ++j){
			piece_buffer[offset+i*PADDLE_HEIGHT + j].y = y+j;
		}
	}
	
	return 0;
}
