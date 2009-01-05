#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "font.h"

Font::Font(const char *font_filename) {
	font = NULL;
	status = load_font(font_filename);
}

Font::~Font() {
	if (font) {
		delete [] font;
		font = NULL;
	}
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

void Font::print_string_right(const char *string, u16 margin, u16 y, Canvas *canvas, Color color, Color bgcolor) const {
	u16 put_x = canvas->get_width() - string_width(string) - margin;

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

s32 Font::get_num_from_file(FILE *fh) {
	char line[8];
	bool valid_number;
	s32 num_value;

	fgets(line, sizeof(line), fh);
	chop(line);
	valid_number = intval(line, &num_value);

	if (!valid_number || num_value < 0) {
		return -1;
	}

	return num_value;
}

void Font::skip_line_in_file(FILE *fh) {
	char current_char = '\0';

	while (!feof(fh) && current_char != '\n') {
		current_char = fgetc(fh);
	}
}

FontStatus Font::load_font(const char *filename) {
	char char_value;
	s32 i;

	FILE *fontfile = fopen(filename, "r");

	if (!fontfile) {
		return FONT_ERR_FILE_NOT_FOUND;
	}

	// First three numbers in file are the font width, font height, and space between chars.
	font_width = get_num_from_file(fontfile);
	font_height = get_num_from_file(fontfile);
	space_between_chars = get_num_from_file(fontfile);

	if (font_width == -1 || font_height == -1 || space_between_chars == -1) {
		fclose(fontfile);
		return FONT_ERR_INVALID_FONT_FILE;
	}

	skip_line_in_file(fontfile);

	// Get font_widths[] values
	for (i = 0; i < NUM_CHARS; i++) {
		font_widths[i] = get_num_from_file(fontfile);

		if (font_widths[i] == -1) {
			fclose(fontfile);
			return FONT_ERR_INVALID_FONT_FILE;
		}
	}

	skip_line_in_file(fontfile);

	// If a font was already loaded, delete the memory
	if (font != NULL) {
		delete [] font;
		font = NULL;
	}

	// Allocate memory for font data
	font = new u8 [font_width * font_height * NUM_CHARS];

	if (font == NULL) {
		fclose(fontfile);
		return FONT_ERR_ALLOC_MEMORY;
	}

	// For the rest of the file, copy each '1' and '0' encountered to the font[] array.
	// The i variable will keep track of how many characters have been copied so far.
	i = 0;
	char_value = fgetc(fontfile);
	while (!feof(fontfile)) {
		if (char_value == '0' || char_value == '1') {
			// Make sure we aren't past the amount of memory we allocated.
			if (i == font_width * font_height * NUM_CHARS) {
				fclose(fontfile);

				delete [] font;
				font = NULL;

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

	fclose(fontfile);

	// Make sure we got enough font data.
	if (i != font_width * font_height * NUM_CHARS) {
		delete [] font;
		font = NULL;

		return FONT_ERR_INVALID_FONT_FILE;
	}

	return FONT_OK;
}

