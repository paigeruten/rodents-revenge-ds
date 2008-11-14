#include <nds.h>

void init_screens(void) {
	// Main screen turn on
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	BACKGROUND.control[3] = BG_BMP16_256x256 | BG_BMP_BASE(0);
	BACKGROUND.bg3_rotation.xdy = 0;
	BACKGROUND.bg3_rotation.xdx = 1 << 8;
	BACKGROUND.bg3_rotation.ydx = 0;
	BACKGROUND.bg3_rotation.ydy = 1 << 8;
	
	// Sub screen
	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	BACKGROUND_SUB.control[3] = BG_BMP16_256x256 | BG_BMP_BASE(0);
	BACKGROUND_SUB.bg3_rotation.xdy = 0;
	BACKGROUND_SUB.bg3_rotation.xdx = 1 << 8;
	BACKGROUND_SUB.bg3_rotation.ydx = 0;
	BACKGROUND_SUB.bg3_rotation.ydy = 1 << 8;
}

int main(void) {
	irqInit();
	irqEnable(IRQ_VBLANK);

	init_screens();

	while (1) {
		swiWaitForVBlank();
	}

	return 0;
}

