#include <nds.h>
#include "fat.h"
#include "canvas.h"
#include "font.h"
#include "keyboard.h"

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

int main(void) {
	irqInit();
	irqEnable(IRQ_VBLANK);

	init_screens();

	screen_top.clear(RGB(0, 0, 0));
	screen_bottom.clear(RGB(0, 0, 0));

	// Init FAT (Displays green screens if it fails)
	if (!fatInitDefault()) {
		screen_top.clear(RGB(0, 31, 0));
		screen_bottom.clear(RGB(0, 31, 0));
		while (1);
	}

	// Init font (Displays red screens if it can't find the font file, blue screens if the font file is invalid)
	Font font("/data/rodents-revenge/fonts/sans.font");
	if (font.get_status() == FONT_OK) {
		font.print_string("Font loaded.", 5, 5, &screen_top, RGB(31, 31, 31));
	} else if (font.get_status() == FONT_ERR_FILE_NOT_FOUND) {
		screen_top.clear(RGB(31, 0, 0));
		screen_bottom.clear(RGB(31, 0, 0));
		while (1);
	} else if (font.get_status() == FONT_ERR_INVALID_FONT_FILE) {
		screen_top.clear(RGB(0, 0, 31));
		screen_bottom.clear(RGB(0, 0, 31));
		while (1);
	}

	// Init keyboard
	Keyboard keyboard("/data/rodents-revenge/keyboards/default.kb", &screen_bottom, &font, 20, 80);
	if (keyboard.get_status() == KEYBOARD_FILE_NOT_FOUND) {
		font.print_string("Keyboard file not found.", 5, 20, &screen_top, RGB(31, 31, 31));
		while(1);
	}

	// Test Canvas and Font classes together.
	Canvas canvas(100, 20);
	canvas.clear(RGB(31, 31, 31));
	font.print_string_center("Testing!", 1, &canvas, RGB(0, 0, 0));
	canvas.copy(&screen_top, 30, 30);

	// Test keyboard
	keyboard.draw(false);

	bool shift = false;
	touchPosition stylus;
	while (1) {
		scanKeys();
		stylus = touchReadXY();

		if (keysHeld() & (KEY_L | KEY_R)) {
			shift = true;
			keyboard.draw(shift);
		} else {
			shift = false;
			keyboard.draw(shift);
		}

		keyboard.key_pressed(shift, stylus.px, stylus.py);

		swiWaitForVBlank();
	}

	return 0;
}

