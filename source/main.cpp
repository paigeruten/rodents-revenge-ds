#include <nds.h>
#include <fat.h>
#include "canvas.h"
#include "font.h"
#include "menuset.h"
#include "game.h"
#include "options.h"
#include "radiomenu.h"

const ButtonColors BUTTON_COLORS = { RGB(2, 6, 10), RGB(2, 8, 14), RGB(27, 12, 0) };
const ButtonColors BUTTON_PRESSED_COLORS = { RGB(2, 6, 10), RGB(27, 12, 0), RGB(2, 8, 14) };

const Color MENU_BACKGROUND_COLOR = RGB(8, 14, 20);
const u8 MENU_BUTTON_WIDTHS = SCREEN_WIDTH - 64;
const u8 MENU_BUTTON_HEIGHTS = 32;

const u8 RADIOMENU_BUTTON_WIDTHS = SCREEN_WIDTH - 64;
const u8 RADIOMENU_BUTTON_HEIGHTS = 24;

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

void play_game(void *font) {
	Game game(&screen_top, (Font *)font);
	game.begin();
}

void change_game_speed(void *font) {
	RadioMenu speed_menu(&screen_bottom, (Font *)font, MENU_BACKGROUND_COLOR);

	speed_menu.set_button_widths(RADIOMENU_BUTTON_WIDTHS);
	speed_menu.set_button_heights(RADIOMENU_BUTTON_HEIGHTS);
	speed_menu.set_button_colors(BUTTON_COLORS, BUTTON_PRESSED_COLORS);

	speed_menu.add_back_button("Back");

	speed_menu.add_radio_button("Snail", SPEED_SNAIL);
	speed_menu.add_radio_button("Slow", SPEED_SLOW);
	speed_menu.add_radio_button("Medium", SPEED_MEDIUM);
	speed_menu.add_radio_button("Fast", SPEED_FAST);
	speed_menu.add_radio_button("Blazing", SPEED_BLAZING);

	speed_menu.set_selected_value(options.get_speed());

	options.set_speed((GameSpeed)speed_menu.select());
}

int main(void) {
	irqInit();
	irqEnable(IRQ_VBLANK);

	keysSetRepeat(10, 2);

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
	Font font(options.full_path("fonts/sans.font"));

	// Create menus
	MenuSet menu(&screen_bottom, &font, MENU_BACKGROUND_COLOR);
	menu.set_button_widths(MENU_BUTTON_WIDTHS);
	menu.set_button_heights(MENU_BUTTON_HEIGHTS);
	menu.set_button_colors(BUTTON_COLORS, BUTTON_PRESSED_COLORS);

	MenuId MAIN_MENU = menu.add_menu();
	MenuId OPTIONS_MENU = menu.add_menu();
	MenuId HIGH_SCORES_MENU = menu.add_menu();
	MenuId RESET_HIGH_SCORES_MENU = menu.add_menu();

	menu.add_button(MAIN_MENU, "Play Game", &play_game, (void *)&font);
	menu.add_button(MAIN_MENU, "High Scores", HIGH_SCORES_MENU);
	menu.add_button(MAIN_MENU, "Options", OPTIONS_MENU);

	menu.add_button(OPTIONS_MENU, "Back", MAIN_MENU);
	menu.add_button(OPTIONS_MENU, "Change Starting Level", &blah, NULL);
	menu.add_button(OPTIONS_MENU, "Change Game Speed", &change_game_speed, (void *)&font);

	menu.add_button(HIGH_SCORES_MENU, "Back", MAIN_MENU);
	menu.add_button(HIGH_SCORES_MENU, "View High Scores", &blah, NULL);
	menu.add_button(HIGH_SCORES_MENU, "Reset High Scores", RESET_HIGH_SCORES_MENU);

	menu.add_button(RESET_HIGH_SCORES_MENU, "Back", HIGH_SCORES_MENU);
	menu.add_button(RESET_HIGH_SCORES_MENU, "Confirm Reset", &blah, NULL);

	menu.begin(MAIN_MENU);

	return 0;
}

