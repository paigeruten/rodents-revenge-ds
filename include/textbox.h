#ifndef _textbox_h
#define _textbox_h

#include "canvas.h"
#include "color.h"
#include "font.h"

struct TextboxColors {
	Color border;
	Color background;
	Color font;
};

const TextboxColors TEXTBOX_DEFAULT_COLORS = {
	RGB(2, 6, 10),
	RGB(2, 8, 14),
	RGB(27, 12, 0)
};

const u16 TEXTBOX_MAX_LENGTH = 256;

class Textbox {
	public:
		// Constructors
		Textbox(Canvas *the_canvas, Font *the_font);
		~Textbox();

		// Draw to screen, with cursor if draw_cursor is true
		void draw(bool draw_cursor);

		// Check if stylus is touching it
		bool pressed(touchPosition stylus) const;

		// Accessors
		void set_value(const char *new_value);
		char *get_value() { return value; }
		u8 get_x() const { return x; }
		u8 get_y() const { return y; }
		void set_x(u8 new_x) { x = new_x; }
		void set_y(u8 new_y) { y = new_y; }
		void set_dimensions(u8 new_width, u8 new_height) { width = new_width; height = new_height; }
		void set_colors(TextboxColors new_colors) { colors = new_colors; }

	private:
		Canvas *canvas;
		Font *font;
		char value[TEXTBOX_MAX_LENGTH];
		u8 x;
		u8 y;
		u8 width;
		u8 height;
		u8 vblank_count;
		u8 cursor_x;
		TextboxColors colors;
};

#endif
