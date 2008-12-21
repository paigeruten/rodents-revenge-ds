#ifndef _game_h
#define _game_h

#include "color.h"
#include "canvas.h"
#include "font.h"
#include "tile.h"
#include "tilemap.h"
#include "level.h"

const u8 NUM_LIVES = 3;

const u8 MAX_CATS = 32;

const u8 SCORE_X = SCREEN_WIDTH - 55;
const u8 SCORE_Y = 15;

const u8 LIVES_X = 10;
const u8 LIVES_Y = 15;

const Color BACKGROUND_COLOR = RGB(23, 23, 23);

const u8 NUM_DIRECTIONS = 8;

enum Direction {
	DIRECTION_NORTH,
	DIRECTION_NORTHEAST,
	DIRECTION_EAST,
	DIRECTION_SOUTHEAST,
	DIRECTION_SOUTH,
	DIRECTION_SOUTHWEST,
	DIRECTION_WEST,
	DIRECTION_NORTHWEST
};

enum PlayerState { STATE_NORMAL, STATE_SINKHOLE, STATE_DYING, STATE_FAST_FORWARD };

class Game {
	public:
		// Constructors
		Game(Canvas *the_canvas, Font *the_font);
		~Game();

		// Methods
		u32 begin();

	private:
		Canvas *canvas;
		Font *font;
		Level level;
		Tile big_mouse_tile;
		u8 mouse_x;
		u8 mouse_y;
		u8 lives;
		u32 score;
		PlayerState state;
		u32 time_stuck_in_sinkhole;
		u8 cats_x[MAX_CATS];
		u8 cats_y[MAX_CATS];
		u8 num_cats;
		u8 num_sitting_cats;
		bool done_level;

		void load_level();
		void play_level();
		void wait_in_sinkhole(u32 current_time);
		void die();
		void handle_input(u32 input, u32 current_time);
		void push_block(u8 x, u8 y);
		void move_mouse(u8 x, u8 y);
		void spawn_cats();
		void spawn_single_cat();
		void random_empty_tile(u8 *x, u8 *y);
		void move_cats();
		void move_cat(u8 cat_num);
		void destroy_cats();
		void update_score();
		void update_lives();
};

float distance(u8 x1, u8 y1, u8 x2, u8 y2);

#endif

