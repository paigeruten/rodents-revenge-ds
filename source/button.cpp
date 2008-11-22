#include <nds.h>
#include <string.h>
#include "color.h"
#include "canvas.h"
#include "font.h"
#include "button.h"

Button::Button() {
	canvas = 0;
	font = 0;
	text[0] = '\0';

	ButtonColors default_colors;
	default_colors.border = RGB(0, 0, 0);
	default_colors.background = RGB(31, 31, 31);
	default_colors.text = RGB(0, 0, 0);
	set_colors(default_colors, default_colors);

	x = 0;
	y = 0;

	state = BUTTON_NORMAL;
}

Button::Button(Canvas *the_canvas, Font *the_font, const char *the_text) {
	canvas = the_canvas;
	font = the_font;
	strncpy(text, the_text, sizeof(text));

	ButtonColors default_colors;
	default_colors.border = RGB(0, 0, 0);
	default_colors.background = RGB(31, 31, 31);
	default_colors.text = RGB(0, 0, 0);
	set_colors(default_colors, default_colors);

	x = 0;
	y = 0;

	width = font->string_width(text) + BUTTON_PADDING;
	height = font->get_font_height() + BUTTON_PADDING;

	center_text();

	state = BUTTON_NORMAL;
}

Button::~Button() {
}

void Button::set_text(const char *new_text) {
	strncpy(text, new_text, sizeof(text));
	center_text();
}

void Button::init(Canvas *the_canvas, Font *the_font, const char *the_text) {
	canvas = the_canvas;
	font = the_font;
	set_text(the_text);
}

void Button::draw() const {
	canvas->rect(x, y, x + width - 1, y + height - 1, colors[state].background, RECT_FILLED);
	canvas->rect(x, y, x + width - 1, y + height - 1, colors[state].border, RECT_OUTLINE);
	font->print_string(text, text_x, text_y, canvas, colors[state].text);
}

ButtonState Button::update(touchPosition stylus) {
	bool stylus_on_button = stylus_on(stylus);

	bool stylus_down = keysDown() & KEY_TOUCH;
	bool stylus_up = keysUp() & KEY_TOUCH;
	bool stylus_held = keysHeld() & KEY_TOUCH;

	switch (state) {
		case BUTTON_NORMAL:
			if (stylus_down && stylus_on_button) {
				state = BUTTON_PRESSED;
				draw();
			}
			break;

		case BUTTON_PRESSED:
			if (stylus_up && stylus_on(last_stylus)) {
				state = BUTTON_CLICKED;
				draw();
			} else if (stylus_held && !stylus_on_button) {
				state = BUTTON_NORMAL;
				draw();
			}
			break;

		case BUTTON_CLICKED:
			state = BUTTON_NORMAL;
			draw();
			break;

		default:
			break;
	}

	last_stylus = stylus;

	return state;
}

void Button::center_x() {
	x = (canvas->get_width() - width) / 2;
	center_text();
}

void Button::center_y() {
	y = (canvas->get_height() - height) / 2;
	center_text();
}

bool Button::stylus_on(touchPosition stylus) const {
	return ((u32)stylus.px >= x) &&
	       ((u32)stylus.px < x + width) &&
	       ((u32)stylus.py >= y) &&
	       ((u32)stylus.py < y + height);
}

void Button::center_text() {
	text_x = x + ((width - font->string_width(text)) / 2);
	text_y = y + ((height - font->get_font_height()) / 2);
}

