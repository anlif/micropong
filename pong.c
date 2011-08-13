#include "pong.h"
#include "draw.h"
#include <stdint.h>

static uint8_t paddle_left_state_y;
static uint8_t paddle_right_state_y;
static uint8_t score[2];
static ball_state_t ball_state;

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

static void pong_place_ball( point_t position, point_t* point_buffer){
	for(uint8_t i = 0; i < BALL_WIDTH; ++i){
		for(uint8_t j = 0; j < BALL_HEIGHT; j++){
			uint8_t x = position.x + i;
			uint8_t y = position.y + j;
			point_buffer[BALL_OFFSET + i*BALL_HEIGHT + j].x = x;
			point_buffer[BALL_OFFSET + i*BALL_HEIGHT + j].y = y;
		}
	}
}

void pong_init(){
	static int first_run = 1;
	score[PONG_PADDLE_LEFT] = score[PONG_PADDLE_RIGHT] = 0;

	point_t paddle_left_origin = {PADDLE_LEFT_X,
					YRES/2 - PADDLE_HEIGHT/2};
	paddle_left_state_y = paddle_left_origin.y;

	point_t paddle_right_origin = {PADDLE_RIGHT_X, 
					YRES/2 - PADDLE_HEIGHT/2};
	paddle_right_state_y = paddle_right_origin.y;

	point_t ball_origin = {XRES/2-3, YRES/2-3};
	ball_state.pos.x = ball_origin.x; 
	ball_state.pos.y = ball_origin.y;
	ball_state.dx = -1; 
	ball_state.dy = 0;

	point_t points[PONG_BUFFER_SIZE];
	for (uint8_t i = 0; i < PONG_BUFFER_SIZE; i++) {
		points[i].x = 0;
		points[i].y = 0;
	}

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

	pong_place_ball(ball_origin, points);
	
	for(uint8_t i = 0; i < LINE_SIZE; ++i){
		points[LINE_OFFSET + i].x = XRES/2 - 1;
		points[LINE_OFFSET + i].y = (i/LINE_SEP_SIZE)*LINE_SEP_SIZE + i + LINE_SEP_SIZE/2;
	}
	
	pong_draw_number(&points[NUMBER1_OFFSET], 0, NUMBER1_X, NUMBER_Y);
	pong_draw_number(&points[NUMBER2_OFFSET], 0, NUMBER2_X, NUMBER_Y);

	if(first_run) {
		draw_init(PONG_BUFFER_SIZE,  points);
		first_run = 0;
	}
	
}


void pong_restart(){
	score[PONG_PADDLE_LEFT] = score[PONG_PADDLE_RIGHT] = 0;
	pong_init();
}

uint8_t pong_handle_score( uint8_t paddle ){
	score[paddle]++;
	if(score[paddle] > 9){
		return paddle == PONG_PADDLE_RIGHT ? PONG_WIN_RIGHT : PONG_WIN_LEFT;
	}

	switch( paddle ){
		case PONG_PADDLE_RIGHT:
			ball_state.dx = -1;
			ball_state.dy = 0;
			break;
		case PONG_PADDLE_LEFT:
			ball_state.dx = 1;
			ball_state.dy = 0;
			break;
	}
	ball_state.pos.x = XRES/2-3; 
	ball_state.pos.y = YRES/2-3;
	pong_place_ball( ball_state.pos, draw_get_back_buffer());
	pong_draw_score(paddle, score[paddle]);
	return PONG_NO_ERROR;
}

#define SCALE_DOWN 4
#define ABS( x ) ( x < 0? -x : x )
#define CLAMP_X( x ) ( (x >= XRES)? (XRES - 3) : ((x < 0)? 3 : x) )
#define CLAMP_Y( y ) ( (y >= YRES)? (YRES - 3) : ((y < 0)? 3 : y) )

#define PADDLE_IDLE_FUNCTION( p1_x, p1_y, p2_x, p2_y ) \
	{ ((XRES/2 - p1_x)/8) + ( ABS( p1_x - p2_x ) < 4 ? p2_y/8 : 0 ), \
	 ( ((YRES/2 - p1_y)/8) + ( ABS( p1_x - p2_x ) < 4 ? p2_x/8 : 0) )}


#define PADDLE_IDLE_FUNCTION2( p1_x, p1_y ) { (p1_x + (XRES/2 - p1_x)%3) , (p1_y + (YRES/2 - p1_y)%3) }

void pong_idle(){
	static point_t orb = {XRES/2, YRES/2};
	
	point_t *points = draw_get_back_buffer();

	for(uint8_t i = 0; i < PADDLE_WIDTH; ++i){
		for(uint8_t j = 0; j < PADDLE_HEIGHT; ++j){
			uint8_t l_offs = PADDLE_LEFT_OFFSET + i*PADDLE_HEIGHT + j;
			uint8_t r_offs = PADDLE_RIGHT_OFFSET + i*PADDLE_HEIGHT + j;
			
			delta_t d_l = { (XRES/2 - points[l_offs].x)/8, (YRES/2 - points[l_offs].y)/8 };
			delta_t d_r = { (XRES/2 - points[r_offs].x)/8, (YRES/2 - points[r_offs].y)/8 };

			points[l_offs].x = CLAMP_X(points[l_offs].x + d_l.x);	
			points[l_offs].y = CLAMP_Y(points[l_offs].y + d_l.y);	
			points[r_offs].x = CLAMP_X(points[r_offs].x + d_r.x);	
			points[r_offs].y = CLAMP_Y(points[r_offs].y + d_r.y);	
		}
	}
}

