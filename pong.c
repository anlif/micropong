#include "pong.h"
#include "draw.h"
#include <stdint.h>

static uint8_t paddle_left_state_y;
static uint8_t paddle_right_state_y;
static point_t ball_state;

uint8_t pong_get_paddle_state(uint8_t piece){
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

	for(uint8_t i = 0; i < PADDLE_WIDTH; ++i){
		for(uint8_t j = 0; j < PADDLE_HEIGHT; j++){
			uint8_t x1 = paddle_left_origin.x + i;
			uint8_t y1 = paddle_left_origin.y + j;
			uint8_t x2 = paddle_right_origin.x + i;
			uint8_t y2 = paddle_right_origin.y + j;
			points[PADDLE_LEFT_OFFSET + i*PADDLE_HEIGHT+j].x = x1;
			points[PADDLE_LEFT_OFFSET + i*PADDLE_HEIGHT+j].y = y1;
			points[PADDLE_RIGHT_OFFSET + i*PADDLE_HEIGHT+j].x = x2;
			points[PADDLE_RIGHT_OFFSET + i*PADDLE_HEIGHT+j].y = y2;
		}
	}

	for(uint8_t i = 0; i < BALL_WIDTH; ++i){
		for(uint8_t j = 0; j < BALL_HEIGHT; j++){
			uint8_t x = ball_origin.x + i;
			uint8_t y = ball_origin.y + j;
			points[BALL_OFFSET + i*BALL_HEIGHT + j].x = x;
			points[BALL_OFFSET + i*BALL_HEIGHT + j].y = y;
		}
	}
	
	for(uint8_t i = 0; i < LINE_SIZE; ++i){
		points[LINE_OFFSET + i].x = XRES/2;
		points[LINE_OFFSET + i].y = (i/LINE_SEP_SIZE)*LINE_SEP_SIZE + i + LINE_SEP_SIZE/2;
	}

	draw_init(PONG_BUFFER_SIZE,  points);
}

// Returns 
uint8_t pong_move_ball(){
	static int8_t dx = 1;
	static int8_t dy = 0;
	static uint8_t offset = 2*PADDLE_SIZE;
	static uint8_t hits = 0;

	point_t* piece_buffer = draw_get_back_buffer();	
	
	// If collision, calc new dx/dy
	// Left paddle collision:
	if(ball_state.x <= PADDLE_LEFT_X+PADDLE_WIDTH && dx < 0){
		// One of two, paddle right wins or ball bounces to the right 
		if(ball_state.y >= paddle_left_state_y && ball_state.y <= paddle_left_state_y+PADDLE_HEIGHT){
			dx = -dx;
			++hits;
		}
		else{
			return PONG_WIN_RIGHT;
		}
		dy = (dy+MAGIC_DY_FUNCTION( ball_state.y, paddle_left_state_y ) & DY_MAX);
	}
	
	if(ball_state.x >= PADDLE_RIGHT_X-BALL_WIDTH && dx > 0){
		// One of two, paddle left wins or ball bounces to the left
		if(ball_state.y >= paddle_right_state_y && ball_state.y <= paddle_right_state_y+PADDLE_HEIGHT){
			dx = -dx;
			++hits;
		}
		else{
			return PONG_WIN_LEFT;
		}
		dy = ((dy+MAGIC_DY_FUNCTION(ball_state.y, paddle_right_state_y)) & DY_MAX);
	}

	if(hits == DX_INTERVAL){
		dx += dx > 0? 1 : -1;
		hits = 0;
	}
	
	if(ball_state.y == 0 || ball_state.y+BALL_HEIGHT == YRES ){
		dy = -dy;
	}
	
	while(ball_state.y+BALL_HEIGHT + dy > YRES){
		dy -= 1;
	}

	while(ball_state.y + dy < 0){
		dy += 1;
	}

	while(ball_state.x+BALL_WIDTH + dx > XRES){
		dx -= 1;
	}

	while(ball_state.x + dx < 0){
		dx += 1;
	}
	
	ball_state.x = piece_buffer[offset].x + dx;
	ball_state.y = piece_buffer[offset].y + dy;
	for(uint8_t i = offset; i < offset+BALL_SIZE; ++i){
		piece_buffer[i].x += dx;
		piece_buffer[i].y += dy;
	}

	return PONG_NO_ERROR;
}

uint8_t pong_move_paddle(uint8_t piece, uint8_t y){
	uint8_t offset;
	point_t* piece_buffer = draw_get_back_buffer(); 		

	if(!IN_BOUNDS_Y(y) || !IN_BOUNDS_Y(y+PADDLE_HEIGHT-2)){ 
		return PONG_MOVE_ERROR;
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
			return PONG_ERROR;
	}
	
	// TODO: OPTIMIZE?? memset or something? move both paddles at the same time?
	for(uint8_t i = 0; i < PADDLE_WIDTH; ++i){
		for(uint8_t j = 0; j < PADDLE_HEIGHT; ++j){
			piece_buffer[offset+i*PADDLE_HEIGHT + j].y = y+j;
		}
	}
	
	return PONG_NO_ERROR;
}
