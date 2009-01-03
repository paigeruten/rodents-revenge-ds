#ifndef _menu_h
#define _menu_h

#include "canvas.h"
#include "button.h"

const u8 MAX_BUTTONS = 32;
const Color MENU_DEFAULT_BACKGROUND_COLOR = RGB(31, 31, 31);

const Color MENU_BACKGROUND_COLOR = RGB(8, 14, 20);
const u8 MENU_BUTTON_WIDTHS = SCREEN_WIDTH - 64;
const u8 MENU_BUTTON_HEIGHTS = 32;

class Menu {
	public:
		// Constructors
		Menu();
		Menu(Canvas *the_canvas, Font *the_font, Color background);
		~Menu();

		// Accessors
		void set_background_color(Color new_bgcolor) { background_color = new_bgcolor; }
		void set_button_widths(u16 new_button_widths) { button_widths = new_button_widths; }
		u16 get_button_widths() const { return button_widths; }
		void set_button_heights(u16 new_button_heights) { button_heights = new_button_heights; }
		u16 get_button_heights() const { return button_heights; }
		void set_button_colors(ButtonColors colors, ButtonColors pressed_colors) { button_colors = colors; button_pressed_colors = pressed_colors; }
		ButtonColors get_button_colors() const { return button_colors; }
		ButtonColors get_button_pressed_colors() const { return button_pressed_colors; }
		Button *get_button(u8 button_id) { return buttons[button_id]; }
		void set_button(u8 button_id, Button *button) { buttons[button_id] = button; }

		// Methods
		void init(Canvas *the_canvas, Font *the_font, Color background);
		void draw();
		u8 add_menu_button(const char *button_text);

	private:
		Canvas *canvas;
		Font *font;
		Color background_color;
		u16 button_widths;
		u16 button_heights;
		ButtonColors button_colors;
		ButtonColors button_pressed_colors;

		Button *buttons[MAX_BUTTONS];

		void arrange_buttons();
		u8 button_count() const;
};

#endif

