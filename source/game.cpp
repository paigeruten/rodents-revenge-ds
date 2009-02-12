#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"

Game::Game(Canvas *the_canvas, Font *the_font) {
	canvas = the_canvas;
	font = the_font;

	level.init(canvas);

	for (u8 i = 0; i < MAX_CATS; i++) {
		cats_x[i] = 0;
		cats_y[i] = 0;
	}

	// This tile is used to display the number of lives
	big_mouse_tile.load_from_file(options.full_path("tiles/mouse_big.tile"));

	back_button = new Button;
	back_button->init(&screen_bottom, font, "Quit");
	back_button->set_colors(BUTTON_COLORS, BUTTON_PRESSED_COLORS);
	back_button->set_x(32);
	back_button->set_y(SCREEN_HEIGHT - 64);
	back_button->set_width(MENU_BUTTON_WIDTHS);
	back_button->set_height(MENU_BUTTON_HEIGHTS);
}

Game::~Game() {
	delete back_button;
	back_button = NULL;
}

u32 Game::begin() {
	lives = NUM_LIVES;
	level.set_current_level(options.get_start_level());
	score = 0;

	while (lives) {
		level.load();
		play_level();
		level.next();
	}

	return score;
}

void Game::play_level() {
	num_cats = 0;
	num_sitting_cats = 0;

	state = STATE_NORMAL;

	fast_forwarding = false;

	screen_top.clear(BACKGROUND_COLOR);
	screen_bottom.clear(BACKGROUND_COLOR);

	Clock clock(&screen_bottom, CLOCK_X, CLOCK_Y);
	clock.draw();

	back_button->draw();

	mouse_x = LEVEL_WIDTH / 2;
	mouse_y = LEVEL_HEIGHT / 2;
	level.set_tile(mouse_x, mouse_y, TILE_MOUSE);

	spawn_cats();

	update_lives();
	update_score();

	for (u8 i = 0; i < MAX_YARNS; i++) {
		yarns[i].state = YARN_DEAD;
	}

	draw_clock = false;

	done_level = false;
	while (!done_level) {
		scanKeys();
		u32 keys_down = keysDownRepeat();

		if (clock.get_second_tick()) {
			move_cats();

			if (level.get_current_level_properties().yarn) {
				if (rand() / (RAND_MAX / YARN_SPAWN_FREQUENCY) == 0) {
					spawn_yarn();
				}
			}
		}

		if (clock.get_minute_tick()) {
			if (!fast_forwarding) {
				score += level.get_current_level();
			}
		}

		if (clock.get_reached_blue_line()) {
			if (fast_forwarding) {
				fast_forwarding = false;
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
			} else {
				if (score < 3) {
					score = 0;
				} else {
					score -= 3;
				}
			}
		}

		if (fast_forwarding) {
			clock.set_speed(1);
		}

		switch (state) {
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

		if (clock.get_tick() % YARN_SPEED == 0) {
			move_yarns();
		}

		level.draw(LEVEL_X, LEVEL_Y);

		update_score();

		clock.update();

		if (draw_clock) {
			clock.draw();
			draw_clock = false;
		}

		swiWaitForVBlank();
	}
}

void Game::wait_in_sinkhole(u32 current_time) {
	s32 length_of_time_in_hole = current_time - time_stuck_in_sinkhole;

	if (length_of_time_in_hole > options.get_speed() * SINKHOLE_TIME_LIMIT) {
		state = STATE_NORMAL;
		level.set_tile(mouse_x, mouse_y, TILE_MOUSE);
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
	lives--;
	if (lives == 0) {
		done_level = true;
	}

	state = STATE_NORMAL;

	TileNum old_tile = level.get_tile(mouse_x, mouse_y);

	level.set_tile(mouse_x, mouse_y, TILE_MOUSE_DEAD);
	level.draw(LEVEL_X, LEVEL_Y);

	for (u8 timer = 0; timer < 60; timer++) {
		swiWaitForVBlank();
	}

	level.set_tile(mouse_x, mouse_y, old_tile);

	// Randomly respawn mouse to new location
	u8 random_x;
	u8 random_y;
	random_empty_tile(&random_x, &random_y);

	mouse_x = random_x;
	mouse_y = random_y;

	level.set_tile(mouse_x, mouse_y, TILE_MOUSE);

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

		TileNum new_tile = level.get_tile(new_mouse_x, new_mouse_y);

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
				level.set_tile(mouse_x, mouse_y, TILE_EMPTY);
				level.set_tile(new_mouse_x, new_mouse_y, TILE_MOUSE_SINKHOLE);

				mouse_x = new_mouse_x;
				mouse_y = new_mouse_y;

				time_stuck_in_sinkhole = current_time;
				state = STATE_SINKHOLE;
				break;

			case TILE_MOUSE_TRAP:
				move_mouse(new_mouse_x, new_mouse_y);
				level.set_tile(new_mouse_x, new_mouse_y, TILE_EMPTY);
				state = STATE_DYING;
				break;

			default:
				// All other tiles cannot be moved upon.
				break;
		}		
	}

	// Pause
	if (keysHeld() & KEY_START) {
		screen_top.darken();
		screen_bottom.darken();
		screen_top.darken();
		screen_bottom.darken();

		font->print_string_center("-Paused-", 85, &screen_top, RGB(31, 15, 0));
		font->print_string_center("-Paused-", 85, &screen_bottom, RGB(31, 15, 0));

		while (keysHeld() & KEY_START) { scanKeys(); swiWaitForVBlank(); }
		while (!(keysHeld() & KEY_START)) { scanKeys(); swiWaitForVBlank(); }
		while (keysHeld() & KEY_START) { scanKeys(); swiWaitForVBlank(); }

		screen_top.clear(BACKGROUND_COLOR);
		screen_bottom.clear(BACKGROUND_COLOR);

		back_button->draw();
		update_lives();

		draw_clock = true;
	}

	touchPosition stylus;
	touchRead(&stylus);

	if (back_button->update(stylus) == BUTTON_CLICKED) {
		done_level = true;
		lives = 0;
	}

	// Take screenshot (not for ordinary users)
	u32 screenshot_key_combo = (KEY_L | KEY_R | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_SELECT);
	if ((keysHeld() & screenshot_key_combo) == screenshot_key_combo) {
		while (keysHeld()) { scanKeys(); }

		FILE *screenshot = fopen("/rodents_revenge.scr", "w");
		fwrite(screen_top.get_data(), sizeof(Color), SCREEN_WIDTH * SCREEN_HEIGHT, screenshot);
		fwrite(screen_bottom.get_data(), sizeof(Color), SCREEN_WIDTH * SCREEN_HEIGHT, screenshot);
		fclose(screenshot);
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

	while (level.get_tile(current_x, current_y) == TILE_MOVABLE_BLOCK) {
		current_x += delta_x;
		current_y += delta_y;
	}

	TileNum end_tile = level.get_tile(current_x, current_y);

	switch (end_tile) {
		case TILE_EMPTY:
		case TILE_CHEESE:
			level.set_tile(current_x, current_y, TILE_MOVABLE_BLOCK);
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
			if (level.get_tile(current_x, current_y) == TILE_EMPTY) {
				level.set_tile(current_x, current_y, TILE_MOVABLE_BLOCK);
				move_mouse(x, y);
			}

			break;

		default:
			// Any other tiles will not budge.
			break;
	}
}

void Game::move_mouse(u8 x, u8 y) {
	level.set_tile(mouse_x, mouse_y, TILE_EMPTY);
	level.set_tile(x, y, TILE_MOUSE);

	mouse_x = x;
	mouse_y = y;
}

void Game::move_mouse_random() {
	u8 random_x;
	u8 random_y;

	random_empty_tile(&random_x, &random_y);

	level.set_tile(random_x, random_y, TILE_MOUSE);

	mouse_x = random_x;
	mouse_y = random_y;
}

void Game::spawn_cats() {
	LevelProperties current_level_properties = level.get_current_level_properties();

	u8 num_cats_to_spawn = rand() / (RAND_MAX / current_level_properties.max_cats_spawn) + 1;

	for (u8 i = 0; i < num_cats_to_spawn; i++) {
		spawn_single_cat();
	}
}

void Game::spawn_single_cat() {
	u8 random_x;
	u8 random_y;

	random_empty_tile(&random_x, &random_y);

	level.set_tile(random_x, random_y, TILE_CAT);

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

void Game::spawn_yarn() {
	u8 yarn_x;
	u8 yarn_y;

	random_border_tile(&yarn_x, &yarn_y);

	if (yarn_x == 0 || yarn_x == LEVEL_WIDTH - 1) {
		level.set_tile(yarn_x, yarn_y, TILE_BORDER_OPENING_HORIZONTAL);
	} else {
		level.set_tile(yarn_x, yarn_y, TILE_BORDER_OPENING_VERTICAL);
	}

	for (u8 i = 0; i < MAX_YARNS; i++) {
		if (yarns[i].state == YARN_DEAD) {
			yarns[i].x = yarn_x;
			yarns[i].y = yarn_y;
			yarns[i].state = YARN_SPAWNING;
			yarns[i].lifespan = 0;
			yarns[i].last_tile = TILE_STATIONARY_BLOCK;

			break;
		}
	}
}

void Game::random_border_tile(u8 *x, u8 *y) {
	// Find all possible empty tiles
	u8 border_tiles_x[LEVEL_WIDTH * 2 + LEVEL_HEIGHT * 2];
	u8 border_tiles_y[LEVEL_WIDTH * 2 + LEVEL_HEIGHT * 2];
	u8 num_border_tiles = 0;

	for (u8 tile_x = 1; tile_x < LEVEL_WIDTH - 1; tile_x++) {
		for (u8 tile_y = 0; tile_y < LEVEL_HEIGHT; tile_y += LEVEL_HEIGHT - 1) {
			if (level.get_tile(tile_x, tile_y) == TILE_STATIONARY_BLOCK) {
				border_tiles_x[num_border_tiles] = tile_x;
				border_tiles_y[num_border_tiles] = tile_y;
				num_border_tiles++;
			}
		}
	}

	for (u8 tile_y = 1; tile_y < LEVEL_HEIGHT - 1; tile_y++) {
		for (u8 tile_x = 0; tile_x < LEVEL_HEIGHT; tile_x += LEVEL_WIDTH - 1) {
			if (level.get_tile(tile_x, tile_y) == TILE_STATIONARY_BLOCK) {
				border_tiles_x[num_border_tiles] = tile_x;
				border_tiles_y[num_border_tiles] = tile_y;
				num_border_tiles++;
			}
		}
	}

	u16 random_tile = rand() / (RAND_MAX / num_border_tiles);

	*x = border_tiles_x[random_tile];
	*y = border_tiles_y[random_tile];
}

void Game::random_empty_tile(u8 *x, u8 *y) {
	// Find all possible empty tiles
	u8 empty_tiles_x[LEVEL_WIDTH * LEVEL_HEIGHT];
	u8 empty_tiles_y[LEVEL_WIDTH * LEVEL_HEIGHT];
	u8 num_empty_tiles = 0;

	for (u8 tile_x = 0; tile_x < LEVEL_WIDTH; tile_x++) {
		for (u8 tile_y = 0; tile_y < LEVEL_HEIGHT; tile_y++) {
			if (level.get_tile(tile_x, tile_y) == TILE_EMPTY) {
				empty_tiles_x[num_empty_tiles] = tile_x;
				empty_tiles_y[num_empty_tiles] = tile_y;
				num_empty_tiles++;
			}
		}
	}

	u16 random_tile = rand() / (RAND_MAX / num_empty_tiles);

	*x = empty_tiles_x[random_tile];
	*y = empty_tiles_y[random_tile];
}

void Game::move_cat(u8 cat_num) {
	u8 x = cats_x[cat_num];
	u8 y = cats_y[cat_num];

	bool sitting = (level.get_tile(x, y) == TILE_CAT_SITTING);

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
	float distances[NUM_DIRECTIONS];
	for (u8 i = 0; i < NUM_DIRECTIONS; i++) {
		distances[i] = distance(new_x[i], new_y[i], mouse_x, mouse_y);
	}

	bool done = false;
	while (!done) {
		// Find the direction with the minimum distance.
		// 256 is just a number that is higher than any possible distance.
		float min_distance = 256;
		Direction min_direction = DIRECTION_NORTH;
		bool can_move = false;
		for (u8 i = 0; i < NUM_DIRECTIONS; i++) {
			if (distances[i] < min_distance && distances[i] >= 0) {
				min_distance = distances[i];
				min_direction = (Direction)i;
			}

			if (distances[i] >= 0) {
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
				level.set_tile(x, y, TILE_CAT_SITTING);
			}

			done = true;
		} else {
			TileNum the_tile = level.get_tile(new_x[min_direction], new_y[min_direction]);

			switch (the_tile) {
				case TILE_MOUSE:
				case TILE_MOUSE_SINKHOLE:
					state = STATE_DYING;
				case TILE_EMPTY:
					level.set_tile(new_x[min_direction], new_y[min_direction], TILE_CAT);
					level.set_tile(x, y, TILE_EMPTY);

					cats_x[cat_num] = new_x[min_direction];
					cats_y[cat_num] = new_y[min_direction];

					if (sitting) {
						num_sitting_cats--;
					}

					done = true;
					break;

				default:
					distances[min_direction] = -1;
			}
		}
	}
}

void Game::move_yarns() {
	for (u8 i = 0; i < MAX_YARNS; i++) {
		if (yarns[i].state != YARN_DEAD) {
			move_yarn(i);
		}
	}
}

void Game::move_yarn(u8 yarn_num) {
	if (yarns[yarn_num].state == YARN_SITTING) {
		if (rand() / (RAND_MAX / YARN_SPAWN_FREQUENCY) == 0) {
			yarns[yarn_num].state = YARN_MOVING;
			yarns[yarn_num].direction = (Direction)(rand() / (RAND_MAX / NUM_DIRECTIONS));
		}
	} else if (yarns[yarn_num].state == YARN_SPAWNING) {
		if (yarns[yarn_num].lifespan > YARN_SPAWN_TIME) {
			yarns[yarn_num].state = YARN_SITTING;
			level.set_tile(yarns[yarn_num].x, yarns[yarn_num].y, TILE_YARN);
		}
	} else if (yarns[yarn_num].state == YARN_MOVING) {
		s32 new_x = move_x(yarns[yarn_num].x, yarns[yarn_num].direction);
		s32 new_y = move_y(yarns[yarn_num].y, yarns[yarn_num].direction);

		TileNum tile = level.get_tile(new_x, new_y);

		bool off_map = (new_x < 0 || new_y < 0 || new_x >= LEVEL_WIDTH || new_y >= LEVEL_HEIGHT);
		bool blocking_tile = (tile != TILE_EMPTY && tile != TILE_MOUSE && tile != TILE_MOUSE_SINKHOLE);

		if (off_map || blocking_tile) {
			if (yarns[yarn_num].lifespan > YARN_MAX_LIFESPAN) {
				yarns[yarn_num].state = YARN_DEAD;

				level.set_tile(yarns[yarn_num].x, yarns[yarn_num].y, yarns[yarn_num].last_tile);
			} else {
				yarns[yarn_num].state = YARN_SITTING;
			}
		} else {
			level.set_tile(yarns[yarn_num].x, yarns[yarn_num].y, yarns[yarn_num].last_tile);

			yarns[yarn_num].last_tile = tile;

			if (tile == TILE_MOUSE || tile == TILE_MOUSE_SINKHOLE) {
				state = STATE_DYING;
			}

			level.set_tile(new_x, new_y, TILE_YARN);

			yarns[yarn_num].x = new_x;
			yarns[yarn_num].y = new_y;
		}
	}

	yarns[yarn_num].lifespan++;
}

void Game::destroy_cats() {
	for (u8 i = 0; i < MAX_CATS; i++) {
		if (cats_x[i] || cats_y[i]) {
			level.set_tile(cats_x[i], cats_y[i], TILE_CHEESE);
			cats_x[i] = 0;
			cats_y[i] = 0;
		}
	}

	num_cats = 0;
	num_sitting_cats = 0;

	fast_forwarding = true;
}

void Game::update_score() {
	char *score_str = strval(score);

	screen_bottom.rect(SCORE_X, SCORE_Y, SCORE_X + font->string_width("00000"), SCORE_Y + font->get_font_height(), BACKGROUND_COLOR, RECT_FILLED);
	font->print_string(score_str, SCORE_X, SCORE_Y, &screen_bottom, SCORE_COLOR);
}

void Game::update_lives() {
	screen_bottom.rect(LIVES_X, LIVES_Y, LIVES_X + ((big_mouse_tile.get_width() + LIVES_SPACING) * NUM_LIVES), LIVES_Y + big_mouse_tile.get_height(), BACKGROUND_COLOR, RECT_FILLED);

	for (u8 i = 0; i < lives - 1; i++) {
		big_mouse_tile.draw(&screen_bottom, LIVES_X + (i * (big_mouse_tile.get_width() + LIVES_SPACING)), LIVES_Y);
	}
}

s32 move_x(s32 current_x, Direction direction) {
	s32 new_x = current_x;

	switch (direction) {
		case DIRECTION_WEST:
		case DIRECTION_NORTHWEST:
		case DIRECTION_SOUTHWEST:
			new_x--;
			break;

		case DIRECTION_EAST:
		case DIRECTION_NORTHEAST:
		case DIRECTION_SOUTHEAST:
			new_x++;
			break;

		default:
			;
	}

	return new_x;
}

s32 move_y(s32 current_y, Direction direction) {
	s32 new_y = current_y;

	switch (direction) {
		case DIRECTION_NORTH:
		case DIRECTION_NORTHEAST:
		case DIRECTION_NORTHWEST:
			new_y--;
			break;

		case DIRECTION_SOUTH:
		case DIRECTION_SOUTHEAST:
		case DIRECTION_SOUTHWEST:
			new_y++;
			break;

		default:
			;
	}

	return new_y;
}

