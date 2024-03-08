#include <ultra64.h>

#include "config.h"
#include "framebuffers.h"

// 0x70800 bytes
RGBA16 gFramebuffers[3][SCREEN_WIDTH * SCREEN_HEIGHT];
RGBA16 gAuxBuffers[1][SCREEN_WIDTH * SCREEN_HEIGHT];
