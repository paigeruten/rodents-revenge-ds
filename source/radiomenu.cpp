#include <nds.h>
#include "radiomenu.h"

RadioMenu::RadioMenu() {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		values[i] = 0;
	}

	selected_button = RADIOBUTTON_NONE_SELECTED;
	back_button = RADIOBUTTON_NONE_SELECTED;
}

RadioMenu::RadioMenu(Canvas *the_canvas, Font *the_font, Color background):
Menu(the_canvas, the_font, background) {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		values[i] = 0;
	}

	selected_button = RADIOBUTTON_NONE_SELECTED;
	back_button = RADIOBUTTON_NONE_SELECTED;
}

RadioMenu::~RadioMenu() {
}

void RadioMenu::set_selected_button(s8 new_selected_button) {
	if (selected_button != RADIOBUTTON_NONE_SELECTED) {
		get_button(selected_button)->set_colors(get_button_colors(), get_button_pressed_colors());
		get_button(selected_button)->draw();
	}

	selected_button = new_selected_button;

	if (selected_button != RADIOBUTTON_NONE_SELECTED) {
		get_button(selected_button)->set_colors(get_button_pressed_colors(), get_button_pressed_colors());
		get_button(selected_button)->draw();
	}
}

void RadioMenu::set_selected_value(s32 value) {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (values[i] == value) {
			set_selected_button(i);
		}
	}
}

void RadioMenu::add_back_button(const char *button_text) {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (!get_button(i)) {
			// Create button
			set_button(i, new Button);
			get_button(i)->init(get_canvas(), get_font(), button_text);
			get_button(i)->set_colors(get_button_colors(), get_button_pressed_colors());
			get_button(i)->set_width(get_button_widths());
			get_button(i)->set_height(get_button_heights());

			back_button = i;
			break;
		}
	}
}

void RadioMenu::add_radio_button(const char *button_text, s32 value) {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		if (!get_button(i)) {
			// Create button
			set_button(i, new Button);
			get_button(i)->init(get_canvas(), get_font(), button_text);
			get_button(i)->set_colors(get_button_colors(), get_button_pressed_colors());
			get_button(i)->set_width(get_button_widths());
			get_button(i)->set_height(get_button_heights());

			values[i] = value;
			break;
		}
	}
}

s32 RadioMenu::select() {
	draw();

	while (1) {
		scanKeys();
		touchPosition stylus = touchReadXY();

		for (u8 i = 0; i < MAX_BUTTONS; i++) {
			if (get_button(i)) {
				if (get_button(i)->update(stylus) == BUTTON_CLICKED) {
					if (i == back_button) {
						return values[selected_button];
					}

					set_selected_button(i);
				}
			}
		}

		swiWaitForVBlank();
	}
}

