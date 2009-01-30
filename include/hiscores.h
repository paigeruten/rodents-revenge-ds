#ifndef _hiscores_h
#define _hiscores_h

#include "canvas.h"
#include "font.h"
#include "options.h"
#include "rrkeyboard.h"
#include "textbox.h"
#include "menu.h"
#include "str.h"

struct HighscoreEntry {
	char name[128];
	u32 score;
};

const u8 MAX_HIGH_SCORES = 10;

bool is_high_score(u32 score);
bool add_high_score(u32 score, Font *font, Font *keyboard_font);
void display_high_scores(Font *font);
void reset_high_scores();
void load_high_scores();
void save_high_scores();

#endif

