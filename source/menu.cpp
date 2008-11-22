#include <nds.h>
#include "canvas.h"
#include "button.h"
#include "menu.h"

Menu::Menu(Canvas *the_canvas, Font *the_font, Color background) {
	canvas = the_canvas;
	font = the_font;
	background_color = background;

	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		buttons[i] = 0;
		actions[i] = 0;
		submenus[i] = 0;
	}
}

Menu::~Menu() {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (buttons[i]) {
			delete buttons[i];
			buttons[i] = 0;
		}
	}
}

Menu *Menu::select() {
	draw();

	while (1) {
		scanKeys();
		touchPosition stylus = touchReadXY();

		for (u8 i = 0; i < MAX_BUTTONS; i++) {
			if (buttons[i]) {
				if (buttons[i]->update(stylus) == BUTTON_CLICKED) {
					if (submenus[i]) {
						return submenus[i];
					}

					if (actions[i]) {
						(*actions[i])();
					}
				}
			}
		}

		swiWaitForVBlank();
	}
}

void Menu::draw() {
	arrange_buttons();

	canvas->clear(background_color);

	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (buttons[i]) {
			buttons[i]->draw();
		}
	}
}

void Menu::add_button(const char *button_text, Menu *submenu) {
	add_button(button_text, submenu, 0);
}

void Menu::add_button(const char *button_text, void (*action)()) {
	add_button(button_text, 0, action);
}

void Menu::add_button(const char *button_text, Menu *submenu, void (*action)()) {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (!buttons[i]) {
			// Create button
			buttons[i] = new Button;
			buttons[i]->init(canvas, font, button_text);
			buttons[i]->set_colors(button_colors, button_pressed_colors);
			buttons[i]->set_width(button_widths);
			buttons[i]->set_height(button_heights);

			actions[i] = action;
			submenus[i] = submenu;
			break;
		}
	}
}

void Menu::arrange_buttons() {
	u8 num_buttons = button_count();
	u16 button_space = canvas->get_height() / num_buttons;

	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (buttons[i]) {
			buttons[i]->set_position((canvas->get_width() - buttons[i]->get_width()) / 2, (i * button_space) + ((button_space - buttons[i]->get_height()) / 2));
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

