#include <nds.h>
#include <string.h>
#include "help.h"

void help(void *fonts) {
	FontSet *fontset = (FontSet *)fonts;
	u8 page = 0;
	touchPosition stylus;

	Button button_prev(&screen_bottom, fontset->large_font, "<<");
	Button button_next(&screen_bottom, fontset->large_font, ">>");
	Button button_back(&screen_bottom, fontset->large_font, "Back");

	button_prev.set_colors(BUTTON_COLORS, BUTTON_PRESSED_COLORS);
	button_next.set_colors(BUTTON_COLORS, BUTTON_PRESSED_COLORS);
	button_back.set_colors(BUTTON_COLORS, BUTTON_PRESSED_COLORS);

	button_prev.set_width(32);
	button_next.set_width(32);
	button_back.set_width(SCREEN_WIDTH - 84);

	button_prev.set_height(32);
	button_next.set_height(32);
	button_back.set_height(32);

	button_prev.set_position(5, SCREEN_HEIGHT - 37);
	button_next.set_position(SCREEN_WIDTH - 37, SCREEN_HEIGHT - 37);
	button_back.set_position(42, SCREEN_HEIGHT - 37);

	print_help_page((FontSet *)fonts, page);

	button_prev.disable();

	button_prev.draw();
	button_next.draw();
	button_back.draw();

	while (1) {
		scanKeys();
		stylus = touchReadXY();

		if (button_prev.update(stylus) == BUTTON_CLICKED) {
			if (page > 0) {
				page--;
				print_help_page((FontSet *)fonts, page);

				if (page == 0) {
					button_prev.disable();
				}

				button_next.enable();

				button_prev.draw();
				button_next.draw();
				button_back.draw();
			}
		}

		if (button_next.update(stylus) == BUTTON_CLICKED) {
			if (page < NUM_PAGES - 1) {
				page++;
				print_help_page((FontSet *)fonts, page);

				if (page == NUM_PAGES - 1) {
					button_next.disable();
				}

				button_prev.enable();

				button_prev.draw();
				button_next.draw();
				button_back.draw();
			}
		}

		if (button_back.update(stylus) == BUTTON_CLICKED) {
			return;
		}

		swiWaitForVBlank();
	}
}

void print_help_page(FontSet *fonts, u8 page) {
	Font *font_large = fonts->large_font;
	Font *font_small = fonts->small_font;

	screen_top.clear(MENU_BACKGROUND_COLOR);
	screen_bottom.clear(MENU_BACKGROUND_COLOR);

	char page_num[16] = "Page ";

	strcat(page_num, strval(page + 1));
	strcat(page_num, "/");
	strcat(page_num, strval(NUM_PAGES));

	font_large->print_string_right(page_num, 5, 5, &screen_top, RGB(31, 15, 0));

	Tile tile_mouse(options.full_path("tiles/mouse_big.tile"));
	tile_mouse.draw(&screen_top, 5, 6);

	switch (page) {
		case 0:
			font_large->print_string("Intro", 22, 5, &screen_top, RGB(31, 15, 0));

			font_small->print_string("In Rodent's Revenge, you are a mouse who", 5, 30, &screen_top);
			font_small->print_string("gets his revenge on the cats by trapping", 5, 40, &screen_top);
			font_small->print_string("them. This is done by pushing around", 5, 50, &screen_top);
			font_small->print_string("blocks while avoiding dangers such as", 5, 60, &screen_top);
			font_small->print_string("mouse traps, sink holes, balls of yarn,", 5, 70, &screen_top);
			font_small->print_string("and the cats themselves who constantly", 5, 80, &screen_top);
			font_small->print_string("chase you.", 5, 90, &screen_top);

			font_small->print_string("There are 50 levels. You may choose", 5, 110, &screen_top);
			font_small->print_string("which level to start on. The higher", 5, 120, &screen_top);
			font_small->print_string("levels are more difficult, but give you", 5, 130, &screen_top);
			font_small->print_string("more points.", 5, 140, &screen_top);

			font_small->print_string("There are 5 speed settings. The game", 5, 160, &screen_top);
			font_small->print_string("speed affects the speed of the clock,", 5, 170, &screen_top);
			font_small->print_string("the cats, and the yarn.", 5, 180, &screen_top);
			break;

		case 1:
			font_large->print_string("Controls", 22, 5, &screen_top, RGB(31, 15, 0));
			break;

		case 2:
			font_large->print_string("Gameplay", 22, 5, &screen_top, RGB(31, 15, 0));
			break;

		case 3:
			font_large->print_string("Scoring", 22, 5, &screen_top, RGB(31, 15, 0));
			break;

		case 4:
			font_large->print_string("The Clock", 22, 5, &screen_top, RGB(31, 15, 0));
			break;	
	}
}

