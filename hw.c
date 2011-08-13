#include "hw.h"
#include "draw.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>


// Lookup table for DAC
static uint16_t xlookup[XRES];
static uint16_t ylookup[YRES];

static uint16_t ADCA_CH0_min;
static uint16_t ADCA_CH1_min;


uint16_t hw_get_ADC_min_shifted(uint8_t ADC, uint8_t shift){
	switch(ADC){
		case HW_ADC_0:
			return ADCA_CH0_min >> shift;
		case HW_ADC_1:
			return ADCA_CH1_min >> shift;
		default:
			return HW_ERROR;
	}
}

// Interrupt callbacks
// painting interrupt
ISR(TCE1_OVF_vect){
	volatile static point_t* current_point;
	
	current_point = draw_next_point();
	DACA.CH0DATA = xlookup[current_point->x];
	DACA.CH1DATA = ylookup[current_point->y];
}



static void calc_lookup(){
	// Calculate lookup table for DAC, same used for X and Y
	uint8_t unit = 16;

	for(uint8_t i = 0; i < XRES; ++i)
		xlookup[i] = i*unit + XOFFSET;	
	for(uint8_t i = 0; i < YRES; ++i)
		ylookup[i] = i*unit + XOFFSET;
}


static void init_clock(){
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

static void init_DAC(){
	// Setup DAC, allow as fast as possible converts
	DACA.CTRLA |= DAC_CH1EN_bm | DAC_CH0EN_bm | DAC_ENABLE_bm;
	DACA.CTRLB |= DAC_CHSEL_DUAL_gc;
	DACA.CTRLC |= DAC_REFSEL_AVCC_gc;
	DACA.TIMCTRL &= ~DAC_CONINTVAL_gm;
	DACA.TIMCTRL |= DAC_CONINTVAL_128CLK_gc | DAC_REFRESH_OFF_gc;
}

static void init_ADC(){
	ADCA.CTRLA |= ADC_ENABLE_bm;
	ADCA.CTRLB |= ADC_FREERUN_bm | ADC_RESOLUTION_12BIT_gc;
	ADCA.REFCTRL |= ADC_REFSEL_VCC_gc; // default is internal 1.00V
	ADCA.EVCTRL |= ADC_SWEEP_01_gc | ADC_EVSEL_4567_gc;
	ADCA.PRESCALER |= ADC_PRESCALER_DIV512_gc;

	ADCA.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN4_gc;
	ADCA.CH1.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH1.MUXCTRL |= ADC_CH_MUXPOS_PIN5_gc;

	uint16_t ADCA_CH0_min = 0xFF;
	uint16_t ADCA_CH1_min = 0xFF;

	for(uint16_t i = 0; i < 10000; i++){
		for(uint16_t i = 0; i < 65000; i++){
		}
		LEDPORT.OUTTGL = 0xff;
		ADCA_CH0_min = ADCA.CH0.RES < ADCA_CH0_min? ADCA.CH0.RES : ADCA_CH0_min;
		ADCA_CH1_min = ADCA.CH1.RES < ADCA_CH1_min? ADCA.CH1.RES : ADCA_CH1_min;
		LEDPORT.OUT = ADCA_CH0_min;
	}
}

static void init_timers(){
	//Setup timer1: framebuffer output, with overflow interrupt.
	TCE1.CTRLA = TC_CLKSEL_DIV1_gc;
	TCE1.INTCTRLA = (TCE1.INTCTRLA & ~TC1_OVFINTLVL_gm) | TC_OVFINTLVL_MED_gc;
	TCE1.PER = 0x200;


	//Setup timer2: other stuff with overflow interrupt
	TCE0.CTRLA = TC_CLKSEL_DIV1024_gc;
	TCE0.INTCTRLA = (TCE0.INTCTRLA & ~TC1_OVFINTLVL_gm) | TC_OVFINTLVL_HI_gc;
	TCE0.PER = 0x61a;
}

static void init_interrupts(){
	//Enable medium and high levels interrupt globally
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_HILVLEN_bm;

	//Enable global interrupts
	sei();
}

void hw_init(){
	/*Setup LEDPORT as the output port*/
	LEDPORT.DIR = 0xFF;
	PORTCFG.MPCMASK = 0xFF;
	LEDPORT.PIN0CTRL = PORT_INVEN_bm;
	LEDPORT.OUT = 0x00;

	// Using porta7 as supply for pot-meters	
	PORTA.DIR |= 1 << 7;
	PORTA.OUT |= 1 << 7;
	
	calc_lookup();
	init_clock();
	init_DAC();	
	init_ADC();
	init_timers();
	init_interrupts();
}
