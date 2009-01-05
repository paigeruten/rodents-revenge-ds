#ifndef _help_h
#define _help_h

#include "canvas.h"
#include "font.h"
#include "button.h"
#include "menu.h"
#include "tile.h"
#include "options.h"

const u8 NUM_PAGES = 5;

void help(void *fonts);
void print_help_page(FontSet *fonts, u8 page);

#endif

