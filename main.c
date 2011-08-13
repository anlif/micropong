#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "main.h"
#include "draw.h"
#include "pong.h"
#include "hw.h"

static bool restart = false;

static void init(){
	pong_init();
	hw_init();
}


int main(void){
	init();

	// Ultimate loop
	while(1){
		// flytte ting
		// nei
	
		//LEDPORT.OUT = ADCA.CH0.RESL;
		cli();
		if(restart){
			pong_restart();
			restart = false;
		}
		draw_swap_buffers();	
		draw_copy_buffers();
		sei();
	}
}


static bool move_flag = false;

/*
 move timing, ~20fps (NB: also controls ball move speed)
TODO: implement win/lose logic
*/
ISR(TCE0_OVF_vect){
	static uint8_t paddle_left_y = 0;
	static uint8_t paddle_right_y = 0;
	static uint8_t status = PONG_NO_ERROR;
	
	paddle_left_y = ADCA.CH0.RES >> 5;// - hw_get_ADC_min_shifted( HW_ADC_0, 5 );
	paddle_right_y = ADCA.CH1.RES >> 5;// - hw_get_ADC_min_shifted( HW_ADC_1, 5 );
	pong_move_paddle(PONG_PADDLE_LEFT, paddle_left_y);
	pong_move_paddle(PONG_PADDLE_RIGHT, paddle_right_y);
	
	status = pong_move_ball();
	if( PONG_NO_ERROR != status ){
		restart = true;
	}
}

