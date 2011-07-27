#include "hw_host.h"
#include "../pong.h"
#include "../draw.h"


int main(){
	hw_init();
	pong_init(); // inits draw
	int run = 1;
	while(run){
		// pong move
		switch(hw_get_input()){
			case ONE_UP:
				break;
			case ONE_DOWN:
				break;
			case TWO_UP:
				break;
			case TWO_DOWN:
				break;
			case EXIT:
				run = 0;
				break;
		}
		
		pong_move_ball();
		
		hw_draw(); // blocks for timing
		draw_swap_buffers();
		draw_copy_buffers();
		
	}

	hw_deinit();

	return 0;
}
