#include <nds.h>
#include <stdio.h>
#include "rrkeyboard.h"

RRKeyboard::RRKeyboard(const char *filename, Canvas *the_canvas, Font *the_font, u8 the_key_width, u8 space_bar_width) {
	canvas = the_canvas;
	font = the_font;
	key_width = the_key_width;
	status = load_keyboard(filename);
	width_of_space_bar = space_bar_width;
	highlighted_key = '\0';
	shift = false;

	// Center the keyboard horizontally, align near the bottom vertically.
	keyboard_x = (canvas->get_width() - calculate_keyboard_width()) / 2;
	keyboard_y = canvas->get_height() - calculate_keyboard_height() - 15;

	// Default colors
	colors.line = RGB(2, 6, 10);
	colors.key_background = RGB(2, 8, 14);
	colors.text = RGB(27, 12, 0);
	colors.background = MENU_BACKGROUND_COLOR;
	colors.highlight_key_background = RGB(27, 12, 0);
	colors.highlight_text = RGB(2, 8, 14);
}

RRKeyboard::~RRKeyboard() {
}

char RRKeyboard::key_pressed(touchPosition stylus) {
	// Use alternate keyboard if they are holding shift
	u8 which_keyboard;
	if (shift) {
		which_keyboard = 1;
	} else {
		which_keyboard = 0;
	}

	// Loop through the keyboard keys
	u8 current_key_x = keyboard_x;
	u8 current_key_y = keyboard_y;
	char the_pressed_key = '\0';
	for (u16 current_key_index = 0; keyboard[which_keyboard][current_key_index]; current_key_index++) {
		char current_key = keyboard[which_keyboard][current_key_index];

		u16 key_left, key_right, key_top, key_bottom;
		bool key_pressed;

		switch (current_key) {
			// Tab char represents a space between keys, half of the width of a key
			case '\t':
				current_key_x += key_width / 2;
				break;

			// This char represents a new row of keys (like a newline
			// except I'm using '\n' to represent the enter key).
			case KEYBOARD_NEW_ROW:
				current_key_x = keyboard_x;
				current_key_y += key_width;
				break;

			default:
				// Check if key is pressed.
				key_left = current_key_x;
				key_right = current_key_x + key_width;
				key_top = current_key_y;
				key_bottom = current_key_y + key_width;

				// Space bar could have a different width
				if (current_key == ' ') {
					key_right += width_of_space_bar;
				}

				key_pressed = ((keysHeld() & KEY_TOUCH) &&
				               stylus.px >= key_left &&
				               stylus.px < key_right &&
				               stylus.py >= key_top &&
				               stylus.py < key_bottom);

				if (key_pressed) {
					print_key(current_key, current_key_x, current_key_y, KEY_HIGHLIGHTED);
					the_pressed_key = current_key;
				} else {
					// The currently highlighted key is not pressed so unhighlight it.
					print_key(current_key, current_key_x, current_key_y, KEY_UNHIGHLIGHTED);
				}

				current_key_x += key_width;
		}
	}

	highlighted_key = the_pressed_key;

	return the_pressed_key;
}

