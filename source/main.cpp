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
	Menu main_menu(&screen_bottom, RGB(26, 26, 26));
	Menu play_game_menu(&screen_bottom, RGB(26, 26, 26));
	Menu options_menu(&screen_bottom, RGB(26, 26, 26));
	Menu high_scores_menu(&screen_bottom, RGB(26, 26, 26));

	ButtonColors button_colors, button_pressed_colors;
	button_colors.border = RGB(3, 3, 3);
	button_colors.background = RGB(20, 20, 20);
	button_colors.text = RGB(15, 0, 0);

	button_pressed_colors = button_colors;
	button_pressed_colors.background = RGB(27, 27, 27);

	Button button_play_game(&screen_bottom, &font, "Play Game"); button_play_game.set_colors(button_colors, button_pressed_colors);
	Button button_high_scores(&screen_bottom, &font, "High Scores");button_high_scores.set_colors(button_colors, button_pressed_colors);
	Button button_options(&screen_bottom, &font, "Options");button_options.set_colors(button_colors, button_pressed_colors);

	Button button_back(&screen_bottom, &font, "Back");button_back.set_colors(button_colors, button_pressed_colors);

	Button button_level1(&screen_bottom, &font, "Level 1");button_level1.set_colors(button_colors, button_pressed_colors);
	Button button_level2(&screen_bottom, &font, "Level 2");button_level2.set_colors(button_colors, button_pressed_colors);
	Button button_level3(&screen_bottom, &font, "Level 3");button_level3.set_colors(button_colors, button_pressed_colors);
	Button button_next(&screen_bottom, &font, ">>>");button_next.set_colors(button_colors, button_pressed_colors);

	Button button_wifi(&screen_bottom, &font, "Wi-Fi High Scores");button_wifi.set_colors(button_colors, button_pressed_colors);
	Button button_local(&screen_bottom, &font, "Local High Scores");button_local.set_colors(button_colors, button_pressed_colors);

	Button button_reset(&screen_bottom, &font, "Reset High Scores");button_reset.set_colors(button_colors, button_pressed_colors);

	main_menu.add_button(&button_play_game, &play_game_menu);
	main_menu.add_button(&button_high_scores, &high_scores_menu);
	main_menu.add_button(&button_options, &options_menu);

	play_game_menu.add_button(&button_back, &main_menu);
	play_game_menu.add_button(&button_level1, &blah);
	play_game_menu.add_button(&button_level2, &blah);
	play_game_menu.add_button(&button_level3, &blah);
	play_game_menu.add_button(&button_next, &play_game_menu);

	options_menu.add_button(&button_back, &main_menu);
	options_menu.add_button(&button_reset, &blah);

	high_scores_menu.add_button(&button_back, &main_menu);
	high_scores_menu.add_button(&button_wifi, &blah);
	high_scores_menu.add_button(&button_local, &blah);

	Menu *menu = &main_menu;

	while (1) {
		menu = menu->select();
	}

	return 0;
}

