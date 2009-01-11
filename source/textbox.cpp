#include <nds.h>
#include <stdlib.h>
#include <string.h>
#include "textbox.h"

Textbox::Textbox(Canvas *the_canvas, Font *the_font) {
	canvas = the_canvas;
	font = the_font;
	x = 0;
	y = 0;
	width = 1;
	height = 1;
	vblank_count = 0;
	set_colors(TEXTBOX_DEFAULT_COLORS);
	set_value("");
}

Textbox::~Textbox() {
}

void Textbox::draw(bool draw_cursor) {
	Color cursor_color;

	// Draw the border
	canvas->rect(x, y, x + width, y + height, colors.border, RECT_OUTLINE);

	// Fill in the box
	canvas->rect(x + 1, y+ 1, x + width - 1, y + height - 1, colors.background, RECT_FILLED);

	// Output the value
	font->print_string(value, x + 3, y + 3, canvas, colors.font);

	if (draw_cursor) {
		// Make the cursor blink
		if (vblank_count < 30) {
			cursor_color = colors.font;
		} else if (vblank_count < 45) {
			cursor_color = colors.background;
		} else {
			cursor_color = colors.font;
			vblank_count = 0;
		}

		// Output the cursor
		canvas->rect(cursor_x, y + 3, cursor_x, y + 3 + font->get_font_height(), cursor_color, RECT_FILLED);

		vblank_count++;
	}
}

bool Textbox::pressed(touchPosition stylus) const {
	return (stylus.px >= x &&
	        stylus.px <= x + width &&
	        stylus.py >= y &&
	        stylus.py <= y + height &&
	        (keysHeld() & KEY_TOUCH));
}

void Textbox::set_value(const char *new_value) {
	strncpy(value, new_value, TEXTBOX_MAX_LENGTH);
	cursor_x = x + 3 + font->string_width(value) + 2;
}

