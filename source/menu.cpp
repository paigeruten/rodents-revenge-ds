#include <nds.h>
#include "canvas.h"
#include "button.h"
#include "menu.h"

Menu::Menu(Canvas *the_canvas, Color background) {
	canvas = the_canvas;
	background_color = background;

	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		buttons[i] = 0;
		actions[i] = 0;
		submenus[i] = 0;
	}
}

Menu::~Menu() {
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

void Menu::add_button(Button *button, Menu *submenu) {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (!buttons[i]) {
			buttons[i] = button;
			submenus[i] = submenu;

			buttons[i]->set_width(canvas->get_width() - BUTTON_MARGIN_LEFT * 2);
			break;
		}
	}
}

void Menu::add_button(Button *button, void (*action)()) {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (!buttons[i]) {
			buttons[i] = button;
			actions[i] = action;

			buttons[i]->set_width(canvas->get_width() - BUTTON_MARGIN_LEFT * 2);
			break;
		}
	}
}

void Menu::arrange_buttons() {
	u8 num_buttons = button_count();
	u16 button_space = canvas->get_height() / num_buttons;

	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (buttons[i]) {
			buttons[i]->set_position(BUTTON_MARGIN_LEFT, (i * button_space) + ((button_space - buttons[i]->get_height()) / 2));
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

