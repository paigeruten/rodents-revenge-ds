#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "font.h"
#include "str.h"

Font::Font(const char *font_filename) {
	font = 0;
	status = load_font(font_filename);
}

Font::~Font() {
	if (font) {
		delete [] font;
		font = 0;
	}
}

FontStatus Font::load_font(const char *filename) {
	char line[8];
	char char_value;
	s32 num_value;
	s32 i;
	bool valid_number;

	FILE *fontfile = fopen(filename, "r");

	if (!fontfile) {
		return FONT_ERR_FILE_NOT_FOUND;
	}

	// Get font width
	fgets(line, sizeof(line), fontfile);
	chop(line);
	valid_number = intval(line, &num_value);
	if (!valid_number || num_value < 0) { fclose(fontfile); return FONT_ERR_INVALID_FONT_FILE; }
	font_width = num_value;

	// Get font height
	fgets(line, sizeof(line), fontfile);
	chop(line);
	valid_number = intval(line, &num_value);
	if (!valid_number || num_value < 0) { fclose(fontfile); return FONT_ERR_INVALID_FONT_FILE; }
	font_height = num_value;

	// Get space between chars
	fgets(line, sizeof(line), fontfile);
	chop(line);
	valid_number = intval(line, &num_value);
	if (!valid_number || num_value < 0) { fclose(fontfile); return FONT_ERR_INVALID_FONT_FILE; }
	space_between_chars = num_value;

	// Skip a line
	fgets(line, sizeof(line), fontfile);

	// Get font_widths[] values
	for (i = 0; i < NUM_CHARS; i++) {
		fgets(line, sizeof(line), fontfile);
		chop(line);
		valid_number = intval(line, &num_value);
		if (!valid_number || num_value < 0) { fclose(fontfile); return FONT_ERR_INVALID_FONT_FILE; }
		font_widths[i] = num_value;
	}

	// Skip a line
	fgets(line, sizeof(line), fontfile);

	// If a font was already loaded, delete the memory
	if (font) {
		delete [] font;
		font = 0;
	}

	// Allocate memory for font data
	font = new u8 [font_width * font_height * NUM_CHARS];
	if (font == 0) { fclose(fontfile); return FONT_ERR_ALLOC_MEMORY; }

	// For the rest of the file, copy each '1' and '0' encountered to the font[] array.
	// The i variable will keep track of how many characters have been copied so far.
	i = 0;
	char_value = fgetc(fontfile);
	while (!feof(fontfile)) {
		if (char_value == '0' || char_value == '1') {
			// Make sure we aren't past the amount of memory we allocated.
			if (i == font_width * font_height * NUM_CHARS) {
				fclose(fontfile);
				fontfile = 0;
				delete [] font;
				font = 0;
				return FONT_ERR_INVALID_FONT_FILE;
			}

			switch (char_value) {
				case '0': font[i] = 0; break;
				case '1': font[i] = 1; break;

				default:;
			}

			i++;
		}

		char_value = fgetc(fontfile);
	}

	// Make sure we got enough font data.
	if (i != font_width * font_height * NUM_CHARS) {
		fclose(fontfile);
		fontfile = 0;
		delete [] font;
		font = 0;
		return FONT_ERR_INVALID_FONT_FILE;
	}

	fclose(fontfile);

	return FONT_OK;
}

void Font::print_char(char character, u16 x, u16 y, Canvas *canvas, Color color, Color bgcolor) const {
	u16 put_x;
	u16 put_y;

	u16 bottom_edge = y + font_height;
	u16 right_edge = x + char_width(character);
	u32 offset = (u8)character * font_width * font_height;

	for (put_y = y; put_y < bottom_edge; put_y++) {
		for (put_x = x; put_x < right_edge; put_x++) {
			if (font[offset + (put_x - x) + (put_y - y) * font_width]) {
				canvas->plot(put_x, put_y, color);
			} else if (bgcolor != TRANSPARENT) {
				canvas->plot(put_x, put_y, bgcolor);
			}
		}
	}
	
	// Fill in space to the right of the char if there's a solid background colour.
	if (bgcolor != TRANSPARENT) {
		for (put_y = y; put_y < bottom_edge; put_y++) {
			for (put_x = right_edge; put_x < right_edge + space_between_chars; put_x++) {
				canvas->plot(put_x, put_y, bgcolor);
			}
		}
	}
}

void Font::print_string(const char *string, u16 x, u16 y, Canvas *canvas, Color color, Color bgcolor) const {
	u16 put_x = x;
	u16 put_y = y;

	for (u32 current_char = 0; string[current_char]; current_char++) {
		print_char(string[current_char], put_x, put_y, canvas, color, bgcolor);
		put_x += space_between_chars + char_width(string[current_char]);
	}
}

void Font::print_string_center(const char *string, u16 y, Canvas *canvas, Color color, Color bgcolor) const {
	// Calculate where to start printing the string so it'll appear in the center.
	u16 put_x = (canvas->get_width() - string_width(string)) / 2;
	
	print_string(string, put_x, y, canvas, color, bgcolor);
}

void Font::print_number(s32 num, u16 x, u16 y, Canvas *canvas, Color color, Color bgcolor) const {
	print_string(strval(num), x, y, canvas, color, bgcolor);
}

u32 Font::string_width(const char *string) const {
	u32 width = 0;
	
	for (u32 current_character = 0; string[current_character]; current_character++) {
		width += char_width(string[current_character]);
		width += space_between_chars;
	}

	width -= space_between_chars;
	
	return width;
}

