#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "canvas.h"
#include "font.h"
#include "options.h"
#include "str.h"
#include "tile.h"
#include "tilemap.h"
#include "clock.h"
#include "game.h"

Game::Game(Canvas *the_canvas, Font *the_font) {
	canvas = the_canvas;
	font = the_font;

	map.init(canvas, LEVEL_WIDTH, LEVEL_HEIGHT, TILE_WIDTH, TILE_HEIGHT);

	for (u8 i = 0; i < MAX_CATS; i++) {
		cats_x[i] = 0;
		cats_y[i] = 0;
	}

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
	Clock clock(&screen_bottom, (SCREEN_WIDTH - CLOCK_WIDTH) / 2, 10);

	screen_bottom.clear(RGB(23, 23, 23));
	screen_bottom.clear(RGB(23, 23, 23));

	clock.draw();

	keysSetRepeat(20, 5);

	spawn_cats();

	while (lives) {
		scanKeys();
		u32 keys_down = keysDownRepeat();

		if (state == STATE_SINKHOLE) {
			s32 length_of_time_in_hole = clock.get_tick() - time_stuck_in_sinkhole;

			if (length_of_time_in_hole > options.get_speed() * 5) {
				state = STATE_NORMAL;
				map.set_tile(mouse_x, mouse_y, TILE_MOUSE);
			}
		} else {
			if (keys_down & (KEY_LEFT | KEY_RIGHT | KEY_UP | KEY_DOWN)) {
				u8 new_mouse_x = mouse_x;
				u8 new_mouse_y = mouse_y;

				if (keys_down & KEY_LEFT) {
					new_mouse_x--;
				} else if (keys_down & KEY_RIGHT) {
					new_mouse_x++;
				} else if (keys_down & KEY_DOWN) {
					new_mouse_y++;
				} else if (keys_down & KEY_UP) {
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
						map.set_tile(mouse_x, mouse_y, TILE_EMPTY);
						map.set_tile(new_mouse_x, new_mouse_y, TILE_MOUSE_SINKHOLE);

						mouse_x = new_mouse_x;
						mouse_y = new_mouse_y;

						time_stuck_in_sinkhole = clock.get_tick();
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
		}

		if (clock.get_second_tick()) {
			for (u8 i = 0; i < MAX_CATS; i++) {
				if (cats_x[i] || cats_y[i]) {
					move_cat(i);
				}
			}
		}

		if (clock.get_reached_blue_line()) {
			spawn_cats();
		}

		map.draw(36, 4);

		update_score();

		clock.update();

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

		case TILE_CAT:
			for (u8 i = 0; i < MAX_CATS; i++) {
				if (cats_x[i] == current_x && cats_y[i] == current_y) {
					move_cat(i);
				}
			}

			// If the cat moved successfully, move the row of blocks
			if (map.get_tile(current_x, current_y) == TILE_EMPTY) {
				map.set_tile(current_x, current_y, TILE_MOVABLE_BLOCK);
				move_mouse(x, y);
			}

			break;

		default:
			// Any other tiles will not budge.
			break;
	}
}

void Game::move_mouse(u8 x, u8 y) {
	map.set_tile(mouse_x, mouse_y, TILE_EMPTY);
	map.set_tile(x, y, TILE_MOUSE);

	mouse_x = x;
	mouse_y = y;
}

void Game::spawn_cats() {
	spawn_single_cat();

	if (rand() & 1) {
		spawn_single_cat();
	}
}

void Game::spawn_single_cat() {
	// Find all possible positions to place the cat (in other words find all empty tiles)
	u8 empty_tiles_x[512];
	u8 empty_tiles_y[512];
	u8 num_empty_tiles = 0;

	for (u8 tile_x = 0; tile_x < LEVEL_WIDTH; tile_x++) {
		for (u8 tile_y = 0; tile_y < LEVEL_HEIGHT; tile_y++) {
			if (map.get_tile(tile_x, tile_y) == TILE_EMPTY) {
				empty_tiles_x[num_empty_tiles] = tile_x;
				empty_tiles_y[num_empty_tiles] = tile_y;
				num_empty_tiles++;
			}
		}
	}

	u16 random_tile = rand() % num_empty_tiles;

	map.set_tile(empty_tiles_x[random_tile], empty_tiles_y[random_tile], TILE_CAT);

	// Record cat's position in cats_x[] and cats_y[]
	for (u8 i = 0; i < MAX_CATS; i++) {
		if (!cats_x[i] && !cats_y[i]) {
			cats_x[i] = empty_tiles_x[random_tile];
			cats_y[i] = empty_tiles_y[random_tile];
			break;
		}
	}
}

void Game::move_cat(u8 cat_num) {
	u8 x = cats_x[cat_num];
	u8 y = cats_y[cat_num];	

	// Get the cat's possible new positions for each of the 8 directions
	u8 new_x[NUM_DIRECTIONS];
	new_x[DIRECTION_NORTH] = x;
	new_x[DIRECTION_SOUTH] = x;
	new_x[DIRECTION_WEST] = x - 1;
	new_x[DIRECTION_EAST] = x + 1;
	new_x[DIRECTION_NORTHWEST] = x - 1;
	new_x[DIRECTION_NORTHEAST] = x + 1;
	new_x[DIRECTION_SOUTHWEST] = x - 1;
	new_x[DIRECTION_SOUTHEAST] = x + 1;

	u8 new_y[NUM_DIRECTIONS];
	new_y[DIRECTION_NORTH] = y - 1;
	new_y[DIRECTION_SOUTH] = y + 1;
	new_y[DIRECTION_WEST] = y;
	new_y[DIRECTION_EAST] = y;
	new_y[DIRECTION_NORTHWEST] = y - 1;
	new_y[DIRECTION_NORTHEAST] = y - 1;
	new_y[DIRECTION_SOUTHWEST] = y + 1;
	new_y[DIRECTION_SOUTHEAST] = y + 1;

	// For each of the 8 directions, calculate the distance from the mouse to
	// the cat if the cat were to move in that direction.
	s16 distance[NUM_DIRECTIONS];
	for (u8 i = 0; i < NUM_DIRECTIONS; i++) {
		distance[i] = map.distance(new_x[i], new_y[i], mouse_x, mouse_y);
	}

	bool done = false;
	while (!done) {
		// Find the direction with the minimum distance.
		s16 min_distance = 255;
		Direction min_direction = DIRECTION_NORTH;
		bool can_move = false;
		for (u8 i = 0; i < NUM_DIRECTIONS; i++) {
			if (distance[i] < min_distance && distance[i] != -1) {
				min_distance = distance[i];
				min_direction = (Direction)i;
			}

			if (distance[i] != -1) {
				can_move = true;
			}
		}

		if (!can_move) {
			// TODO: make cat curl up or turn into a piece of cheese here.
			// For now just turn it into a piece of cheese.
			map.set_tile(x, y, TILE_CHEESE);
			cats_x[cat_num] = 0;
			cats_y[cat_num] = 0;

			done = true;
		} else {
			TileNum the_tile = map.get_tile(new_x[min_direction], new_y[min_direction]);

			switch (the_tile) {
				case TILE_MOUSE:
					state = STATE_DYING;
				case TILE_EMPTY:
					map.set_tile(new_x[min_direction], new_y[min_direction], TILE_CAT);
					map.set_tile(x, y, TILE_EMPTY);

					cats_x[cat_num] = new_x[min_direction];
					cats_y[cat_num] = new_y[min_direction];

					done = true;
					break;

				default:
					distance[min_direction] = -1;
			}
		}
	}
}

void Game::update_score() {
	char *score_str = strval(score);

	screen_bottom.rect(SCORE_X, SCORE_Y, SCORE_X + 50, SCORE_Y + font->get_font_height(), BACKGROUND_COLOR, RECT_FILLED);
	font->print_string(score_str, SCORE_X, SCORE_Y, &screen_bottom, RGB(0, 0, 0));
}
