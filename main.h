#define LEDPORT	PORTE

// Screen parameters
// NOTE: both XRES and YRES must be divisible by SQUARES
#define XRES 	64	
#define YRES 	64
#define SQUARES 8 // number of squares in width/height
#define TILES 	((SQUARES)*(SQUARES)) // number of tiles on screen
#define XSPACE 	((XRES)/(SQUARES)) // space between each tile, in 'pixels'
#define YSPACE 	((YRES)/(SQUARES))
#define BUFSZX	(XRES )
#define BUFSZY	(YRES )

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

// Screen buffer and lookup table for DAC
static bool buffer[BUFSZY][BUFSZX+1];
static uint16_t xlookup[BUFSZX];
static uint16_t ylookup[BUFSZY];
