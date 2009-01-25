#ifndef _game_h
#define _game_h

#include "color.h"
#include "canvas.h"
#include "font.h"
#include "tile.h"
#include "tilemap.h"
#include "menu.h"
#include "level.h"
#include "options.h"
#include "clock.h"
#include "distance.h"

const u8 NUM_LIVES = 3;

const u8 MAX_CATS = 32;
const u8 MAX_CATS_TO_SPAWN = 3;

const u8 MAX_YARNS = 32;

const u8 SCORE_X = SCREEN_WIDTH - 55;
const u8 SCORE_Y = 15;
const Color SCORE_COLOR = RGB(0, 0, 0);

const u8 LIVES_X = 10;
const u8 LIVES_Y = 15;
const u8 LIVES_SPACING = 3;

const Color BACKGROUND_COLOR = MENU_BACKGROUND_COLOR;

const u8 SINKHOLE_TIME_LIMIT = 5;

const u8 YARN_SPAWN_FREQUENCY = 8;
const u32 YARN_MAX_LIFESPAN = 1800;
const u32 YARN_SPAWN_TIME = 30;
const u8 YARN_SPEED = 3;

enum Direction {
	DIRECTION_NORTH,
	DIRECTION_NORTHEAST,
	DIRECTION_EAST,
	DIRECTION_SOUTHEAST,
	DIRECTION_SOUTH,
	DIRECTION_SOUTHWEST,
	DIRECTION_WEST,
	DIRECTION_NORTHWEST,

	NUM_DIRECTIONS
};

enum PlayerState {
	STATE_NORMAL,
	STATE_SINKHOLE,
	STATE_DYING
};

enum YarnState {
	YARN_SITTING,
	YARN_SPAWNING,
	YARN_MOVING,
	YARN_DEAD
};

struct Yarn {
	u8 x;
	u8 y;
	YarnState state;
	Direction direction;
	u32 lifespan;
};

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
		Yarn yarns[MAX_YARNS];
		Button *back_button;
		bool fast_forwarding;
		bool done_level;
		bool draw_clock;

		void play_level();
		void wait_in_sinkhole(u32 current_time);
		void die();
		void handle_input(u32 input, u32 current_time);
		void push_block(u8 x, u8 y);
		void move_mouse(u8 x, u8 y);
		void move_mouse_random();
		void spawn_cats();
		void spawn_single_cat();
		void spawn_yarn();
		void random_border_tile(u8 *x, u8 *y);
		void random_empty_tile(u8 *x, u8 *y);
		void move_cats();
		void move_cat(u8 cat_num);
		void move_yarns();
		void move_yarn(u8 yarn_num);
		void destroy_cats();
		void update_score();
		void update_lives();
};

s32 move_x(s32 current_x, Direction direction);
s32 move_y(s32 current_y, Direction direction);

#endif