uint8_t pong_move_ball(){
	static uint8_t offset = 2*PADDLE_SIZE;
	static uint8_t hits = 0;

	point_t* piece_buffer = draw_get_back_buffer();	
	
	// If collision, calc new dx/dy
	// Left paddle collision:
	if(ball_state.pos.x <= PADDLE_LEFT_X+PADDLE_WIDTH && ball_state.dx < 0){
		// One of two, paddle right wins or ball bounces to the right 
		if(ball_state.pos.y >= paddle_left_state_y && ball_state.pos.y <= paddle_left_state_y+PADDLE_HEIGHT){
			ball_state.dx = -ball_state.dx;
			++hits;
		}
		else{
			return pong_handle_score( PONG_PADDLE_RIGHT );
		}
		ball_state.dy = (ball_state.dy+MAGIC_DY_FUNCTION( ball_state.pos.y, paddle_left_state_y ) & DY_MAX);
	}
	
	if(ball_state.pos.x >= PADDLE_RIGHT_X-BALL_WIDTH && ball_state.dx > 0){
		// One of two, paddle left wins or ball bounces to the left
		if(ball_state.pos.y >= paddle_right_state_y && ball_state.pos.y <= paddle_right_state_y+PADDLE_HEIGHT){
			ball_state.dx = -ball_state.dx;
			++hits;
		}
		else{
			return pong_handle_score( PONG_PADDLE_LEFT );
		}
		ball_state.dy = ((ball_state.dy+MAGIC_DY_FUNCTION(ball_state.pos.y, paddle_right_state_y)) & DY_MAX);
	}

	if(hits == DX_INTERVAL){
		ball_state.dx += ball_state.dx > 0? 1 : -1;
		hits = 0;
	}
	
	if(ball_state.pos.y == 0 || ball_state.pos.y+BALL_HEIGHT == YRES ){
		ball_state.dy = -ball_state.dy;
	}
	
	while(ball_state.pos.y+BALL_HEIGHT + ball_state.dy > YRES){
		ball_state.dy -= 1;
	}

	while(ball_state.pos.y + ball_state.dy < 0){
		ball_state.dy += 1;
	}

	while(ball_state.pos.x+BALL_WIDTH + ball_state.dx > XRES){
		ball_state.dx -= 1;
	}

	while(ball_state.pos.x + ball_state.dx < 0){
		ball_state.dx += 1;
	}
	
	ball_state.pos.x = piece_buffer[offset].x + ball_state.dx;
	ball_state.pos.y = piece_buffer[offset].y + ball_state.dy;
	
	pong_place_ball(ball_state.pos, piece_buffer);
#if 0
	for(uint8_t i = offset; i < offset+BALL_SIZE; ++i){
		piece_buffer[i].x += ball_state.dx;
		piece_buffer[i].y += ball_state.dy;
	}
#endif

	return PONG_NO_ERROR;
}

