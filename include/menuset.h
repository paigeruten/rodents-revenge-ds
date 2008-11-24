#ifndef _menuset_h
#define _menuset_h

#include "color.h"
#include "canvas.h"
#include "font.h"
#include "menu.h"
#include "button.h"

const u8 MAX_MENUS = 32;

typedef u8 MenuId;

class MenuSet {
	public:
		// Constructors
		MenuSet(Canvas *the_canvas, Font *the_font, Color background);
		~MenuSet();

		// Accessors
		void set_button_widths(u16 new_button_widths) { button_widths = new_button_widths; }
		u16 get_button_widths() const { return button_widths; }
		void set_button_heights(u16 new_button_heights) { button_heights = new_button_heights; }
		u16 get_button_heights() const { return button_heights; }
		void set_button_colors(ButtonColors colors, ButtonColors pressed_colors) { button_colors = colors; button_pressed_colors = pressed_colors; }

		// Methods
		void begin(MenuId top_menu);
		MenuId add_menu();
		void add_button(MenuId menu, const char *button_text, void (*action)(void *), void *data);
		void add_button(MenuId menu, const char *button_text, MenuId submenu);

	private:
		Canvas *canvas;
		Font *font;
		Menu *menus[MAX_MENUS];
		Color background_color;
		u16 button_widths;
		u16 button_heights;
		ButtonColors button_colors;
		ButtonColors button_pressed_colors;
};

#endif

