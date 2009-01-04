#include <nds.h>
#include <math.h>
#include <stdlib.h>
#include "clock.h"

const float PI = 3.14159;

const Color clock_colors[] = {
	TRANSPARENT,     // 0: transparent
	RGB(0, 0, 0),    // 1: black
	RGB(31, 31, 31), // 2: white
	RGB(15, 15, 15), // 3: grey
	RGB(31, 0, 0),   // 4: red
	RGB(23, 23, 23)  // 5: light grey
};

const Color clock_data[CLOCK_WIDTH * CLOCK_HEIGHT] = {
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,2,1,5,1,3,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,2,1,5,1,3,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,5,3,3,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,
	0,1,2,3,1,0,0,0,1,1,5,5,5,5,5,5,5,5,5,1,1,0,0,0,1,2,5,1,0,
	1,2,5,3,1,0,1,1,3,3,2,2,2,2,4,2,2,2,2,3,3,1,1,0,1,5,5,3,1,
	1,5,3,1,3,1,3,3,5,2,2,2,2,2,2,2,2,2,2,2,3,3,3,1,5,1,3,3,1,
	0,1,1,3,1,3,5,2,2,4,2,2,2,2,2,2,2,2,2,4,2,2,3,3,1,3,1,1,0,
	0,0,5,1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,5,0,0,
	0,0,1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,0,0,
	0,0,1,3,5,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,0,0,
	0,1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,3,3,1,0,
	0,1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,0,
	1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,
	1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,
	1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,
	1,3,5,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,3,1,
	1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,
	1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,
	1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,
	1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,
	0,1,3,5,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,3,1,0,
	0,1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,0,
	0,0,1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,1,0,0,
	0,0,1,3,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,0,0,
	0,0,0,1,3,5,2,2,4,2,2,2,2,2,2,2,2,2,2,2,4,2,2,3,3,1,0,0,0,
	0,0,0,0,1,3,5,5,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,1,0,0,0,0,
	0,0,0,0,0,1,3,3,5,5,2,2,2,2,4,2,2,2,2,2,2,3,3,1,0,0,0,0,0,
	0,0,0,0,0,0,1,1,3,3,5,5,5,5,5,5,5,5,5,3,3,1,1,0,0,0,0,0,0,
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
	speed = options.get_speed();

	second_tick = false;
	minute_tick = false;
	reached_blue_line = false;

	update_blue_line();
}

Clock::~Clock() {
}

void Clock::update() {
	tick++;

	second_tick = false;
	minute_tick = false;
	reached_blue_line = false;

	if (tick % speed == 0) {
		second++;
		second_tick = true;

		if (second == 30) {
			second = 0;

			srand(tick);

			if (state == CLOCK_STARTED) {
				minute++;
				minute_tick = true;

				if (minute == 30) {
					state = CLOCK_STOPPED;
				}

				if (minute == blue_line) {
					update_blue_line();
					reached_blue_line = true;
				}
			}
		}

		draw();
	}
}

void Clock::draw() {
	Color pixel;

	for (u8 put_x = 0; put_x < CLOCK_WIDTH; put_x++) {
		for (u8 put_y = 0; put_y < CLOCK_HEIGHT; put_y++) {
			pixel = clock_colors[clock_data[put_x + put_y * CLOCK_WIDTH]];

			if (pixel != TRANSPARENT) {
				canvas->plot(x + put_x, y + put_y, pixel);
			}
		}
	}

	// These are multiplied by 12 because their maximum value is 30, and
	// 30 * 12 is 360, a full circle around the clock.
	float second_angle = degrees2radians(second * 12);
	float minute_angle = degrees2radians(minute * 12);
	float blue_line_angle = degrees2radians(blue_line * 12);

	draw_clock_hand(second_angle, SECOND_HAND_RADIUS, RGB(31, 0, 0));
	draw_clock_hand(minute_angle, MINUTE_HAND_RADIUS, RGB(0, 0, 15));
	draw_clock_hand(blue_line_angle, BLUE_HAND_RADIUS, RGB(0, 0, 31));
}

void Clock::draw_clock_hand(float angle, float radius, Color color) {
	u32 clock_center_x = x + (CLOCK_WIDTH / 2);
	u32 clock_center_y = y + (CLOCK_HEIGHT / 2);

	u16 x1 = clock_center_x;
	u16 y1 = clock_center_y;
	u16 x2 = clock_center_x + sin(angle) * radius;
	u16 y2 = clock_center_y - cos(angle) * radius;

	canvas->line(x1, y1, x2, y2, color);
}

void Clock::update_blue_line() {
	blue_line = (blue_line + 6) % 30;
}

float Clock::degrees2radians(u16 degrees) {
	return degrees * PI / 180.0;
}

