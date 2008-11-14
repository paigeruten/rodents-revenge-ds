#ifndef _canvas_h
#define _canvas_h

#include "color.h"

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
		void plot(u32 x, u32 y, Color color);
		void copy(Canvas *destination, u32 x, u32 y);
		void clear(Color color);

	private:
		Color *data;
		u32 width;
		u32 height;
		bool memory_allocated;
};

extern Canvas screen_top;
extern Canvas screen_bottom;

#endif

