#ifndef _clock_h
#define _clock_h

#include "canvas.h"

enum ClockState { CLOCK_STARTED, CLOCK_STOPPED };

const u8 CLOCK_WIDTH = 29;
const u8 CLOCK_HEIGHT = 32;

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

		void update_blue_line();
};

#endif

