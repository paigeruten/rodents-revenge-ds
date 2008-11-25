#ifndef _game_h
#define _game_h

#include "canvas.h"
#include "font.h"
#include "tile.h"
#include "tilemap.h"

const u8 NUM_TILES = 12;

const u8 LEVEL_WIDTH = 23;
const u8 LEVEL_HEIGHT = 23;

const u8 TILE_WIDTH = 8;
const u8 TILE_HEIGHT = 8;

const u8 NUM_LIVES = 3;

const u8 MAX_CATS = 32;

enum {
	TILE_EMPTY,
	TILE_MOVABLE_BLOCK,
	TILE_STATIONARY_BLOCK,
	TILE_MOUSE,
	TILE_CAT,
	TILE_YARN,
	TILE_MOUSE_TRAP,
	TILE_SINK_HOLE,
	TILE_CHEESE,
	TILE_MOUSE_SINKHOLE,
	TILE_BORDER_OPENING_VERTICAL,
	TILE_BORDER_OPENING_HORIZONTAL
};

enum PlayerState { STATE_NORMAL, STATE_SINKHOLE };

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
		TileMap map;
		Tile tiles[NUM_TILES];
		u8 mouse_x;
		u8 mouse_y;
		u8 lives;
		u8 level;
		u32 score;
		PlayerState state;
		u32 time_stuck_in_sinkhole;
		u8 cats_x[MAX_CATS];
		u8 cats_y[MAX_CATS];

		void load_level();
		void play_level();
		void push_block(u8 x, u8 y);
		void move_mouse(u8 x, u8 y);
		void spawn_cats();
		void spawn_single_cat();
};

#endif

