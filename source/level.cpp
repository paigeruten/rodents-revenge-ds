#include <nds.h>
#include "level.h"

Level::Level() {
	canvas = 0;
	current_level = 0;
	num_levels = 0;

	for (u8 i = 0; i < MAX_LEVELS; i++) {
		levels[i].id = 0;
	}
}

Level::~Level() {
}

void Level::init(Canvas *the_canvas) {
	canvas = the_canvas;

	map.init(canvas, LEVEL_WIDTH, LEVEL_HEIGHT, TILE_WIDTH, TILE_HEIGHT);

	tiles[0].load_from_file("/data/rodents-revenge/tiles/empty.tile");
	tiles[1].load_from_file("/data/rodents-revenge/tiles/movable_block.tile");
	tiles[2].load_from_file("/data/rodents-revenge/tiles/stationary_block.tile");
	tiles[3].load_from_file("/data/rodents-revenge/tiles/mouse.tile");
	tiles[4].load_from_file("/data/rodents-revenge/tiles/cat.tile");
	tiles[5].load_from_file("/data/rodents-revenge/tiles/yarn.tile");
	tiles[6].load_from_file("/data/rodents-revenge/tiles/trap.tile");
	tiles[7].load_from_file("/data/rodents-revenge/tiles/sinkhole.tile");
	tiles[8].load_from_file("/data/rodents-revenge/tiles/cheese.tile");
	tiles[9].load_from_file("/data/rodents-revenge/tiles/mouse_sinkhole.tile");
	tiles[10].load_from_file("/data/rodents-revenge/tiles/border_opening_vertical.tile");
	tiles[11].load_from_file("/data/rodents-revenge/tiles/border_opening_horizontal.tile");
	tiles[12].load_from_file("/data/rodents-revenge/tiles/cat_sitting.tile");

	map.add_tile(&tiles[0], TILE_EMPTY);
	map.add_tile(&tiles[1], TILE_MOVABLE_BLOCK);
	map.add_tile(&tiles[2], TILE_STATIONARY_BLOCK);
	map.add_tile(&tiles[3], TILE_MOUSE);
	map.add_tile(&tiles[4], TILE_CAT);
	map.add_tile(&tiles[5], TILE_YARN);
	map.add_tile(&tiles[6], TILE_MOUSE_TRAP);
	map.add_tile(&tiles[7], TILE_SINK_HOLE);
	map.add_tile(&tiles[8], TILE_CHEESE);
	map.add_tile(&tiles[9], TILE_MOUSE_SINKHOLE);
	map.add_tile(&tiles[10], TILE_BORDER_OPENING_VERTICAL);
	map.add_tile(&tiles[11], TILE_BORDER_OPENING_HORIZONTAL);
	map.add_tile(&tiles[12], TILE_CAT_SITTING);

	load_levels_from_file("/data/rodents-revenge/levels");
}

void Level::load() {

}

bool Level::last() {
	LevelId last = 0;

	for (u8 i = 0; i < num_levels; i++) {
		if (levels[i].id > last) {
			last = levels[i].id;
		}
	}

	return (current_level == last);
}

void Level::next() {
	if (!last()) {
		for (u8 i = 0; i < num_levels; i++) {
			if (levels[i].id == current_level) {
				current_level = levels[i + 1].id;
				break;
			}
		}
	}
}

void Level::load_levels_from_file(const char *filename) {
	// Temporarily hardcoded for testing
	num_levels = 1;
	
}

