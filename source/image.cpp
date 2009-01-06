#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include "image.h"

Image::Image(Canvas *the_canvas, const char *filename) {
	u8 read_height, read_width;

	canvas = the_canvas;
	
	FILE *img = fopen(filename, "r");
	
	if (img) {			
		// Read width and height
		fread(&read_width, sizeof(u8), 1, img);
		fread(&read_height, sizeof(u8), 1, img);
		
		width = read_width;
		height = read_height;
		
		// Assume that if it's 0 pixels wide, it's supposed to be 256 pixels wide.
		if (width == 0) { width = 256; }
		
		data = new Color [width * height];
		
		// Read image data
		fread(data, sizeof(Color), width * height, img);
		
		fclose(img);
	} else {
		width = 0;
		height = 0;
		data = NULL;
	}
}

Image::~Image() {
	if (data != NULL) {
		delete [] data;
		data = NULL;
	}
}

void Image::draw(u16 x, u16 y) const {
	u16 put_x, put_y, get_x, get_y, end_x, end_y;
	
	end_x = x + width;
	end_y = y + height;
	
	get_y = 0;
	for (put_y = y; put_y < end_y; put_y++) {
		get_x = 0;
		for (put_x = x; put_x < end_x; put_x++) {
			canvas->plot(put_x, put_y, data[get_x + get_y * width]);
			get_x++;
		}
		get_y++;
	}
}