KeyboardStatus RRKeyboard::load_keyboard(const char *filename) {
	FILE *keyboardfile = fopen(filename, "r");

	u16 current_char_index = 0;
	u8 current_keyboard = 0;
	bool do_not_fgetc = false;

	if (!keyboardfile) {
		return KEYBOARD_FILE_NOT_FOUND;
	}

	bool done_loading = false;
	char current_char = fgetc(keyboardfile);
	while (!feof(keyboardfile) && !done_loading) {
		switch (current_char) {
			// If the current char is a backslash, check if it's an escape sequence.
			case '\\':
				current_char = fgetc(keyboardfile);

				// If the char after the backslash is one of the following characters,
				// change it to the character the escape sequence represents.
				if (current_char == 'n') { current_char = '\n'; }
				if (current_char == 's') { current_char = ' '; }
				if (current_char == 'b') { current_char = '\b'; }
				if (current_char == '\\') { current_char = '\\'; }
				
				keyboard[current_keyboard][current_char_index] = current_char;
				current_char_index++;
				break;

			// A single newline represents a new row of keys. However, two newlines
			// represent the beginning the the alternate second keyboard. (Used for shifting.)
			case '\n':
				// So we have to get the next character to check if it's another newline.
				current_char = fgetc(keyboardfile);

				if (current_char == '\n') {
					keyboard[current_keyboard][current_char_index] = '\0';

					// If we were on the first keyboard, start a second one. Otherwise
					// we're done here.
					if (current_keyboard == 0) {
						current_keyboard++;
						current_char_index = 0;
					} else {
						done_loading = true;
					}
				} else {
					// It was only one newline after all, so make a new row of keys.
					keyboard[current_keyboard][current_char_index] = KEYBOARD_NEW_ROW;
					current_char_index++;

					// This causes the next loop iteration to use the current_char we just
					// got instead of fgetc()ing it.
					do_not_fgetc = true;
				}
				break;

			// Spaces in between keys are represented with the tab character.
			case ' ':
				keyboard[current_keyboard][current_char_index] = '\t';
				current_char_index++;
				break;

			// All other chars represent themselves.
			default:
				keyboard[current_keyboard][current_char_index] = current_char;
				current_char_index++;
		}

		if (do_not_fgetc) {
			do_not_fgetc = false;
		} else {
			current_char = fgetc(keyboardfile);
		}
	}

	fclose(keyboardfile);

	return KEYBOARD_OK;
}

void RRKeyboard::print_key(char key, u16 x, u16 y, bool highlighted) const {
	// If highlighted, change the colors.
	Color text_color;
	Color key_background_color;
	if (highlighted) {
		text_color = colors.highlight_text;
		key_background_color = colors.highlight_key_background;
	} else {
		text_color = colors.text;
		key_background_color = colors.key_background;
	}

	// Width and height of key.
	u8 width = key_width;
	u8 height = key_width;

	// If the key is the space bar, make the key much longer
	if (key == ' ') {
		width += width_of_space_bar;
	}

	// Fill in the box
	canvas->rect(x, y, x + width, y + height, key_background_color, RECT_FILLED);

	// Draw the border
	canvas->rect(x, y, x + width, y + height, colors.line, RECT_OUTLINE);

	// Draw the char in the middle of the key
	u32 middle_x = x + ((key_width - font->char_width(key)) / 2);
	u32 middle_y = y + ((key_width - font->get_font_height()) / 2);

	font->print_char(key, middle_x, middle_y, canvas, text_color);
}

void RRKeyboard::draw() const {
	canvas->clear(colors.background);

	// Use alternate keyboard if they are holding shift
	u8 which_keyboard;
	if (shift) {
		which_keyboard = 1;
	} else {
		which_keyboard = 0;
	}

	// Loop through the keyboard keys
	u8 current_key_index = 0;
	u32 put_x = keyboard_x;
	u32 put_y = keyboard_y;
	char current_key;
	while ((current_key = keyboard[which_keyboard][current_key_index])) {
		switch (current_key) {
			case '\t':
				put_x += key_width / 2;
				break;

			case KEYBOARD_NEW_ROW:
				put_x = keyboard_x;
				put_y += key_width;
				break;

			default:
				print_key(current_key, put_x, put_y);
				put_x += key_width;
		}

		current_key_index++;
	}
}

u32 RRKeyboard::calculate_keyboard_width() {
	u32 max_width = 0;

	for (u8 current_keyboard = 0; current_keyboard <= 1; current_keyboard++) {
		u8 current_key_index = 0;
		u32 width = 0;
		char current_key;

		while ((current_key = keyboard[current_keyboard][current_key_index])) {
			switch (current_key) {
				case '\t':
					width += key_width / 2;
					break;

				case KEYBOARD_NEW_ROW:
					if (width > max_width) {
						max_width = width;
					}

					width = 0;
					break;

				default:
					width += key_width;
			}

			current_key_index++;
		}
	}

	return max_width;
}

u32 RRKeyboard::calculate_keyboard_height() {
	u32 max_height = 0;

	for (u8 current_keyboard = 0; current_keyboard <= 1; current_keyboard++) {
		u8 current_key_index = 0;
		u32 height = 0;
		char current_key;

		while ((current_key = keyboard[current_keyboard][current_key_index])) {
			if (current_key == KEYBOARD_NEW_ROW) {
				height += key_width;
			}

			current_key_index++;
		}

		if (height > max_height) {
			max_height = height;
		}
	}

	return max_height;
}

