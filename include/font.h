#ifndef _font_h
#define _font_h

#include <stdio.h>
#include "color.h"
#include "canvas.h"
#include "str.h"

enum FontStatus { FONT_OK, FONT_ERR_FILE_NOT_FOUND, FONT_ERR_INVALID_FONT_FILE, FONT_ERR_ALLOC_MEMORY };

const u8 NUM_CHARS = 128;

class Font {
	public:
		// Constructors
		Font(const char *font_filename);
		~Font();

		// Accessors
		u8 get_space_between_chars() const { return space_between_chars; }
		void set_space_between_chars(u8 sbc) { space_between_chars = sbc; }
		FontStatus get_status() const { return status; }
		u8 get_font_height() const { return font_height; }
		u8 get_font_width() const { return font_width; }

		// Printing characters/strings/numbers
		void print_char(char character, u16 x, u16 y, Canvas *canvas, Color color = RGB(0, 0, 0), Color bgcolor = TRANSPARENT) const;
		void print_string(const char *string, u16 x, u16 y, Canvas *canvas, Color color = RGB(0, 0, 0), Color bgcolor = TRANSPARENT) const;
		void print_string_center(const char *string, u16 y, Canvas *canvas, Color color = RGB(0, 0, 0), Color bgcolor = TRANSPARENT) const;
		void print_number(s32 num, u16 x, u16 y, Canvas *canvas, Color color = RGB(0, 0, 0), Color bgcolor = TRANSPARENT) const;

		// Returns width of string/single char in pixels
		u32 string_width(const char *string) const;
		u8 char_width(char ch) const { return font_widths[(u8)ch]; }

	private:
		u8 *font;
		s8 font_widths[NUM_CHARS];
		s8 space_between_chars;
		s8 font_height;
		s8 font_width;
		FontStatus status;

		s32 get_num_from_file(FILE *fh);
		void skip_line_in_file(FILE *fh);
		FontStatus load_font(const char *filename);
};

#endif
