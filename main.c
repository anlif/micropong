#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "main.h"
#include "draw.h"
#include "pong.h"
#include "hw.h"

/* Takes in an ADC value, and returns a pong paddle y coordinate */
static uint8_t get_paddle_y( uint8_t inp ){
		

}

static void init(){
	hw_init();
	pong_init();
}


int main(void){
	init();

	// Ultimate loop
	while(1){
		// flytte ting
		// nei
	
		//LEDPORT.OUT = ADCA.CH0.RESL;
		cli();
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
	
	paddle_left_y = ADCA.CH0.RES >> 5 - hw_get_ADC_min_shifted( HW_ADC_0, 5 );
	paddle_right_y = ADCA.CH1.RES >> 5 - hw_get_ADC_min_shifted( HW_ADC_1, 5 );
	
	pong_move_ball();
	pong_move_paddle(PONG_PADDLE_LEFT, paddle_left_y);
	pong_move_paddle(PONG_PADDLE_RIGHT, paddle_right_y);


}

