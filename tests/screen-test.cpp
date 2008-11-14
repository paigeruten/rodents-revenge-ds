#include <nds.h>
#include "screen.h"

int main(void) {
	irqInit();
	irqEnable(IRQ_VBLANK);

	init_screens();

	screen_top.clear(RGB(0, 0, 0));
	screen_bottom.clear(RGB(0, 0, 0));

	screen_top.plot(5, 5, RGB(31, 15, 0));

	Screen test(5, 5);
	test.clear(RGB(31, 15, 0));
	test.plot(3, 2, RGB(0, 0, 31));
	test.copy(&screen_bottom, 10, 10);

	return 0;
}

