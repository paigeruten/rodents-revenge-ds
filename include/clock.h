#ifndef _clock_h
#define _clock_h

#include "canvas.h"
#include "options.h"

enum ClockState { CLOCK_STARTED, CLOCK_STOPPED };

const u8 CLOCK_WIDTH = 29;
const u8 CLOCK_HEIGHT = 32;

const u8 CLOCK_X = (SCREEN_WIDTH - CLOCK_WIDTH) / 2;
const u8 CLOCK_Y = 10;

const float SECOND_HAND_RADIUS = 9.0;
const float MINUTE_HAND_RADIUS = 9.0;
const float BLUE_HAND_RADIUS = 5.0;

class Clock {
	public:
		// Constructors
		Clock(Canvas *the_canvas, u16 the_x, u16 the_y);
		~Clock();

		// Accessors
		u8 get_second() const { return second; }
		u8 get_minute() const { return minute; }
		u8 get_blue_line() const { return blue_line; }
		u32 get_tick() const { return tick; }
		u16 get_x() const { return x; }
		u16 get_y() const { return y; }
		void set_x(u16 new_x) { x = new_x; }
		void set_y(u16 new_y) { y = new_y; }
		ClockState get_state() const { return state; }
		bool get_reached_blue_line() const { return reached_blue_line; }
		bool get_second_tick() const { return second_tick; }
		bool get_minute_tick() const { return minute_tick; }
		u8 get_speed() const { return speed; }
		void set_speed(u8 new_speed) { speed = new_speed; }

		// Methods
		void update();
		void draw();

	private:
		Canvas *canvas;
		u16 x;
		u16 y;
		u8 second;
		u8 minute;
		u8 blue_line;
		u32 tick;
		ClockState state;
		u8 speed;
		bool reached_blue_line;
		bool second_tick;
		bool minute_tick;

		void draw_clock_hand(float angle, float radius, Color color);
		void update_blue_line();
		float degrees2radians(u16 degrees);
};

#endif

