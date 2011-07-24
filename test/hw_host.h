#ifndef HW_HOST
#define HW_HOST

#define COLOR_GREEN (0462 >> 1)
#define SCALE_FACTOR 12
#define DRAW_RATE 20

void hw_init();
void hw_deinit();
void hw_draw();
int hw_quit();

#endif
