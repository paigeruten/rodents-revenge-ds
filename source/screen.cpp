#include <nds.h>
#include "screen.h"

Screen screen_top(SCREEN_WIDTH, SCREEN_HEIGHT, (Color *)BG_BMP_RAM(0));
Screen screen_bottom(SCREEN_WIDTH, SCREEN_HEIGHT, (Color *)BG_BMP_RAM_SUB(0));

void init_screens(void) {
	// Main screen turn on
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	BACKGROUND.control[3] = BG_BMP16_256x256 | BG_BMP_BASE(0);
	BACKGROUND.bg3_rotation.xdy = 0;
	BACKGROUND.bg3_rotation.xdx = 1 << 8;
	BACKGROUND.bg3_rotation.ydx = 0;
	BACKGROUND.bg3_rotation.ydy = 1 << 8;
	
	// Sub screen
	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	BACKGROUND_SUB.control[3] = BG_BMP16_256x256 | BG_BMP_BASE(0);
	BACKGROUND_SUB.bg3_rotation.xdy = 0;
	BACKGROUND_SUB.bg3_rotation.xdx = 1 << 8;
	BACKGROUND_SUB.bg3_rotation.ydx = 0;
	BACKGROUND_SUB.bg3_rotation.ydy = 1 << 8;
}

Screen::Screen(u32 the_width, u32 the_height) {
	width = the_width;
	height = the_height;
	data = new Color [width * height];
	memory_allocated = true;
}

Screen::Screen(u32 the_width, u32 the_height, Color *the_data) {
	width = the_width;
	height = the_height;
	data = the_data;
	memory_allocated = false;
}

Screen::~Screen() {
	if (memory_allocated) {
		delete [] data;
		data = 0;
	}
}

void Screen::plot(u32 x, u32 y, Color color) {
	if (x < width && y < height) {
		data[x + y * width] = color;
	}
}

void Screen::copy(Screen *destination, u32 x, u32 y) {
	u32 get_x, get_y;

	get_x = 0;
	for (u32 put_x = x; put_x < x + width; put_x++) {
		get_y = 0;
		for (u32 put_y = y; put_y < y + height; put_y++) {
			destination->plot(put_x, put_y, data[get_x + get_y * width]);
			get_y++;
		}
		get_x++;
	}
}

void Screen::clear(Color color) {
	for (u32 i = 0; i < width * height; i++) {
		data[i] = color;
	}
}

