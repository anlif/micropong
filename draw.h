// Screen parameters
// NOTE: both XRES and YRES
#define XRES 	64	
#define YRES 	64
#define BUFFER_SIZE ((XRES)*(YRES))

typedef struct {
	int x;
	int y;
} point;

// Screen buffer and lookup table for DAC
static point buffer1[BUFFER_SIZE];
static point buffer2[BUFFER_SIZE];
static point (*read_buffer) = buffer1;
static point (*write_buffer) = buffer2;
