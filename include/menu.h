#ifndef _menu_h
#define _menu_h

#include "canvas.h"
#include "button.h"

const u8 MAX_BUTTONS = 10;
const u8 BUTTON_MARGIN_LEFT = 16;

class Menu {
	public:
		// Constructors
		Menu(Canvas *the_canvas, Color background);
		~Menu();

		// Methods
		Menu *select();
		void draw();
		void add_button(Button *button, Menu *submenu);
		void add_button(Button *button, void (*action)());

	private:
		Canvas *canvas;
		Color background_color;

		Button *buttons[MAX_BUTTONS];
		Menu *submenus[MAX_BUTTONS];
		void (*actions[MAX_BUTTONS])();

		void arrange_buttons();
		u8 button_count() const;
};

#endif

