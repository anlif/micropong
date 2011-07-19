#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"

static inline void swap_buffers(){
	bool (*temp)[XRES] = read_buffer;
	read_buffer = write_buffer;
	write_buffer = temp;
}

static inline void clear_buffer(bool val){
	for(int i = 0; i < YRES; ++i){
		for(int j = 0; j < XRES; ++j){
			write_buffer[i][j] = val;
		}
	}
}


static void init(){
	init_hw();

	//Enable medium levels interrupt globally
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_HILVLEN_bm;

	LEDPORT.OUT = OSC.CTRL;
	
	// clear both buffers
	clear_buffer(false);
	swap_buffers();
	clear_buffer(false);
	
	//Enable global interrupts
	sei();
}

void swap(int* x, int* y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

inline static void draw_pixel(int x, int y){
	write_buffer[y][x] = true;
}

void draw_rect(int x, int y, int width, int height){
	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			write_buffer[y+i][x+j] = true;
		}
	}
}

int main(void){
	init();


	// Ultimate loop
	while(1){
		while(!move_flag){ 
			/* wait for interrupt */ 
		}
		move_flag = false;
		// flytte ting
		pong_move();
		
		cli();
		swap_buffers();	
		sei();
	}
}



// painting interrupt
ISR(TCE1_OVF_vect){
	// x and y index
	static uint8_t yi = 0;
	static uint8_t xi = 0;
	static bool draw_y = true;
	static bool increment = false;
	
	if (increment) {
		// Assuming XRES/YRES is 2^n for any n >= 1
		// a & (2^n - 1) == a % 2^n, but faster.
		do{
			if( xi == XRES-1 ){
				yi = (yi + 1) & (YRES -1);
				xi = 0;
				draw_y = true;
				continue;
			}
			xi++;
		} while( read_buffer[yi][xi] == false );
	}
	if(draw_y) {
		draw_y = false;
		DACA.CH0DATA = ylookup[yi];
		increment = false;
	} else {
		DACA.CH1DATA = xlookup[xi];
		increment = true;
	}
}

ISR(TCE0_OVF_vect){
	move_flag = true;		
}
