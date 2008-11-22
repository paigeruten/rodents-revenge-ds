#include <nds.h>
#include "fat.h"
#include "canvas.h"
#include "font.h"
#include "keyboard.h"
#include "tile.h"
#include "tilemap.h"
#include "button.h"
#include "menu.h"

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

void blah() {
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
	Menu main_menu(&screen_bottom, &font, RGB(26, 26, 26));
	Menu play_game_menu(&screen_bottom, &font, RGB(26, 26, 26));
	Menu options_menu(&screen_bottom, &font, RGB(26, 26, 26));
	Menu high_scores_menu(&screen_bottom, &font, RGB(26, 26, 26));

	ButtonColors button_colors;
	button_colors.border = RGB(3, 3, 3);
	button_colors.background = RGB(20, 20, 20);
	button_colors.text = RGB(15, 0, 0);

	ButtonColors button_pressed_colors = button_colors;
	button_pressed_colors.background = RGB(27, 27, 27);

	main_menu.set_button_widths(SCREEN_WIDTH - 64);
	main_menu.set_button_heights(32);
	main_menu.set_button_colors(button_colors, button_pressed_colors);

	play_game_menu.set_button_widths(SCREEN_WIDTH - 64);
	play_game_menu.set_button_heights(32);
	play_game_menu.set_button_colors(button_colors, button_pressed_colors);

	options_menu.set_button_widths(SCREEN_WIDTH - 64);
	options_menu.set_button_heights(32);
	options_menu.set_button_colors(button_colors, button_pressed_colors);

	high_scores_menu.set_button_widths(SCREEN_WIDTH - 64);
	high_scores_menu.set_button_heights(32);
	high_scores_menu.set_button_colors(button_colors, button_pressed_colors);

	main_menu.add_button("Play Game", &play_game_menu);
	main_menu.add_button("High Scores", &high_scores_menu);
	main_menu.add_button("Options", &options_menu);

	play_game_menu.add_button("Back", &main_menu);
	play_game_menu.add_button("Level 1", &blah);
	play_game_menu.add_button("Level 2", &blah);
	play_game_menu.add_button("Level 3", &blah);
	play_game_menu.add_button("Next >>>", &play_game_menu);

	options_menu.add_button("Back", &main_menu);
	options_menu.add_button("Reset All Data", &blah);

	high_scores_menu.add_button("Back", &main_menu);
	high_scores_menu.add_button("Online High Scores", &blah);
	high_scores_menu.add_button("Local High Scores", &blah);

	Menu *menu = &main_menu;

	while (1) {
		menu = menu->select();
	}

	return 0;
}

