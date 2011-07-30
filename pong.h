#ifndef PONG_H
#define PONG_H

#include "draw.h" // for bounds check

// Point buffer sizes
#define PADDLE_WIDTH 2
#define PADDLE_HEIGHT 12
#define PADDLE_SPACING 2
#define PADDLE_SIZE ((PADDLE_WIDTH)*(PADDLE_HEIGHT))
#define PADDLE_LEFT_X PADDLE_SPACING 
#define PADDLE_RIGHT_X ((XRES) - (PADDLE_WIDTH) - (PADDLE_SPACING))

#define BALL_WIDTH 2
#define BALL_HEIGHT 2
#define BALL_SIZE ((BALL_WIDTH)*(BALL_HEIGHT)) 
#define PONG_BUFFER_SIZE ((2*(PADDLE_SIZE)) + (BALL_SIZE))

#define IN_BOUNDS_X( x ) (( x >= 0) && (x < (XRES)))
#define IN_BOUNDS_Y( y ) (( y >= 0) && (y < (YRES)))

#define MAGIC_DY_FUNCTION( ball_state_y, paddle_state_y )  ((ball_state_y+(BALL_HEIGHT/2)) - \
								(paddle_state_y + (PADDLE_HEIGHT/2)))

#define DY_MAX 3
#define DX_INTERVAL 2

enum {
	PONG_PADDLE_LEFT,
	PONG_PADDLE_RIGHT,
	PONG_BALL,
	PONG_WIN_LEFT,
	PONG_WIN_RIGHT,
	PONG_MOVE_ERROR,

};

void pong_init();

// Move a paddle, returns 0 on success, 1 on failure
int pong_move_paddle(int piece, int y);
int pong_get_paddle_state(int piece);

#endif
