#include <nds.h>
#include <stdio.h>
#include <string.h>
#include "tile.h"
#include "str.h"

Tile::Tile() {
	data = 0;
	width = 0;
	height = 0;
	status = TILE_NOT_LOADED;
}

Tile::Tile(const char *filename) {
	data = 0;
	width = 0;
	height = 0;

	status = load_from_file(filename);
}

Tile::~Tile() {
	if (data) {
		delete [] data;
		data = 0;
	}
}

void Tile::draw(Canvas *canvas, u32 x, u32 y) {
	u32 get_x, get_y;

	get_x = 0;
	for (u32 put_x = x; put_x < x + width; put_x++) {
		get_y = 0;

		for (u32 put_y = y; put_y < y + height; put_y++) {
			Color pixel = data[get_x + get_y * width];

			if (pixel != TRANSPARENT) {
				canvas->plot(put_x, put_y, pixel);
			}

			get_y++;
		}
		get_x++;
	}
}

TileStatus Tile::load_from_file(const char *filename) {
	FILE *file = fopen(filename, "r");

	if (!file) {
		return TILE_FILE_NOT_FOUND;
	}

	char line[32];
	s32 num_value;
	bool valid_number;

	// Get tile width
	fgets(line, sizeof(line), file);
	chop(line);
	valid_number = intval(line, &num_value);
	valid_number = valid_number && (num_value >= 0 && num_value <= 255);
	if (!valid_number) { fclose(file); return TILE_INVALID_FILE; }
	width = num_value;

	// Get tile height
	fgets(line, sizeof(line), file);
	chop(line);
	valid_number = intval(line, &num_value);
	valid_number = valid_number && (num_value >= 0 && num_value <= 255);
	if (!valid_number) { fclose(file); return TILE_INVALID_FILE; }
	height = num_value;

	// Should be a blank line now
	fgets(line, sizeof(line), file);

	// Now make the palette, loop until we hit a blank line
	Color palette[10] = { TRANSPARENT };
	u8 i = 1;
	while (strcmp("\n", fgets(line, sizeof(line), file))) {
		palette[i] = parse_palette_color(line);
		i++;
	}

	// Allocate tile data
	if (data) {
		delete [] data;
		data = 0;
	}

	data = new Color [width * height];

	// Finally, read the tile data
	char current_char;
	u32 index = 0;
	while (!feof(file)) {
		current_char = fgetc(file);

		if (current_char >= '0' && current_char <= '9') {
			// Make sure we're not past the end of the allocated data.
			if (index == width * height) {
				delete [] data;
				data = 0;

				fclose(file);
				return TILE_INVALID_FILE;
			}

			data[index] = palette[current_char - '0'];
			index++;
		}
	}

	// Make sure we got enough data
	if (index < width * height) {
		delete [] data;
		data = 0;

		fclose(file);
		return TILE_INVALID_FILE;
	}

	fclose(file);

	return TILE_OK;
}

Color Tile::parse_palette_color(const char *string) {
	u16 current_string_char = 2;
	u8 current_num_string_char = 0;
	char num_string[10];
	u8 current_color = 0;
	s32 colors[3];

	while (string[current_string_char]) {
		switch (string[current_string_char]) {
			case ',':
			case '\n':
				num_string[current_num_string_char] = '\0';
				current_num_string_char = 0;

				intval(num_string, &colors[current_color]);
				current_color++;
				break;

			default:
				num_string[current_num_string_char] = string[current_string_char];
				current_num_string_char++;
		}

		current_string_char++;
	}

	return RGB(colors[0], colors[1], colors[2]);
}

