#include <nds.h>
#include "canvas.h"

Canvas screen_top(SCREEN_WIDTH, SCREEN_HEIGHT, (Color *)BG_BMP_RAM(0));
Canvas screen_bottom(SCREEN_WIDTH, SCREEN_HEIGHT, (Color *)BG_BMP_RAM_SUB(0));

Canvas::Canvas(u32 the_width, u32 the_height) {
	width = the_width;
	height = the_height;
	data = new Color [width * height];
	memory_allocated = true;
}

Canvas::Canvas(u32 the_width, u32 the_height, Color *the_data) {
	width = the_width;
	height = the_height;
	data = the_data;
	memory_allocated = false;
}

Canvas::~Canvas() {
	if (memory_allocated) {
		delete [] data;
		data = 0;
	}
}

void Canvas::plot(u32 x, u32 y, Color color) {
	if (x < width && y < height) {
		data[x + y * width] = color;
	}
}

void Canvas::rect(u32 upper_left_x, u32 upper_left_y, u32 lower_right_x, u32 lower_right_y, Color color, bool filled) {
	if (filled) {
		// Draw a filled-in rectangle.
		for (u32 put_x = upper_left_x; put_x < lower_right_x; put_x++) {
			for (u32 put_y = upper_left_y; put_y < lower_right_y; put_y++) {
				plot(put_x, put_y, color);
			}
		}
	} else {
		// Only draw an outline of the rectangle.
		for (u32 put_x = upper_left_x; put_x < lower_right_x; put_x++) {
			plot(put_x, upper_left_y, color);
			plot(put_x, lower_right_y, color);
		}

		for (u32 put_y = upper_left_y; put_y < lower_right_y; put_y++) {
			plot(upper_left_x, put_y, color);
			plot(lower_right_x, put_y, color);
		}
	}
}

void Canvas::copy(Canvas *destination, u32 x, u32 y) {
	u32 get_x, get_y;

	get_x = 0;
	for (u32 put_x = x; put_x < x + width; put_x++) {
		get_y = 0;
		for (u32 put_y = y; put_y < y + height; put_y++) {
			destination->plot(put_x, put_y, data[get_x + get_y * width]);
			get_y++;
		}
		get_x++;
	}
}

void Canvas::clear(Color color) {
	for (u32 i = 0; i < width * height; i++) {
		data[i] = color;
	}
}

