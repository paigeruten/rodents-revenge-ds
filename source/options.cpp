#include <nds.h>
#include "options.h"

Options options(DEFAULT_START_LEVEL, DEFAULT_SPEED);

Options::Options(LevelId the_start_level, GameSpeed the_speed) {
	start_level = the_start_level;
	speed = the_speed;
}

Options::~Options() {
}

