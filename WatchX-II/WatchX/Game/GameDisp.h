#ifndef __GAMEDISP_H
#define __GAMEDISP_H

#include <stdint.h>

#define GAME_DISP_WIDTH  128
#define GAME_DISP_HEIGHT 64

void CanvasGame_DrawBuf(uint8_t* buf, int16_t w, int16_t h);

#endif
