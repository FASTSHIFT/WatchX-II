#include "BSP/BSP.h"

#if (WX_USE_BUILT_IN_I2C != 0)

#include "FT6336U/FT6336U.h"

static FT6336U ft6336u;

static bool IsPressed = false;
static int16_t TouchPoint_X = 0, TouchPoint_Y = 0;

void TouchPad_Init()
{
    IsPressed = false;
    TouchPoint_X = 0;
    TouchPoint_Y = 0;
}

void TouchPad_Update()
{
    IsPressed = (ft6336u.read_touch1_event() == 0x02);
    if(IsPressed)
    {
        TouchPoint_X = (239 - ft6336u.read_touch1_x());
        TouchPoint_Y = (239 - ft6336u.read_touch1_y());
//        Serial.print(TouchPoint_X);
//        Serial.print(',');
//        Serial.println(TouchPoint_Y);
    }
}

bool TouchPad_GetPressed()
{
    return IsPressed;
}

void TouchPad_GetPoints(int16_t* x, int16_t* y)
{
    (*x) = TouchPoint_X;
    (*y) = TouchPoint_Y;
}

#endif
