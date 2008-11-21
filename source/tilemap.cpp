#include <nds.h>
#include "canvas.h"
#include "tile.h"
#include "tilemap.h"

TileMap::TileMap(Canvas *the_canvas, u8 the_width, u8 the_height, u8 the_tile_width, u8 the_tile_height) {
	canvas = the_canvas;
	width = the_width;
	height = the_height;
	tile_width = the_tile_width;
	tile_height = the_tile_height;

	map = new TileNum [width * height];

	for (u16 i = 0; i < width * height; i++) {
		map[i] = 0;
	}

	for (u16 i = 0; i < MAX_TILES; i++) {
		tiles[i] = 0;
	}
}

TileMap::~TileMap() {
	delete [] map;
	map = 0;
}

TileNum TileMap::add_tile(Tile *tile) {
	for (u16 i = 0; i < MAX_TILES; i++) {
		if (!tiles[i]) {
			tiles[i] = tile;
			return i;
		}
	}

	return 0;
}

void TileMap::remove_tile(TileNum num) {
	tiles[num] = 0;

	for (u16 i = 0; i < width * height; i++) {
		if (map[i] == num) {
			map[i] = 0;
		}
	}
}

void TileMap::draw(u16 x, u16 y) const {
	u16 put_x, put_y;

	put_x = x;
	for (u16 get_x = 0; get_x < width; get_x++) {
		put_y = y;
		for (u16 get_y = 0; get_y < height; get_y++) {
			tiles[map[get_x + get_y * width]]->draw(canvas, put_x, put_y);
			put_y += tile_height;
		}
		put_x += tile_width;
	}
}

