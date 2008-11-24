#include <nds.h>
#include <stdio.h>
#include <string.h>
#include "canvas.h"
#include "font.h"
#include "options.h"
#include "str.h"
#include "tile.h"
#include "tilemap.h"
#include "game.h"

Game::Game(Canvas *the_canvas, Font *the_font) {
	canvas = the_canvas;
	font = the_font;

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
}

Game::~Game() {
}

u32 Game::begin() {
	lives = NUM_LIVES;
	level = options.get_start_level();
	score = 0;
	state = STATE_NORMAL;

	while (lives) {
		load_level();
		play_level();

		if (level < 50) {
			level++;
		}
	}

	return score;
}

void Game::load_level() {
	char level_path[256];

	strcpy(level_path, "/data/rodents-revenge/levels/");
	strcat(level_path, strval(level));
	strcat(level_path, ".level");

	FILE *level_file = fopen(level_path, "r");

	for (u8 y = 0; y < LEVEL_HEIGHT; y++) {
		for (u8 x = 0; x < LEVEL_WIDTH; x++) {
			char current_char = fgetc(level_file);

			switch (current_char) {
				case ' ':
					map.set_tile(x, y, TILE_EMPTY);
					break;

				case 'O':
					map.set_tile(x, y, TILE_MOVABLE_BLOCK);
					break;

				case '-':
				case '|':
				case '+':
				case '@':
					map.set_tile(x, y, TILE_STATIONARY_BLOCK);
					break;

				case 'M':
					map.set_tile(x, y, TILE_MOUSE);
					mouse_x = x;
					mouse_y = y;
					break;

				case 'T':
					map.set_tile(x, y, TILE_MOUSE_TRAP);
					break;

				case 'S':
					map.set_tile(x, y, TILE_SINK_HOLE);
					break;

				default:
					map.set_tile(x, y, TILE_EMPTY);
			}
		}

		// Skip newline
		fgetc(level_file);
	}

	fclose(level_file);
}

void Game::play_level() {
	map.draw(0, 0);

	while (lives) {
		scanKeys();

		if (keysDown() & (KEY_LEFT | KEY_RIGHT | KEY_UP | KEY_DOWN)) {
			u8 new_mouse_x = mouse_x;
			u8 new_mouse_y = mouse_y;

			if (keysDown() & KEY_LEFT) {
				new_mouse_x--;
			} else if (keysDown() & KEY_RIGHT) {
				new_mouse_x++;
			} else if (keysDown() & KEY_DOWN) {
				new_mouse_y++;
			} else if (keysDown() & KEY_UP) {
				new_mouse_y--;
			}

			TileNum new_tile = map.get_tile(new_mouse_x, new_mouse_y);

			switch (new_tile) {
				case TILE_CHEESE:
					score += 100;
					move_mouse(new_mouse_x, new_mouse_y);
					break;

				case TILE_EMPTY:
					move_mouse(new_mouse_x, new_mouse_y);
					break;

				case TILE_STATIONARY_BLOCK:
					// Can't move here
					break;

				case TILE_MOVABLE_BLOCK:
					push_block(new_mouse_x, new_mouse_y);
					break;

				case TILE_SINK_HOLE:
					// TODO: Use clock ticks to keep mouse in sinkhole for
					// a certain amount of time.

					map.set_tile(mouse_x, mouse_y, TILE_EMPTY);
					map.set_tile(new_mouse_x, new_mouse_y, TILE_MOUSE_SINKHOLE);

					map.draw(0, 0);

					mouse_x = new_mouse_x;
					mouse_y = new_mouse_y;
					state = STATE_SINKHOLE;
					break;

				case TILE_CAT:
				case TILE_YARN:
				case TILE_MOUSE_TRAP:
					// TODO: Kill the mouse here.
					lives--;
					break;

				default:
					// This shouldn't happen.
					break;
			}		
		}

		swiWaitForVBlank();
	}
}

// This method handles the pushing of a block or row of blocks. Note: when you push
// a row of blocks, it is like the first block in the row is getting moved to the end
// of the row and the mouse is taking the first block's place, and the rest of the
// blocks stay in place.
void Game::push_block(u8 x, u8 y) {
	s16 delta_x = x - mouse_x;
	s16 delta_y = y - mouse_y;

	u8 current_x = x;
	u8 current_y = y;

	while (map.get_tile(current_x, current_y) == TILE_MOVABLE_BLOCK) {
		current_x += delta_x;
		current_y += delta_y;
	}

	TileNum end_tile = map.get_tile(current_x, current_y);

	switch (end_tile) {
		case TILE_EMPTY:
		case TILE_CHEESE:
			map.set_tile(current_x, current_y, TILE_MOVABLE_BLOCK);
			move_mouse(x, y);
			break;

		case TILE_SINK_HOLE:
			move_mouse(x, y);
			break;

		default:
			// Any other tiles will not budge.
			break;
	}
}

void Game::move_mouse(u8 x, u8 y) {
	map.set_tile(mouse_x, mouse_y, TILE_EMPTY);
	map.set_tile(x, y, TILE_MOUSE);

	map.draw(0, 0);

	mouse_x = x;
	mouse_y = y;
}

