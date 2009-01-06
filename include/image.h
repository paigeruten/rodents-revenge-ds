#ifndef _image_h
#define _image_h

#include "canvas.h"

class Image {
	public:
		// Constructors
		Image(Canvas *the_canvas, const char *filename);
		~Image();
		
		// Accessors
		u16 get_width() const { return width; }
		u16 get_height() const { return height; }
		
		// Methods
		void draw(u16 x, u16 y) const;

	private:
		Canvas *canvas;
		u16 width;
		u16 height;
		Color *data;
};

#endif

