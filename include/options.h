#ifndef _options_h
#define _options_h

#include "level.h"

enum GameSpeed {
	SPEED_SNAIL = 60,
	SPEED_SLOW = 30,
	SPEED_MEDIUM = 15,
	SPEED_FAST = 8,
	SPEED_BLAZING = 5
};

const LevelId DEFAULT_START_LEVEL = 1;
const GameSpeed DEFAULT_SPEED = SPEED_SLOW;

const char DATA_DIRECTORY[] = "/data/rodents-revenge/";

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

		// Methods
		const char *full_path(const char *relative_path) const;

	private:
		LevelId start_level;
		GameSpeed speed;
};

extern Options options;

#endif

