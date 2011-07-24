#ifndef PONG_H
#define PONG_H

// Point buffer sizes
#define PADDLE_WIDTH 2
#define PADDLE_HEIGHT 12
#define PADDLE_SIZE ((PADDLE_WIDTH)*(PADDLE_HEIGHT))
#define BALL_WIDTH 2
#define BALL_HEIGHT 2
#define BALL_SIZE ((BALL_WIDTH)*(BALL_HEIGHT)) 
#define PONG_BUFFER_SIZE ((2*(PADDLE_SIZE)) + (BALL_SIZE))

void pong_init();

#endif
