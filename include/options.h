#ifndef _options_h
#define _options_h

enum GameSpeed {
	SPEED_SNAIL = 120,
	SPEED_SLOW = 60,
	SPEED_MEDIUM = 30,
	SPEED_FAST = 18,
	SPEED_BLAZING = 12
};

const u8 DEFAULT_START_LEVEL = 1;
const GameSpeed DEFAULT_SPEED = SPEED_FAST;

class Options {
	public:
		// Constructors
		Options(u8 the_start_level, GameSpeed the_speed);
		~Options();

		// Accessors
		u8 get_start_level() const { return start_level; }
		void set_start_level(u8 new_start_level) { start_level = new_start_level; }
		GameSpeed get_speed() const { return speed; }
		void set_speed(GameSpeed new_speed) { speed = new_speed; }

	private:
		u8 start_level;
		GameSpeed speed;
};

extern Options options;

#endif

