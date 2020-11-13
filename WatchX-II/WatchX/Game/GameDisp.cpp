#include "GamePrivate.h"
#include "GameDisp.h"

static volatile bool GameDispUpdating = false;

static const uint8_t HEIGHT = GAME_DISP_HEIGHT, WIDTH = GAME_DISP_WIDTH;

static uint8_t GameDispBuff[HEIGHT*WIDTH/8];
static bool GameDispClear = false;

bool Game_DispIsBusy()
{
    return GameDispUpdating;
}

bool Game_DispReqUpdate(bool clear)
{
    if(GameDispUpdating)
    {
        return false;
    }
    
    GameDispClear = clear;
    GameDispUpdating = true;
    return true;
}

void Game_DispTaskUpdate()
{
    if(!GameDispUpdating)
        return;

    CanvasGame_DrawBuf(GameDispBuff, WIDTH, HEIGHT);
    
    if(GameDispClear)
    {
        memset(GameDispBuff, 0, sizeof(GameDispBuff));
    }

    GameDispUpdating = false;
}

uint8_t* Game_DispGetBuffer()
{
    return GameDispBuff;
}
