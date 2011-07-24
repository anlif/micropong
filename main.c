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


	LEDPORT.OUT = OSC.CTRL;
	
}


int main(void){
	init();

	// Ultimate loop
	while(1){
		// flytte ting
		// nei
		
		hw_wait(); // waits for next move interrupt (timing)
		
		cli();
		draw_swap_buffers();	
		sei();
	}
}


