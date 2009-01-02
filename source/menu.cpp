#include <nds.h>
#include "canvas.h"
#include "button.h"
#include "menu.h"

Menu::Menu() {
	init(NULL, NULL, MENU_DEFAULT_BACKGROUND_COLOR);
}

Menu::Menu(Canvas *the_canvas, Font *the_font, Color background) {
	init(the_canvas, the_font, background);
}

Menu::~Menu() {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (buttons[i]) {
			delete buttons[i];
			buttons[i] = NULL;
		}
	}
}

void Menu::init(Canvas *the_canvas, Font *the_font, Color background) {
	canvas = the_canvas;
	font = the_font;
	background_color = background;

	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		buttons[i] = NULL;
	}
}

void Menu::draw() {
	arrange_buttons();

	Canvas buffer(SCREEN_WIDTH, SCREEN_HEIGHT);

	buffer.clear(background_color);

	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (buttons[i]) {
			buttons[i]->set_canvas(&buffer);
			buttons[i]->draw();
			buttons[i]->set_canvas(canvas);
		}
	}

	swiWaitForVBlank();
	buffer.copy(canvas);
}

u8 Menu::add_menu_button(const char *button_text) {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (!buttons[i]) {
			// Create button
			buttons[i] = new Button;
			buttons[i]->init(canvas, font, button_text);
			buttons[i]->set_colors(button_colors, button_pressed_colors);
			buttons[i]->set_width(button_widths);
			buttons[i]->set_height(button_heights);

			return i;
		}
	}

	return 255;
}

void Menu::arrange_buttons() {
	u8 num_buttons = button_count();
	u16 button_space = canvas->get_height() / num_buttons;

	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (buttons[i]) {
			u16 button_x = (canvas->get_width() - buttons[i]->get_width()) / 2; // center it horizontally
			u16 button_y = (i * button_space) + ((button_space - buttons[i]->get_height()) / 2); // space them evenly

			buttons[i]->set_position(button_x, button_y);
		}
	}
}

u8 Menu::button_count() const {
	u8 count = 0;

	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (buttons[i]) {
			count++;
		}
	}

	return count;
}

