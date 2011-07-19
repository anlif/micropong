#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"

static inline void paint(int y, int x){
	// CH0 = X, CH1 = y
	DACA.CH1DATA = xlookup[x];
	//DACA.CH1DATA = ylookup[y];
}

static inline void clear_buffer(bool val){
	for(int i = 0; i < BUFSZY; ++i){
		buffer[i][BUFSZX] = val;
		for(int j = 0; j < BUFSZX; ++j){
			buffer[i][j] = val;
	}}
}

static void calc_lookup(){
	// Calculate xlookup and ylookup
	//float unitx = (DACMAX-XOFFSET)/(float)XRES;
	//float unity = (DACMAX-YOFFSET)/(float)YRES;
	uint8_t unit = 32;

	for(int i = 0; i < BUFSZX; ++i)
		xlookup[i] = i*unit + XOFFSET;	
	for(int i = 0; i < BUFSZY; ++i)
		ylookup[i] = i*unit + YOFFSET;	
}

static void setup_clock(){
	// Oscillator setup, enable 32MHz clock
	OSC.CTRL |= OSC_RC32MEN_bm;
	// wait a bit for clock to stabilize
	_delay_ms(50);
	// CLock prescaling: 1, set 32MHz as clock source
	CCP = CCP_IOREG_gc; // needed to write to CLK.CTRL register 
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
	CLK.PSCTRL = CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;
	// disable 2MHz clock to save power
	_delay_ms(50);
	OSC.CTRL = OSC_RC32MEN_bm;
}

static void setup_DAC(){
	// Setup DAC, allow as fast as possible converts
	DACA.CTRLA |= DAC_CH1EN_bm | DAC_CH0EN_bm | DAC_ENABLE_bm;
	DACA.CTRLB |= DAC_CHSEL_DUAL_gc;
	DACA.CTRLC |= DAC_REFSEL_AVCC_gc;
	DACA.TIMCTRL &= ~DAC_CONINTVAL_gm;
	DACA.TIMCTRL = DAC_CONINTVAL_128CLK_gc | DAC_REFRESH_OFF_gc;
}

static void setup_timers(){
	//Setup timer1: framebuffer output, with overflow interrupt.
	TCE1.CTRLA = TC_CLKSEL_DIV1_gc;
	TCE1.INTCTRLA = (TCE1.INTCTRLA & ~TC1_OVFINTLVL_gm) | TC_OVFINTLVL_HI_gc;
	TCE1.PER = 0x100; 


	//Setup timer2: other stuff with overflow interrupt
	TCE0.CTRLA = TC_CLKSEL_DIV1024_gc;
	TCE0.INTCTRLA = (TCE0.INTCTRLA & ~TC1_OVFINTLVL_gm) | TC_OVFINTLVL_MED_gc;
	TCE0.PER = 0x28B0;
}
static void init_hw(){
	setup_clock();
	setup_DAC();	
	setup_timers();	
}

static void init(){
	/*Setup LEDPORT as the output port*/
	LEDPORT.DIR = 0xFF;
	PORTCFG.MPCMASK = 0xFF;
	LEDPORT.PIN0CTRL = PORT_INVEN_bm;
	LEDPORT.OUT = 0x00;
	
	clear_buffer(false);
	calc_lookup();
	init_hw();

	//Enable medium levels interrupt globally
	//PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_HILVLEN_bm;

	LEDPORT.OUT = OSC.CTRL;

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
	buffer[y][x] = true;
	buffer[y][BUFSZX] = true;
}

void draw_rect(int x, int y, int width, int height){
	for(int i = 0; i < height; ++i){
		buffer[i][BUFSZX] = true;
	for(int j = 0; j < width; ++j){
		buffer[y+i][x+j] = true;	
	}
	}
}

int main(void){
	init();

	clear_buffer(true);

	// Ultimate loop
	while(1){
	}
}



// painting interrupt
ISR(TCE1_OVF_vect){
	// x and y index
	static uint8_t yi = 0;
	static uint8_t xi = 0;
	static uint8_t toggle = 0;
	
	// Assuming XRES/YRES is 2^n for any n >= 1
	// a & (2^n - 1) == a % 2^n, but faster.
	if(xi == 0){
		if(buffer[yi][BUFSZX]){
			if(toggle == 0){
				DACA.CH0DATA = ylookup[yi];
				toggle = 1;
			}
			else{
				toggle = 0;
				yi = (yi + 1) & (YRES - 1);
			}
		}
		else{
			yi = (yi + 1) & (YRES - 1);
			return;
		}
	}
	
	if(buffer[yi][xi]) DACA.CH1DATA = xlookup[xi];
	
	xi = (xi + 1) & (XRES - 1);
}

// drawing? 
ISR(TCE0_OVF_vect){
	static int8_t q = 0;

	LEDPORT.OUTTGL = 0xF7;
	q == 0? clear_buffer(true) : clear_buffer(false);

	q ^= 1;
}
