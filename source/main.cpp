#include <nds.h>
#include "fat.h"
#include "canvas.h"
#include "font.h"
#include "keyboard.h"
#include "tile.h"
#include "tilemap.h"
#include "button.h"
#include "menu.h"
#include "menuset.h"
#include "game.h"

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

void blah(void *data) {
	static Color color = RGB(0, 0, 0);
	screen_top.clear(color);
	color = (~color) | BIT(15);
}

void play_game(void *data) {
	Game game(&screen_top, (Font *)data);
	game.begin();
}

int main(void) {
	irqInit();
	irqEnable(IRQ_VBLANK);

	init_screens();

	screen_top.clear(RGB(31, 31, 31));
	screen_bottom.clear(RGB(31, 31, 31));

	// Init FAT (Displays green screens if it fails)
	if (!fatInitDefault()) {
		screen_top.clear(RGB(0, 31, 0));
		screen_bottom.clear(RGB(0, 31, 0));
		while (1);
	}

	// Init font
	Font font("/data/rodents-revenge/fonts/sans.font");

	// Test menus
	ButtonColors button_colors;
	button_colors.border = RGB(2, 6, 10);
	button_colors.background = RGB(2, 8, 14);
	button_colors.text = RGB(27, 12, 0);

	ButtonColors button_pressed_colors = button_colors;
	button_pressed_colors.background = button_colors.text;
	button_pressed_colors.text = button_colors.background;

	MenuSet menu(&screen_bottom, &font, RGB(8, 14, 20));
	menu.set_button_widths(SCREEN_WIDTH - 64);
	menu.set_button_heights(32);
	menu.set_button_colors(button_colors, button_pressed_colors);

	MenuId MAIN_MENU = menu.add_menu();
	MenuId PLAY_GAME_MENU = menu.add_menu();
	MenuId OPTIONS_MENU = menu.add_menu();
	MenuId HIGH_SCORES_MENU = menu.add_menu();

	menu.add_button(MAIN_MENU, "Play Game", PLAY_GAME_MENU);
	menu.add_button(MAIN_MENU, "High Scores", HIGH_SCORES_MENU);
	menu.add_button(MAIN_MENU, "Options", OPTIONS_MENU);

	menu.add_button(PLAY_GAME_MENU, "Back", MAIN_MENU);
	menu.add_button(PLAY_GAME_MENU, "Level 1", &play_game, (void *)&font);
	menu.add_button(PLAY_GAME_MENU, "Level 2", &blah, 0);
	menu.add_button(PLAY_GAME_MENU, "Level 3", &blah, 0);
	menu.add_button(PLAY_GAME_MENU, "Next >>>", PLAY_GAME_MENU);

	menu.add_button(OPTIONS_MENU, "Back", MAIN_MENU);
	menu.add_button(OPTIONS_MENU, "Reset All Data", &blah, 0);

	menu.add_button(HIGH_SCORES_MENU, "Back", MAIN_MENU);
	menu.add_button(HIGH_SCORES_MENU, "Online High Scores", &blah, 0);
	menu.add_button(HIGH_SCORES_MENU, "Local High Scores", &blah, 0);

	menu.begin(MAIN_MENU);

	return 0;
}

