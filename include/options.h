#ifndef _options_h
#define _options_h

#include "level.h"

enum GameSpeed {
	SPEED_SNAIL = 120,
	SPEED_SLOW = 60,
	SPEED_MEDIUM = 30,
	SPEED_FAST = 18,
	SPEED_BLAZING = 12
};

const LevelId DEFAULT_START_LEVEL = 1;
const GameSpeed DEFAULT_SPEED = SPEED_FAST;

class Options {
	public:
		// Constructors
		Options(LevelId the_start_level, GameSpeed the_speed);
		~Options();

		// Accessors
		LevelId get_start_level() const { return start_level; }
		void set_start_level(LevelId new_start_level) { start_level = new_start_level; }
		GameSpeed get_speed() const { return speed; }
		void set_speed(GameSpeed new_speed) { speed = new_speed; }

	private:
		LevelId start_level;
		GameSpeed speed;
};

extern Options options;

#endif

