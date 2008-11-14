#ifndef _screen_h
#define _screen_h

#include "color.h"

class Screen {
	public:
		// Constructors
		Screen(u32 the_width, u32 the_height);
		Screen(u32 the_width, u32 the_height, Color *the_data);
		~Screen();

		// Accessors
		u32 get_width() const { return width; }
		u32 get_height() const { return height; }
		Color *get_data() const { return data; }
		Color get_pixel(u32 x, u32 y) const { return data[x + y * width]; }

		// Methods
		void plot(u32 x, u32 y, Color color);
		void copy(Screen *destination, u32 x, u32 y);
		void clear(Color color);

	private:
		Color *data;
		u32 width;
		u32 height;
		bool memory_allocated;
};

void init_screens(void);

extern Screen screen_top;
extern Screen screen_bottom;

#endif

