#ifndef PONG_H
#define PONG_H

#include "draw.h" // for bounds check

// Point buffer sizes
#define PADDLE_WIDTH 2
#define PADDLE_HEIGHT 12
#define PADDLE_SPACING 2
#define PADDLE_SIZE ((PADDLE_WIDTH)*(PADDLE_HEIGHT))
#define PADDLE_ONE_X PADDLE_SPACING 
#define PADDLE_TWO_X ((XRES) - (PADDLE_WIDTH) - (PADDLE_SPACING))

#define BALL_WIDTH 2
#define BALL_HEIGHT 2
#define BALL_SIZE ((BALL_WIDTH)*(BALL_HEIGHT)) 
#define PONG_BUFFER_SIZE ((2*(PADDLE_SIZE)) + (BALL_SIZE))

#define IN_BOUNDS( x ) (( x >= 0) && (x < (XRES)))

enum PONG_PIECE {
	PADDLE_LEFT,
	PADDLE_RIGHT,
	BALL
};

enum PONG_MOVE {
	PADDLE_UP,
	PADDLE_DOWN,
	BALL_RIGHT,
	BALL_LEFT
};

void pong_init();

// Move a piece, returns 0 on success, 1 on failure
int pong_move(enum PONG_PIECE piece, enum PONG_MOVE move);

#endif
