#include <nds.h>
#include "menuset.h"

MenuSet::MenuSet(Canvas *the_canvas, Font *the_font, Color background) {
	canvas = the_canvas;
	font = the_font;
	background_color = background;

	for (u8 i = 0; i < MAX_MENUS; i++) {
		menus[i] = 0;
	}
}

MenuSet::~MenuSet() {
	for (u8 i = 0; i < MAX_MENUS; i++) {
		if (menus[i]) {
			delete menus[i];
			menus[i] = 0;
		}
	}
}

void MenuSet::begin(MenuId top_menu) {
	Menu *current_menu = menus[top_menu];

	while (1) {
		current_menu = current_menu->select();
	}
}

MenuId MenuSet::add_menu() {
	for (u8 i = 0; i < MAX_MENUS; i++) {
		if (!menus[i]) {
			menus[i] = new Menu;
			menus[i]->init(canvas, font, background_color);
			menus[i]->set_button_widths(button_widths);
			menus[i]->set_button_heights(button_heights);
			menus[i]->set_button_colors(button_colors, button_pressed_colors);

			return i;
		}
	}

	return 255;
}

void MenuSet::add_button(MenuId menu, const char *button_text, void (*action)(void *), void *data) {
	menus[menu]->add_button(button_text, action, data);
}

void MenuSet::add_button(MenuId menu, const char *button_text, MenuId submenu) {
	menus[menu]->add_button(button_text, menus[submenu]);
}

