#include <nds.h>
#include "levelselector.h"

LevelSelector::LevelSelector(Font *the_font, Font *the_small_font, Color background) {
	font = the_font;
	small_font = the_small_font;
	background_color = background;

	level.init(&screen_top);

	num_buttons = level.get_num_levels();
	selected_button = 0;

	u8 current_button_x = LEVEL_SELECTOR_BUTTON_SPACING;
	u8 current_button_y = LEVEL_SELECTOR_BUTTON_SPACING;

	for (u8 current_button = 0; current_button < num_buttons; current_button++) {
		buttons[current_button] = new Button;
		buttons[current_button]->init(&screen_bottom, small_font, strval(level.get_level(current_button).id));
		buttons[current_button]->set_colors(BUTTON_COLORS, BUTTON_PRESSED_COLORS);
		buttons[current_button]->set_position(current_button_x, current_button_y);
		buttons[current_button]->set_width(LEVEL_SELECTOR_BUTTON_WIDTHS);
		buttons[current_button]->set_height(LEVEL_SELECTOR_BUTTON_HEIGHTS);

		if ((current_button + 1) % LEVEL_SELECTOR_BUTTON_COLUMNS == 0) {
			current_button_x = LEVEL_SELECTOR_BUTTON_SPACING;
			current_button_y += LEVEL_SELECTOR_BUTTON_HEIGHTS + LEVEL_SELECTOR_BUTTON_SPACING;
		} else {
			current_button_x += LEVEL_SELECTOR_BUTTON_WIDTHS + LEVEL_SELECTOR_BUTTON_SPACING;
		}
	}

	button_ok.init(&screen_bottom, font, "OK");
	button_ok.set_colors(BUTTON_COLORS, BUTTON_PRESSED_COLORS);
	button_ok.set_x(32);//button_ok.center_x();
	button_ok.set_y(BUTTON_OK_Y);
	button_ok.set_width(MENU_BUTTON_WIDTHS);
	button_ok.set_height(MENU_BUTTON_HEIGHTS);
}

LevelSelector::~LevelSelector() {
	for (u8 current_button = 0; current_button < num_buttons; current_button++) {
		delete buttons[current_button];
		buttons[current_button] = NULL;
	}
}

void LevelSelector::set_selected_level(LevelId level_id) {
	for (u8 current_button = 0; current_button < num_buttons; current_button++) {
		if (level.get_level(current_button).id == level_id) {
			selected_button = current_button;
			buttons[current_button]->set_colors(BUTTON_PRESSED_COLORS, BUTTON_PRESSED_COLORS);
		} else {
			buttons[current_button]->set_colors(BUTTON_COLORS, BUTTON_PRESSED_COLORS);
		}
	}
}

void LevelSelector::draw() {
	screen_bottom.clear(background_color);
	screen_top.clear(background_color);

	level.draw(LEVEL_X, LEVEL_Y);

	for (u8 current_button = 0; current_button < num_buttons; current_button++) {
		buttons[current_button]->draw();
	}

	button_ok.draw();
}

LevelId LevelSelector::select_level() {
	touchPosition stylus;

	level.set_current_level(level.get_level(selected_button).id);
	level.load();
	draw();

	while (true) {
		scanKeys();
		stylus = touchReadXY();

		if (button_ok.update(stylus) == BUTTON_CLICKED) {
			return level.get_level(selected_button).id;
		}

		for (u8 current_button = 0; current_button < num_buttons; current_button++) {
			if (buttons[current_button]->update(stylus) == BUTTON_CLICKED) {
				u8 last_selected_button = selected_button;

				set_selected_level(level.get_level(current_button).id);

				buttons[last_selected_button]->draw();
				buttons[selected_button]->draw();

				level.set_current_level(level.get_level(selected_button).id);
				level.load();
				level.draw(LEVEL_X, LEVEL_Y);
			}
		}

		swiWaitForVBlank();
	}
}

