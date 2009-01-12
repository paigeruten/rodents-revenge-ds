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
			break;
		}

		swiWaitForVBlank();
	}

	// Load title screen
	Image image_title(&screen_top, options.full_path("images/title.img"));
	image_title.draw(0, 0);
}

void print_help_page(FontSet *fonts, u8 page) {
	Font *font_large = fonts->large_font;
	Font *font_small = fonts->small_font;

	screen_top.clear(MENU_BACKGROUND_COLOR);
	screen_bottom.clear(MENU_BACKGROUND_COLOR);

	screen_top.rect(0, 0, SCREEN_WIDTH - 1, 23, RGB(8, 14, 20), RECT_FILLED);
	screen_top.rect(0, 24, SCREEN_WIDTH - 1, 24, RGB(0, 0, 0), RECT_FILLED);

	char page_num[16] = "Page ";

	strcat(page_num, strval(page + 1));
	strcat(page_num, "/");
	strcat(page_num, strval(NUM_PAGES));

	font_large->print_string_right(page_num, 5, 5, &screen_top, RGB(31, 15, 0));

	Tile tile_mouse(options.full_path("tiles/mouse_big.tile"));
	tile_mouse.draw(&screen_top, 5, 6);

	Clock example_clock(&screen_top, CLOCK_X, 30);

	Tile tile_movable_block(options.full_path("tiles/movable_block.tile"));
	Tile tile_stationary_block(options.full_path("tiles/stationary_block.tile"));
	Tile tile_sink_hole(options.full_path("tiles/sinkhole.tile"));
	Tile tile_yarn(options.full_path("tiles/yarn.tile"));
	Tile tile_trap(options.full_path("tiles/trap.tile"));

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

			font_small->print_string("Move the mouse up, down, left, and", 5, 30, &screen_top);
			font_small->print_string("right with the directional pad.", 5, 40, &screen_top);

			font_small->print_string("Pause/unpause the game with the", 5, 60, &screen_top);
			font_small->print_string("Start button.", 5, 70, &screen_top);
			break;

		case 2:
			font_large->print_string("Scoring", 22, 5, &screen_top, RGB(31, 15, 0));

			font_small->print_string("You get 100 points for eating a piece", 5, 30, &screen_top);
			font_small->print_string("of cheese.", 5, 40, &screen_top);

			font_small->print_string("You get points for every time the", 5, 60, &screen_top);
			font_small->print_string("second hand on the clock makes a", 5, 70, &screen_top);
			font_small->print_string("complete revolution. The amount of", 5, 80, &screen_top);
			font_small->print_string("points you get depends on what level you", 5, 90, &screen_top);
			font_small->print_string("are on. The higher levels give you more", 5, 100, &screen_top);
			font_small->print_string("points, while the lower levels give you", 5, 110, &screen_top);
			font_small->print_string("less points.", 5, 120, &screen_top);
			break;

		case 3:
			font_large->print_string("The Clock", 22, 5, &screen_top, RGB(31, 15, 0));

			example_clock.draw();

			font_small->print_string("The clock has a second hand, a minute", 5, 80, &screen_top);
			font_small->print_string("hand, and a blue line. The speed of the", 5, 90, &screen_top);
			font_small->print_string("clock depends on the game speed setting.", 5, 100, &screen_top);
			font_small->print_string("When the minute hand reaches the blue", 5, 110, &screen_top);
			font_small->print_string("line, new cats are spawned. If all cats", 5, 120, &screen_top);
			font_small->print_string("are trapped before the minute hand", 5, 130, &screen_top);
			font_small->print_string("reaches the blue line, the clock", 5, 140, &screen_top);
			font_small->print_string("fast-forwards up to the blue line. In", 5, 150, &screen_top);
			font_small->print_string("either case, the blue line then moves a", 5, 160, &screen_top);
			font_small->print_string("few minutes ahead. Once the minute hand", 5, 170, &screen_top);
			font_small->print_string("makes a full revolution around the clock,", 5, 180, &screen_top);

			font_small->print_string("the minute hand and blue line stop, and", 5, 5, &screen_bottom);
			font_small->print_string("your points start decreasing rapidly. If", 5, 15, &screen_bottom);
			font_small->print_string("all cats are trapped at this point, you", 5, 25, &screen_bottom);
			font_small->print_string("will move onto the next level before", 5, 35, &screen_bottom);
			font_small->print_string("your points decrease. Otherwise you must", 5, 45, &screen_bottom);
			font_small->print_string("trap all the remaining cats to move onto", 5, 55, &screen_bottom);
			font_small->print_string("the next level.", 5, 65, &screen_bottom);
			break;

		case 4:
			font_large->print_string("Tiles", 22, 5, &screen_top, RGB(31, 15, 0));

			tile_movable_block.draw(&screen_top, 5, 33);
			font_large->print_string("Movable Block", 17, 30, &screen_top);
			font_small->print_string("These blocks can be pushed by the mouse.", 5, 45, &screen_top);
			font_small->print_string("A whole row of any number of these blocks", 5, 55, &screen_top);
			font_small->print_string("can be pushed all at once as long as", 5, 65, &screen_top);
			font_small->print_string("there is a free tile for the last block", 5, 75, &screen_top);
			font_small->print_string("to move onto.", 5, 85, &screen_top);

			tile_stationary_block.draw(&screen_top, 5, 108);
			font_large->print_string("Stationary Block", 17, 105, &screen_top);
			font_small->print_string("These blocks cannot be moved or", 5, 120, &screen_top);
			font_small->print_string("destroyed.", 5, 130, &screen_top);

			tile_sink_hole.draw(&screen_top, 5, 153);
			font_large->print_string("Sink Hole", 17, 150, &screen_top);
			font_small->print_string("If you step onto one of these, you will", 5, 165, &screen_top);
			font_small->print_string("remain stuck in it for a few seconds. If", 5, 175, &screen_top);
			font_small->print_string("a movable block is pushed onto a sink", 5, 5, &screen_bottom);
			font_small->print_string("hole, the movable block disappears.", 5, 15, &screen_bottom);

			tile_yarn.draw(&screen_bottom, 5, 38);
			font_large->print_string("Ball of Yarn", 17, 35, &screen_bottom);
			font_small->print_string("These appear at the border of the level", 5, 50, &screen_bottom);
			font_small->print_string("randomly. They stay in one place for a", 5, 60, &screen_bottom);
			font_small->print_string("while and then suddenly move in one of", 5, 70, &screen_bottom);
			font_small->print_string("eight directions. If one touches you,", 5, 80, &screen_bottom);
			font_small->print_string("you die.", 5, 90, &screen_bottom);

			tile_trap.draw(&screen_bottom, 5, 113);
			font_large->print_string("Mouse Trap", 17, 110, &screen_bottom);
			font_small->print_string("If you step on one of these, you die", 5, 125, &screen_bottom);
			font_small->print_string("instantly.", 5, 135, &screen_bottom);
			break;	
	}
}

