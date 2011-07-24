#ifndef HW_H
#define HW_H
#include "draw.h"

#define LEDPORT	PORTE
// Timing
#define CPU_CLK 32000000UL
#define FRAMERATE 20
#define DRAW_RATE ((FRAMERATE)*(XRES)*(YRES))
#define DRAW_INTERRUPT ((CPU_CLK)/(DRAW_RATE))
#define DRAW_OFFSET 0


// DAC parameters
#define XOFFSET 0x300
#define YOFFSET 0x300
#define DACMAX 0xFFF // 12 bit

void hw_set_pixel(point_t point);
void hw_init();
void hw_wait();
#endif
