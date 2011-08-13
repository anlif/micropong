#ifndef HW_H
#define HW_H
#include "draw.h"

#define LEDPORT	PORTE
// Timing
#define CPU_CLK 32000000UL
#define FRAMERATE 20
#define DRAW_RATE ((FRAMERATE)*(XRES)*(YRES))
#define DRAW_INTERRUPT 512
#define DRAW_FRAMERATE 800
#define DRAW_OFFSET 0


// DAC parameters
#define XOFFSET 0x300
#define YOFFSET 0x300
#define DACMAX 0xFFF // 12 bit

enum{
	HW_ADC_0,
	HW_ADC_1,
	HW_ERROR
};

void hw_init();
uint16_t hw_get_ADC_min_shifted(uint8_t ADC, uint8_t shift);
#endif
