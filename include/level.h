#ifndef _level_h
#define _level_h

#include "canvas.h"
#include "tile.h"
#include "tilemap.h"

const u8 LEVEL_WIDTH = 23;
const u8 LEVEL_HEIGHT = 23;

const u8 TILE_WIDTH = 8;
const u8 TILE_HEIGHT = 8;

const u8 MAX_LEVELS = 128;

const u8 LEVEL_X = 36;
const u8 LEVEL_Y = 4;

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
	TILE_BORDER_OPENING_HORIZONTAL,
	TILE_CAT_SITTING,
	TILE_MOUSE_DEAD,

	NUM_TILES
};

enum LevelType {
	LEVEL_SQUARE,
	LEVEL_SCATTERED,
	LEVEL_STATIONARY_CHECKERBOARD,
	LEVEL_MOVABLE_CHECKERBOARD
};

enum LevelSize {
	LEVEL_SMALL,
	LEVEL_MEDIUM,
	LEVEL_LARGE
};

enum LevelTile {
	LEVEL_TILE_EMPTY = 0,
	LEVEL_TILE_MOVABLE = 1,
	LEVEL_TILE_STATIONARY = 2
};

typedef u8 LevelId;
typedef u8 LevelDensity;

struct LevelProperties {
	LevelId id;
	LevelType type;
	LevelSize size;
	u8 max_cats_spawn;
	LevelDensity movable_block_density;
	LevelDensity stationary_block_density;
	LevelDensity trap_density;
	LevelDensity sinkhole_density;
	bool yarn;
};

const LevelProperties DEFAULT_LEVEL_PROPERTIES = {
	0,                    // id
	LEVEL_SCATTERED,      // type
	LEVEL_MEDIUM,         // size
	3,                    // max_cats_spawn
	0, 0, 0, 0,           // x_density
	true                  // yarn
};

class Level {
	public:
		// Constructors
		Level();
		~Level();

		// Accessors
		void set_canvas(Canvas *new_canvas) { canvas = new_canvas; }
		u8 get_num_levels() const { return num_levels; }
		void set_current_level(LevelId new_current_level) { current_level.id = new_current_level; }
		LevelId get_current_level() const { return current_level.id; }
		LevelProperties get_current_level_properties() const { return current_level; }
		LevelProperties get_level(u8 index) { return levels[index]; }
		TileNum get_tile(u8 x, u8 y) const { return map.get_tile(x, y); }
		void set_tile(u8 x, u8 y, TileNum num) { map.set_tile(x, y, num); }

		// Methods
		void init(Canvas *the_canvas);
		void load();
		bool is_last();
		void next();
		void draw(u16 x, u16 y) const { map.draw(x, y); }

	private:
		Canvas *canvas;
		TileMap map;
		Tile tiles[NUM_TILES];
		LevelProperties levels[MAX_LEVELS];
		LevelProperties current_level;
		u8 num_levels;

		void load_levels_from_file(const char *filename);
		void load_map(const u8 data[LEVEL_HEIGHT][LEVEL_WIDTH]);
		void randomly_disperse_tile(LevelDensity density, TileNum tile);
};

#endif

