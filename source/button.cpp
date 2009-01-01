#include <nds.h>
#include <string.h>
#include "color.h"
#include "canvas.h"
#include "font.h"
#include "button.h"

Button::Button() {
	init(NULL, NULL, "");

	set_colors(BUTTON_DEFAULT_COLORS, BUTTON_DEFAULT_COLORS);

	x = 0;
	y = 0;

	state = BUTTON_NORMAL;
}

Button::Button(Canvas *the_canvas, Font *the_font, const char *the_text) {
	init(the_canvas, the_font, the_text);

	set_colors(BUTTON_DEFAULT_COLORS, BUTTON_DEFAULT_COLORS);

	x = 0;
	y = 0;

	width = font->string_width(text) + BUTTON_DEFAULT_PADDING;
	height = font->get_font_height() + BUTTON_DEFAULT_PADDING;

	center_text();

	state = BUTTON_NORMAL;
}

Button::~Button() {
}

void Button::set_colors(ButtonColors normal, ButtonColors pressed) {
	colors[BUTTON_NORMAL] = normal;
	colors[BUTTON_PRESSED] = pressed;
	colors[BUTTON_CLICKED] = pressed;
}

void Button::set_position(u32 new_x, u32 new_y) {
	x = new_x;
	y = new_y;

	center_text();
}

void Button::set_x(u32 new_x) {
	x = new_x;

	center_text();
}

void Button::set_y(u32 new_y) {
	y = new_y;

	center_text();
}

void Button::set_width(u32 new_width) {
	width = new_width;

	center_text();
}

void Button::set_height(u32 new_height) {
	height = new_height;

	center_text();
}

void Button::set_text(const char *new_text) {
	strncpy(text, new_text, MAX_BUTTON_TEXT_LENGTH);
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
	ButtonState old_state = state;

	update_state(stylus);

	if (state != old_state) {
		draw();
	}

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

void Button::update_state(touchPosition stylus) {
	bool stylus_on_button = stylus_on(stylus);

	bool stylus_down = keysDown() & KEY_TOUCH;
	bool stylus_up = keysUp() & KEY_TOUCH;
	bool stylus_held = keysHeld() & KEY_TOUCH;

	switch (state) {
		case BUTTON_NORMAL:
			if (stylus_down && stylus_on_button) {
				state = BUTTON_PRESSED;
			}
			break;

		case BUTTON_PRESSED:
			if (stylus_up && stylus_on(last_stylus)) {
				state = BUTTON_CLICKED;
			} else if (stylus_held && !stylus_on_button) {
				state = BUTTON_NORMAL;
			}
			break;

		case BUTTON_CLICKED:
			state = BUTTON_NORMAL;
			break;

		default:
			break;
	}

	last_stylus = stylus;
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

