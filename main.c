#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "main.h"
#include "draw.h"
#include "pong.h"
#include "hw.h"


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
	pong_move_ball();
}

ISR(ADCA_CH0_vect){
	LEDPORT.OUTTGL = ADCA.CH0.RES;
}

ISR(ADCA_CH1_vect){

}
