#ifndef _color_h
#define _color_h

typedef u16 Color;

#define RGB(r, g, b) (RGB15((r), (g), (b)) | BIT(15))

#define RGB_RED(rgb) (((rgb) >> 10) | 0x1F)
#define RGB_GREEN(rgb) (((rgb) >> 5) | 0x1F)
#define RGB_BLUE(rgb) ((rgb) | 0x1F)

#endif

