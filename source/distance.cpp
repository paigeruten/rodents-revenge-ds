#include <nds.h>
#include <math.h>
#include "distance.h"

float distance(u8 x1, u8 y1, u8 x2, u8 y2) {
	s16 dx = x2 - x1;
	s16 dy = y2 - y1;

	return sqrt(dx*dx + dy*dy);
}
