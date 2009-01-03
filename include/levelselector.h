#ifndef _levelselector_h
#define _levelselector_h

#include "canvas.h"
#include "font.h"
#include "button.h"
#include "level.h"
#include "menu.h"

const u8 LEVEL_SELECTOR_BUTTON_WIDTHS = 20;
const u8 LEVEL_SELECTOR_BUTTON_HEIGHTS = 20;

const u8 BUTTON_OK_Y = SCREEN_HEIGHT - 48;

const u8 LEVEL_SELECTOR_BUTTON_SPACING = 5;
const u8 LEVEL_SELECTOR_BUTTON_COLUMNS = 10;

class LevelSelector {
	public:
		// Constructors
		LevelSelector(Font *the_font, Font *the_small_font, Color background);
		~LevelSelector();

		// Accessors
		void set_selected_level(LevelId level_id);

		// Methods
		void draw(); 
		LevelId select_level();

	private:
		Font *font;
		Font *small_font;
		Level level;
		u8 num_buttons;
		Button *buttons[MAX_LEVELS];
		u8 selected_button;
		Button button_ok;
		Color background_color;
};

#endif
