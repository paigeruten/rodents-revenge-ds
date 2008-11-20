#ifndef _tile_h
#define _tile_h

#include "color.h"
#include "canvas.h"

enum TileStatus { TILE_OK, TILE_NOT_LOADED, TILE_FILE_NOT_FOUND, TILE_INVALID_FILE };

class Tile {
	public:
		// Constructors
		Tile(const char *filename);
		~Tile();

		// Accessors
		u8 get_width() const { return width; }
		u8 get_height() const { return height; }
		TileStatus get_status() const { return status; }

		// Methods
		void draw(Canvas *canvas, u32 x, u32 y);
		TileStatus load_from_file(const char *filename);

	private:
		Color *data;
		u8 width;
		u8 height;
		TileStatus status;

		Color parse_palette_color(const char *string);
};

#endif

