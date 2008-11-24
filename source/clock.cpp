#include <nds.h>
#include "color.h"
#include "canvas.h"
#include "options.h"
#include "clock.h"

const Color clock_colors[] = {
	RGB(23, 23, 23), // 0: light grey
	RGB(0, 0, 0),    // 1: black
	RGB(31, 31, 31), // 2: white
	RGB(15, 15, 15), // 3: grey
	RGB(31, 0, 0)    // 4: red
};

const Color clock_data[CLOCK_WIDTH * CLOCK_HEIGHT] = {
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,1,3,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,2,1,0,1,3,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,0,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,
	0,1,2,3,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,2,0,1,0,
	1,2,0,3,1,0,1,1,3,3,2,2,2,2,4,2,2,2,2,3,3,1,1,0,1,0,0,3,1,
	1,0,3,1,3,1,3,3,0,2,2,2,2,2,2,2,2,2,2,2,3,3,3,1,0,1,3,3,1,
	0,1,1,3,1,3,0,2,2,4,2,2,2,2,2,2,2,2,2,4,2,2,3,3,1,3,1,1,0,
	0,0,0,1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,0,0,0,
	0,0,1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,0,0,
	0,0,1,3,0,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,0,0,
	0,1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,3,3,1,0,
	0,1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,0,
	1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,
	1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,
	1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,
	1,3,0,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,3,1,
	1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,
	1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,
	1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,
	1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,
	0,1,3,0,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,3,1,0,
	0,1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,0,
	0,0,1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,0,0,
	0,0,1,3,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,0,0,
	0,0,0,1,3,0,2,2,4,2,2,2,2,2,2,2,2,2,2,2,4,2,2,3,3,1,0,0,0,
	0,0,0,0,1,3,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,0,0,0,0,
	0,0,0,0,0,1,3,3,0,0,2,2,2,2,4,2,2,2,2,2,2,3,3,1,0,0,0,0,0,
	0,0,0,0,0,0,1,1,3,3,0,0,0,0,0,0,0,0,0,3,3,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,1,1,3,3,3,3,3,3,3,3,3,1,1,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0
};

Clock::Clock(Canvas *the_canvas, u16 the_x, u16 the_y) {
	canvas = the_canvas;
	x = the_x;
	y = the_y;

	second = 0;
	minute = 0;
	blue_line = 0;
	tick = 0;
	state = CLOCK_STARTED;

	update_blue_line();
}

Clock::~Clock() {
}

void Clock::update() {
	tick++;

	if (tick % options.get_speed()) {
		second++;

		if (second == 30) {
			second = 0;

			if (state == CLOCK_STARTED) {
				minute++;

				if (minute == 30) {
					state = CLOCK_STOPPED;
				}

				if (minute == blue_line) {
					// TODO: spawn cats here somehow
					update_blue_line();
				}
			}
		}

		draw();
	}
}

void Clock::draw() {
	for (u8 put_x = 0; put_x < CLOCK_WIDTH; put_x++) {
		for (u8 put_y = 0; put_y < CLOCK_HEIGHT; put_y++) {
			canvas->plot(x + put_x, y + put_y, clock_data[put_x + put_y * CLOCK_WIDTH]);
		}
	}
}

void Clock::update_blue_line() {
	blue_line = (blue_line + 5) % 30;
}

