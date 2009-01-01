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

// These values are used to index into the Button::colors[] array.
enum ButtonState {
	BUTTON_NORMAL = 0,
	BUTTON_PRESSED,
	BUTTON_CLICKED,

	NUM_BUTTON_STATES
};

const u8 BUTTON_DEFAULT_PADDING = 6;
const u16 MAX_BUTTON_TEXT_LENGTH = 256;
const ButtonColors BUTTON_DEFAULT_COLORS = { RGB(0, 0, 0), RGB(31, 31, 31), RGB(0, 0, 0) };

class Button {
	public:
		// Constructors
		Button();
		Button(Canvas *the_canvas, Font *the_font, const char *the_text);
		~Button();

		// Accessors
		void set_canvas(Canvas *new_canvas) { canvas = new_canvas; }
		void set_text(const char *new_text);
		const char *get_text() const { return text; }
		void set_colors(ButtonColors normal, ButtonColors pressed);
		void set_position(u32 new_x, u32 new_y);
		void set_x(u32 new_x);
		void set_y(u32 new_y);
		u32 get_x() const { return x; }
		u32 get_y() const { return y; }
		void set_width(u32 new_width);
		void set_height(u32 new_height);
		u32 get_width() const { return width; }
		u32 get_height() const { return height; }
		ButtonState get_state() const { return state; }

		// Methods
		void init(Canvas *the_canvas, Font *the_font, const char *the_text);
		void draw() const;
		ButtonState update(touchPosition stylus);
		void center_x();
		void center_y();

	private:
		Canvas *canvas;
		Font *font;
		char text[MAX_BUTTON_TEXT_LENGTH];
		ButtonColors colors[NUM_BUTTON_STATES];
		u32 x;
		u32 y;
		u32 width;
		u32 height;
		ButtonState state;
		u32 text_x;
		u32 text_y;
		touchPosition last_stylus;

		void update_state(touchPosition stylus);
		bool stylus_on(touchPosition stylus) const;
		void center_text();
};

#endif

