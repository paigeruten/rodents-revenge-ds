#include <nds.h>
#include "canvas.h"
#include "button.h"
#include "menu.h"
#include "actionmenu.h"

ActionMenu::ActionMenu() {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		actions[i] = NULL;
		actions_data[i] = NULL;
		submenus[i] = NULL;
	}
}

ActionMenu::ActionMenu(Canvas *the_canvas, Font *the_font, Color background) :
Menu(the_canvas, the_font, background) {
	for (u8 i = 0; i < MAX_BUTTONS; i++) {
		actions[i] = NULL;
		actions_data[i] = NULL;
		submenus[i] = NULL;
	}
}

ActionMenu::~ActionMenu() {
}

ActionMenu *ActionMenu::select() {
	draw();

	while (1) {
		scanKeys();
		touchPosition stylus = touchReadXY();

		for (u8 i = 0; i < MAX_BUTTONS; i++) {
			if (get_button(i)) {
				if (get_button(i)->update(stylus) == BUTTON_CLICKED) {
					if (submenus[i]) {
						return submenus[i];
					}

					if (actions[i]) {
						(*actions[i])(actions_data[i]);
						draw();
					}
				}
			}
		}

		swiWaitForVBlank();
	}
}

void ActionMenu::add_button(const char *button_text, ActionMenu *submenu) {
	add_button(button_text, submenu, NULL, NULL);
}

void ActionMenu::add_button(const char *button_text, void (*action)(void *), void *data) {
	add_button(button_text, NULL, action, data);
}

void ActionMenu::add_button(const char *button_text, ActionMenu *submenu, void (*action)(void *), void *data) {
	u8 button_id = add_menu_button(button_text);

	actions[button_id] = action;
	actions_data[button_id] = data;
	submenus[button_id] = submenu;
}

