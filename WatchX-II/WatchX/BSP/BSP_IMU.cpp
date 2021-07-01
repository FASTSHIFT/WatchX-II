#include "BSP.h"

#if (WX_USE_BUILT_IN_I2C == 0)

typedef struct{
    uint16_t Steps;
}DataPack_TypeDef;

static DataPack_TypeDef DataPack;

ComMaster_MakeNode(IMU, DataPack);

void IMU_Init()
{
    ComMaster_InitNode(IMU, 0x02);
}

void IMU_Update()
{
}

int16_t IMU_GetSteps()
{
    return DataPack.Steps;
}

#endif
