#ifndef _button_h
#define _button_h

#include "color.h"
#include "canvas.h"
#include "font.h"

struct ButtonColors {
	Color border;
	Color background;
	Color text;
};

enum ButtonState { BUTTON_NORMAL = 0, BUTTON_PRESSED = 1, BUTTON_CLICKED = 2 };

const u8 BUTTON_PADDING = 6;

class Button {
	public:
		// Constructors
		Button(Canvas *the_canvas, Font *the_font, const char *the_text);
		~Button();

		// Accessors
		void set_text(const char *new_text);
		const char *get_text() const { return text; }
		void set_colors(ButtonColors normal, ButtonColors pressed) { colors[BUTTON_NORMAL] = normal; colors[BUTTON_PRESSED] = pressed; colors[BUTTON_CLICKED] = pressed; }
		void set_position(u32 new_x, u32 new_y) { x = new_x; y = new_y; center_text(); }
		u32 get_x() const { return x; }
		u32 get_y() const { return y; }
		void set_width(u32 new_width) { width = new_width; center_text(); }
		void set_height(u32 new_height) { height = new_height; center_text(); }
		u32 get_width() const { return width; }
		u32 get_height() const { return height; }
		ButtonState get_state() const { return state; }

		// Methods
		void draw() const;
		ButtonState update(touchPosition stylus);
		void center_x();
		void center_y();

	private:
		Canvas *canvas;
		Font *font;
		char text[128];
		ButtonColors colors[3];
		u32 x;
		u32 y;
		u32 width;
		u32 height;
		ButtonState state;
		u32 text_x;
		u32 text_y;

		bool stylus_on(touchPosition stylus) const;
		void center_text();
};

#endif

