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

	// This tile is used to display the number of lives
	big_mouse_tile.load_from_file("/data/rodents-revenge/tiles/mouse_big.tile");

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
}

Game::~Game() {
}

u32 Game::begin() {
	lives = NUM_LIVES;
	level = options.get_start_level();
	score = 0;

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
	num_cats = 0;
	num_sitting_cats = 0;

	state = STATE_NORMAL;

	screen_top.clear(RGB(23, 23, 23));
	screen_bottom.clear(RGB(23, 23, 23));

	Clock clock(&screen_bottom, (SCREEN_WIDTH - CLOCK_WIDTH) / 2, 10);
	clock.draw();

	spawn_cats();

	update_lives();
	update_score();

	keysSetRepeat(15, 3);

	done_level = false;

	while (!done_level) {
		scanKeys();
		u32 keys_down = keysDownRepeat();

		if (clock.get_second_tick()) {
			move_cats();
		}

		if (clock.get_reached_blue_line()) {
			if (state == STATE_FAST_FORWARD) {
				state = STATE_NORMAL;
				clock.set_speed(options.get_speed());
			}

			if (clock.get_state() == CLOCK_STOPPED) {
				done_level = true;
			} else {
				spawn_cats();
			}
		}

		if (clock.get_state() == CLOCK_STOPPED) {
			if (num_cats == 0) {
				done_level = true;
			}
		}

		switch (state) {
			case STATE_FAST_FORWARD:
				// Intentional fall-through
				clock.set_speed(1);
			case STATE_NORMAL:
				handle_input(keys_down, clock.get_tick());
				break;

			case STATE_DYING:
				die();
				break;

			case STATE_SINKHOLE:
				wait_in_sinkhole(clock.get_tick());
				break;
		}

		map.draw(36, 4);

		update_score();

		clock.update();

		swiWaitForVBlank();
	}
}

void Game::wait_in_sinkhole(u32 current_time) {
	s32 length_of_time_in_hole = current_time - time_stuck_in_sinkhole;

	if (length_of_time_in_hole > options.get_speed() * 5) {
		state = STATE_NORMAL;
		map.set_tile(mouse_x, mouse_y, TILE_MOUSE);
	}
}

void Game::move_cats() {
	for (u8 i = 0; i < MAX_CATS; i++) {
		if (cats_x[i] || cats_y[i]) {
			move_cat(i);
		}
	}
}

void Game::die() {
	u8 random_x;
	u8 random_y;

	lives--;
	if (lives == 0) {
		done_level = true;
	}

	state = STATE_NORMAL;

	// TODO: Play animation of mouse dying here.

	// Randomly respawn mouse to new location
	random_empty_tile(&random_x, &random_y);

	mouse_x = random_x;
	mouse_y = random_y;

	map.set_tile(mouse_x, mouse_y, TILE_MOUSE);

	update_lives();
}

void Game::handle_input(u32 input, u32 current_time) {
	if (input & (KEY_LEFT | KEY_RIGHT | KEY_UP | KEY_DOWN)) {
		u8 new_mouse_x = mouse_x;
		u8 new_mouse_y = mouse_y;

		if (input & KEY_LEFT) {
			new_mouse_x--;
		} else if (input & KEY_RIGHT) {
			new_mouse_x++;
		} else if (input & KEY_DOWN) {
			new_mouse_y++;
		} else if (input & KEY_UP) {
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

			case TILE_MOVABLE_BLOCK:
				push_block(new_mouse_x, new_mouse_y);
				break;

			case TILE_SINK_HOLE:
				map.set_tile(mouse_x, mouse_y, TILE_EMPTY);
				map.set_tile(new_mouse_x, new_mouse_y, TILE_MOUSE_SINKHOLE);

				mouse_x = new_mouse_x;
				mouse_y = new_mouse_y;

				time_stuck_in_sinkhole = current_time;
				state = STATE_SINKHOLE;
				break;

			case TILE_MOUSE_TRAP:
				move_mouse(new_mouse_x, new_mouse_y);
				map.set_tile(new_mouse_x, new_mouse_y, TILE_MOUSE_TRAP);
				state = STATE_DYING;
				break;

			default:
				// All other tiles cannot be moved upon.
				break;
		}		
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

	if (rand() <= RAND_MAX / 3) {
		spawn_single_cat();
	}

	if (rand() <= RAND_MAX / 3) {
		spawn_single_cat();
	}
}

void Game::spawn_single_cat() {
	u8 random_x;
	u8 random_y;

	random_empty_tile(&random_x, &random_y);

	map.set_tile(random_x, random_y, TILE_CAT);

	// Record cat's position in cats_x[] and cats_y[]
	for (u8 i = 0; i < MAX_CATS; i++) {
		if (!cats_x[i] && !cats_y[i]) {
			cats_x[i] = random_x;
			cats_y[i] = random_y;
			break;
		}
	}

	num_cats++;
}

void Game::random_empty_tile(u8 *x, u8 *y) {
	// Find all possible empty tiles
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

	*x = empty_tiles_x[random_tile];
	*y = empty_tiles_y[random_tile];
}

void Game::move_cat(u8 cat_num) {
	u8 x = cats_x[cat_num];
	u8 y = cats_y[cat_num];

	bool sitting = (map.get_tile(x, y) == TILE_CAT_SITTING);

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
			if (!sitting) {
				num_sitting_cats++;
			}

			// If all cats are sitting, turn them all into cheese
			if (num_cats == num_sitting_cats) {
				destroy_cats();
			} else {
				map.set_tile(x, y, TILE_CAT_SITTING);
			}

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

					if (sitting) {
						num_sitting_cats--;
					}

					done = true;
					break;

				default:
					distance[min_direction] = -1;
			}
		}
	}
}

void Game::destroy_cats() {
	for (u8 i = 0; i < MAX_CATS; i++) {
		if (cats_x[i] || cats_y[i]) {
			map.set_tile(cats_x[i], cats_y[i], TILE_CHEESE);
			cats_x[i] = 0;
			cats_y[i] = 0;
		}
	}

	num_cats = 0;
	num_sitting_cats = 0;

	state = STATE_FAST_FORWARD;
}

void Game::update_score() {
	char *score_str = strval(score);

	screen_bottom.rect(SCORE_X, SCORE_Y, SCORE_X + 50, SCORE_Y + font->get_font_height(), BACKGROUND_COLOR, RECT_FILLED);
	font->print_string(score_str, SCORE_X, SCORE_Y, &screen_bottom, RGB(0, 0, 0));
}

void Game::update_lives() {
	screen_bottom.rect(LIVES_X, LIVES_Y, LIVES_X + ((big_mouse_tile.get_width() + 3) * NUM_LIVES), LIVES_Y + big_mouse_tile.get_height(), BACKGROUND_COLOR, RECT_FILLED);

	for (u8 i = 0; i < lives; i++) {
		big_mouse_tile.draw(&screen_bottom, LIVES_X + (i * (big_mouse_tile.get_width() + 3)), LIVES_Y);
	}
}
