#ifndef HW_HOST
#define HW_HOST

#define COLOR_GREEN (0462 >> 1)
#define SCALE_FACTOR 6
#define DRAW_RATE 5

enum HW_INPUT_KEYS {
	ONE_UP,
	ONE_DOWN,
	TWO_UP,
	TWO_DOWN,
	EXIT,
	NO_INPUT
};

void hw_init();
void hw_deinit();
void hw_draw();
int hw_quit();
enum HW_INPUT_KEYS hw_get_input();

#endif
