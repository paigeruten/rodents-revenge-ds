#ifndef _actionmenu_h
#define _actionmenu_h

#include "canvas.h"
#include "button.h"
#include "menu.h"

class ActionMenu : public Menu {
	public:
		// Constructors
		ActionMenu();
		ActionMenu(Canvas *the_canvas, Font *the_font, Color background);
		~ActionMenu();

		// Methods
		ActionMenu *select();
		void add_button(const char *button_text, ActionMenu *submenu);
		void add_button(const char *button_text, void (*action)(void *), void *data);

	private:
		ActionMenu *submenus[MAX_BUTTONS];
		void (*actions[MAX_BUTTONS])(void *);
		void *actions_data[MAX_BUTTONS];

		void add_button(const char *button_text, ActionMenu *submenu, void (*action)(void *), void *data);
};

#endif

