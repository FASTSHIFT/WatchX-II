#include "BSP.h"
#include "FT6336U/FT6336U.h"

static FT6336U touchPad;

typedef struct{
    int16_t X;
    int16_t Y;
    bool IsPressed;
}DataPack_TypeDef;

static DataPack_TypeDef DataPack;

ComSlave_MakeNode(TouchPad, DataPack);

void TouchPad_Init()
{
    ComSlave_InitNode(TouchPad, 0x01);
}

void TouchPad_Update()
{
    DataPack.IsPressed = (touchPad.read_touch1_event() == 0x02);
    if(DataPack.IsPressed)
    {
        DataPack.X = (239 - touchPad.read_touch1_x());
        DataPack.Y = (239 - touchPad.read_touch1_y());
    }
    ComSlave_ReqExecuteUserCallbackNode(&TouchPad_Node);
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
