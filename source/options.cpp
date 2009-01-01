#include <nds.h>
#include <string.h>
#include "options.h"

Options options(DEFAULT_START_LEVEL, DEFAULT_SPEED);

Options::Options(LevelId the_start_level, GameSpeed the_speed) {
	start_level = the_start_level;
	speed = the_speed;
}

Options::~Options() {
}

const char *Options::full_path(const char *relative_path) const {
	static char path[512];

	strcpy(path, DATA_DIRECTORY);
	strcat(path, relative_path);

	return path;
}
