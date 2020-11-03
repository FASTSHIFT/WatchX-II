#include "BSP.h"

#if (WX_USE_BUILT_IN_I2C == 0)

typedef struct{
    int16_t X;
    int16_t Y;
    bool IsPressed;
}DataPack_TypeDef;

static DataPack_TypeDef DataPack;

ComMaster_MakeNode(TouchPad, DataPack);

void TouchPad_Init()
{
    ComMaster_InitNode(TouchPad, 0x01);
}

void TouchPad_Update()
{
}

bool TouchPad_GetPressed()
{
    return DataPack.IsPressed;
}

void TouchPad_GetPoints(int16_t* x, int16_t* y)
{
    *x = DataPack.X;
    *y = DataPack.Y;
}

#endif
