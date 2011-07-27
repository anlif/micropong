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
				pong_move(PADDLE_LEFT, PADDLE_UP);
				break;
			case ONE_DOWN:
				pong_move(PADDLE_LEFT, PADDLE_DOWN);
				break;
			case TWO_UP:
				pong_move(PADDLE_RIGHT, PADDLE_UP);
				break;
			case TWO_DOWN:
				pong_move(PADDLE_RIGHT, PADDLE_DOWN);
				break;
			case EXIT:
				run = 0;
				break;
		}	
		
		hw_draw(); // blocks for timing
		draw_swap_buffers();
		
	}

	hw_deinit();

	return 0;
}
