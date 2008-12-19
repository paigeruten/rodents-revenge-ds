#ifndef _tilemap_h
#define _tilemap_h

#include "canvas.h"
#include "tile.h"

typedef u8 TileNum;

const u8 MAX_TILES = 32;

class TileMap {
	public:
		// Constructors
		TileMap();
		TileMap(Canvas *the_canvas, u8 the_width, u8 the_height, u8 the_tile_width, u8 the_tile_height);
		~TileMap();

		// Accessors
		void set_canvas(Canvas *new_canvas) { canvas = new_canvas; }
		u8 get_width() const { return width; }
		u8 get_height() const { return height; }
		TileNum get_tile(u8 x, u8 y) const { return map[x + y * width]; }
		void set_tile(u8 x, u8 y, TileNum num) { map[x + y * width] = num; }
		u8 get_tile_width() const { return tile_width; }
		u8 get_tile_height() const { return tile_height; }
		void set_tile_width(u8 new_tile_width) { tile_width = new_tile_width; }
		void set_tile_height(u8 new_tile_height) { tile_height = new_tile_height; }

		// Methods
		void init(Canvas *the_canvas, u8 the_width, u8 the_height, u8 the_tile_width, u8 the_tile_height);
		void add_tile(Tile *tile, TileNum num);
		void remove_tile(TileNum num);
		void draw(u16 x, u16 y) const;
		float distance(u8 x1, u8 y1, u8 x2, u8 y2);

	private:
		Canvas *canvas;
		TileNum *map;
		Tile *tiles[MAX_TILES];
		u8 width;
		u8 height;
		u8 tile_width;
		u8 tile_height;
};

#endif