uint8_t pong_move_paddle(uint8_t piece, uint8_t y){
	uint8_t offset;
	point_t* piece_buffer = draw_get_back_buffer(); 		
	
	// FIXME: Why do I need - 2 here????
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

void pong_draw_score(uint8_t piece, uint8_t number)
{
	point_t* piece_buffer = draw_get_back_buffer(); 		

	uint8_t pos, orig_x, orig_y;
	if (piece == PONG_PADDLE_LEFT) {
		pos = NUMBER1_OFFSET;
		orig_x = NUMBER1_X;
	} else {
		pos = NUMBER2_OFFSET;
		orig_x = NUMBER2_X;
	}
	orig_y = NUMBER_Y;

	point_t* buf = piece_buffer + pos;

	pong_draw_number(buf, number, orig_x, orig_y);
}

void pong_draw_number(point_t* buf, uint8_t number,
		      uint8_t orig_x, uint8_t orig_y)
{
	uint8_t i;

	switch (number) {
	case 0:
		for (i = 0; i < 6; i++) {
			buf[i].x = orig_x;
			buf[i].y = orig_y + i + 1;
			buf[6+i].x = orig_x + 1;
			buf[6+i].y = orig_y + i;
			buf[21+i].x = orig_x + 4;
			buf[21+i].y = orig_y + i;
			buf[28+i].x = orig_x + 5;
			buf[28+i].y = orig_y + i;
		}
		buf[12].x = orig_x + 1;
		buf[12].y = orig_y + 6;
		buf[27].x = orig_x + 4;
		buf[27].y = orig_y + 4;
		for (i = 13; i < 17; i++) {
			buf[i].x = orig_x + 2;
			buf[4+i].x = orig_x + 3;
		}
		buf[13].y = buf[17].y = orig_y;
		buf[14].y = buf[18].y = orig_y+1;
		buf[15].y = buf[19].y = orig_y+5;
		buf[16].y = buf[20].y = orig_y+6;
		break;
	case 1:
		for (i = 0; i < 6; i++) {
			buf[i].x = orig_x;
			buf[i].y = orig_y + i + 1;
		}
		for (i = 0; i < 28; i++) {
			buf[6+i].x = orig_x + 1 + i/7;
			buf[6+i].y = orig_y + i%7;
		}
		break;
	case 2:
	case 3:
	case 5:
		for (i = 0; i < 6; i++) {
			buf[i].x = buf[6+i].x = buf[14+i].x
				= buf[22+i].x = buf[28+i].x
				= orig_x + i;
			buf[i].y = orig_y;
			buf[6+i].y = orig_y + 1;
			buf[14+i].y = orig_y + 3;
			buf[22+i].y = orig_y + 5;
			buf[28+i].y = orig_y + 6;
		}
		buf[12].y = buf[13].y = orig_y + 2;
		buf[20].y = buf[21].y = orig_y + 4;
		buf[12].x = orig_x + 4;
		buf[13].x = orig_x + 5;
		buf[20].x = orig_x + 4;
		buf[21].x = orig_x + 5;
		if (number == 2) {
			buf[20].x = orig_x + 0;
			buf[21].x = orig_x + 1;
		} else if (number == 5) {
			buf[12].x = orig_x + 0;
			buf[13].x = orig_x + 1;
		}
		break;
	case 4:
		for (i = 0; i < 4; i++) {
			buf[i].x = orig_x;
			buf[i].y = orig_y + i;
		}
		for (i = 0; i < 5; i++) {
			buf[4+i].x = orig_x + 1;
			buf[4+i].y = orig_y + i;
		}
		for (i = 0; i < 2; i++) {
			buf[9+i].x = orig_x + 2;
			buf[11+i].x = orig_x + 3;
			buf[9+i].y = buf[11+i].y = orig_y + 3 + i;
		}
		for (i = 0; i < 21; i++) {
			buf[13+i].x = orig_x + 4 + i/7;
			buf[13+i].y = orig_y + i%7;
		}
		break;
	case 6:
		for (i = 0; i < 12; i++) {
			buf[i].x = orig_x + i%6;
			buf[i].y = orig_y + i/7;
		}
		buf[12].x = orig_x;
		buf[13].x = orig_x + 1;
		buf[12].y = buf[13].y = 2;
		for (i = 0; i < 6; i++) {
			buf[14+i].x = buf[28+i].x = orig_x + i;
			buf[14+i].y = 3;
			buf[28+i].y = 6;
		}
		for (i = 0; i < 4; i++) {
			buf[20+i].x = orig_x + i%2;
			buf[20+i].y = orig_y + i/2;
			buf[24+i].x = orig_x + 4 + i%2;
			buf[24+i].y = orig_y + 4 + i/2;
		}
		break;
	case 7:
		for (i = 0; i < 21; i++) {
			buf[i].x = orig_x + i%7;
			buf[i].y = orig_y + i/7;
		}
		buf[21].x = orig_x;
		buf[21].y = orig_y + 3;
		for (i = 0; i < 12; i++) {
			buf[22+i].x = orig_x + 4 + i%3;
			buf[22+i].y = orig_y + 3 + i/3;
		}
		break;
	case 8:
		for (i = 0; i < 4; i++) {
			buf[i].x = buf[30+i].x = orig_x + 1 + i;
			buf[i].y = orig_y;
			buf[30+i].y = orig_y + 6;
		}
		for (i = 0; i < 6; i++) {
			buf[4+i].x = buf[14+i].x = buf[24+i].x = orig_x + i;
			buf[4+i].y = orig_y + 1;
			buf[14+i].y = orig_y + 3;
			buf[24+i].y = orig_y + 5;
		}
		for (i = 0; i < 2; i++) {
			buf[10+i].x = buf[20+i].x = orig_x + i;
			buf[12+i].x = buf[22+i].x = orig_x + 4 + i;
			buf[10+i].y = buf[12+i].y = orig_y + 2;
			buf[20+i].y = buf[22+i].y = orig_y + 4;
		}
		break;
	case 9:
		for (i = 0; i < 6; i++) {
			buf[i].x = orig_x + i;
			buf[i].y = orig_y;
			buf[14+i].x = orig_x + i;
			buf[14+i].y = orig_y + 3;
		}
		for (i = 0; i < 2; i++) {
			buf[6+i].x = buf[10+i].x = orig_x + i;
			buf[8+i].x = buf[12+i].x = orig_x + 4 + i;
			buf[6+i].y = buf[8+i].y = orig_y + 1;
			buf[10+i].y = buf[12+i].y = orig_y + 2;
		}
		buf[20].x = orig_x + 4;
		buf[21].x = orig_x + 5;
		buf[20].y = buf[21].y = orig_y + 4;
		for (i = 0; i < 12; i++) {
			buf[22+i].x = orig_x + i%6;
			buf[22+i].y = orig_y + 5 + i/6;
		}
		break;
	}
}
