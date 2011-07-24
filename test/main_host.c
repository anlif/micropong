#include "hw_host.h"
#include "../pong.h"
#include "../draw.h"


int main(){
	hw_init();
	pong_init(); // inits draw

	while(!hw_quit()){
		// pong move
		
		hw_draw(); // blocks for timing

		draw_swap_buffers();
	}

	hw_deinit();

	return 0;
}
