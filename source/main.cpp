#include <nds.h>
#include "screen.h"

int main(void) {
	irqInit();
	irqEnable(IRQ_VBLANK);

	init_screens();

	while (1) {
		swiWaitForVBlank();
	}

	return 0;
}

