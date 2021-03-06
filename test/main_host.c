#include "hw_host.h"
#include "../pong.h"
#include "../draw.h"
#include <stdbool.h>
#include <stdio.h>


int main(){
	hw_init();
	pong_init(); // inits draw
	int run = 1;
	bool play = true;
	while(run){
		if(play){
			// pong move
			switch(hw_get_input()){
				case EXIT:
					run = 0;
					break;
				case ONE_UP:
					pong_move_paddle(PONG_PADDLE_LEFT, 
							pong_get_paddle_state(PONG_PADDLE_LEFT) - 2);
					break;
				case ONE_DOWN:
					pong_move_paddle(PONG_PADDLE_LEFT, 
							pong_get_paddle_state(PONG_PADDLE_LEFT) + 2);
					break;
				case TWO_UP:
					pong_move_paddle(PONG_PADDLE_RIGHT, 
							pong_get_paddle_state(PONG_PADDLE_RIGHT) - 2);
					break;
				case TWO_DOWN:
					pong_move_paddle(PONG_PADDLE_RIGHT, 
							pong_get_paddle_state(PONG_PADDLE_RIGHT) + 2);
					break;
			}

			int status = pong_move_ball();
			if( status != PONG_NO_ERROR ){
				printf("restart\n");
				pong_restart();
			}

			hw_draw(); // blocks for timing
			draw_swap_buffers();
			draw_copy_buffers();

		}
	}
	hw_deinit();

	return 0;
}
