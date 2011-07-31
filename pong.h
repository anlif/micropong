#ifndef PONG_H
#define PONG_H

#include "draw.h" // for bounds check
#include <stdint.h>

// SIZE = number of points

#define PADDLE_WIDTH 2
#define PADDLE_HEIGHT 8
#define PADDLE_SPACING 2
#define PADDLE_SIZE ((PADDLE_WIDTH)*(PADDLE_HEIGHT))
#define PADDLE_LEFT_X PADDLE_SPACING 
#define PADDLE_RIGHT_X ((XRES) - (PADDLE_WIDTH) - (PADDLE_SPACING))

#define BALL_WIDTH 2
#define BALL_HEIGHT 2
#define BALL_SIZE ((BALL_WIDTH)*(BALL_HEIGHT)) 

#define LINE_WIDTH 1
#define LINE_SEP_SIZE 2
#define LINE_PARTS ((YRES)/(LINE_SEP_SIZE))/2
#define LINE_SIZE ((LINE_SEP_SIZE)*(LINE_PARTS)*(LINE_WIDTH))

#define NUMBER_SIZE 34
#define NUMBER_WIDTH 7
#define NUMBER_HEIGHT 7

#define NUMBER1_X 55
#define NUMBER2_X 66
#define NUMBER_Y 2

#define PADDLE_LEFT_OFFSET 0
#define PADDLE_RIGHT_OFFSET PADDLE_SIZE
#define BALL_OFFSET ((PADDLE_RIGHT_OFFSET)+(PADDLE_SIZE))
#define LINE_OFFSET ((BALL_OFFSET)+(BALL_SIZE))
#define NUMBER1_OFFSET ((LINE_OFFSET)+(LINE_SIZE))
#define NUMBER2_OFFSET ((NUMBER1_OFFSET)+(NUMBER_SIZE))

#define PONG_BUFFER_SIZE ((2*(PADDLE_SIZE)) + (BALL_SIZE) + (LINE_SIZE) + 2*(NUMBER_SIZE))

#define IN_BOUNDS_X( x ) (( x >= 0) && (x < (XRES)))
#define IN_BOUNDS_Y( y ) (( y >= 0) && (y < (YRES)))

#define MAGIC_DY_FUNCTION( ball_state_y, paddle_state_y )  ((ball_state_y+(BALL_HEIGHT/2)) - \
								(paddle_state_y + (PADDLE_HEIGHT/2)))

// if you change this to a non power of two - 1, remember to change to modulo in move_ball
#define DY_MAX 3 

#define DX_INTERVAL 4 

enum {
	PONG_NO_ERROR = 0,
	PONG_ERROR,
	PONG_PADDLE_LEFT,
	PONG_PADDLE_RIGHT,
	PONG_BALL,
	PONG_WIN_LEFT,
	PONG_WIN_RIGHT,
	PONG_MOVE_ERROR
};

void pong_init();

// Move a paddle, returns 0 on success, 1 on failure
uint8_t pong_move_paddle(uint8_t piece, uint8_t y);
uint8_t pong_get_paddle_state(uint8_t piece);
uint8_t pong_move_ball();
void pong_restart(uint8_t last_winner);
void pong_draw_score(uint8_t piece, uint8_t number);
void pong_draw_number(point_t* buf, uint8_t number,
		      uint8_t orig_x, uint8_t orig_y);

#endif
