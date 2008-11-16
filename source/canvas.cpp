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

void Canvas::rect(u32 upper_left_x, u32 upper_left_y, u32 lower_right_x, u32 lower_right_y, Color color, bool filled) {
	// Don't try to print the box if it's completely out of range of the canvas.
	if (upper_left_x >= width || upper_left_y >= height) {
		return;
	}

	// Make box smaller if the lower-right corner is out of range of the canvas.
	if (lower_right_x >= width) {
		lower_right_x = width - 1;
	}
	if (lower_right_y >= height) {
		lower_right_y = height - 1;
	}

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
	if (x >= destination->get_width() || y >= destination->get_height()) {
		return;
	}

	// Don't write into other areas of memory if this canvas won't fit on the
	// destination canvas.
	u32 right_limit = x + width;
	u32 bottom_limit = y + height;
	if (x + width >= destination->get_width()) {
		right_limit = destination->get_width();
	}
	if (y + height >= destination->get_height()) {
		bottom_limit = destination->get_height();
	}

	u32 get_x = 0;
	for (u32 put_x = x; put_x < right_limit; put_x++) {
		u32 get_y = 0;
		for (u32 put_y = y; put_y < bottom_limit; put_y++) {
			destination->plot(put_x, put_y, get_pixel(get_x, get_y));
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

