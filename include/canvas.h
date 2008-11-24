#ifndef _canvas_h
#define _canvas_h

#include "color.h"

const bool RECT_FILLED = true;
const bool RECT_OUTLINE = false;

class Canvas {
	public:
		// Constructors
		Canvas(u32 the_width, u32 the_height);
		Canvas(u32 the_width, u32 the_height, Color *the_data);
		~Canvas();

		// Accessors
		u32 get_width() const { return width; }
		u32 get_height() const { return height; }
		Color *get_data() const { return data; }
		Color get_pixel(u32 x, u32 y) const { return data[x + y * width]; }

		// Methods
		void plot(u32 x, u32 y, Color color) { data[x + y * width] = color; }
		void rect(u32 upper_left_x, u32 upper_left_y, u32 lower_right_x, u32 lower_right_y, Color color, bool filled);
		void copy(Canvas *destination, u32 x, u32 y);
		void copy(Canvas *destination);
		void clear(Color color);
		void line(u32 x1, u32 y1, u32 x2, u32 y2, Color color);

	private:
		Color *data;
		u32 width;
		u32 height;
		bool memory_allocated;
};

extern Canvas screen_top;
extern Canvas screen_bottom;

#endif

