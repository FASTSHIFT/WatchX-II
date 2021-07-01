#include "BSP.h"

#if (WX_USE_BUILT_IN_I2C == 0)

typedef struct{
    int16_t BeatsPerMinute;
    int16_t BeatAvg;
}DataPack_TypeDef;

static DataPack_TypeDef DataPack;

ComMaster_MakeNode(ParticleSensor, DataPack);

void ParticleSensor_Init()
{
    ComMaster_InitNode(ParticleSensor, 0x03);
}

void ParticleSensor_Update()
{
    
}

float ParticleSensor_GetBeats()
{
    return DataPack.BeatsPerMinute / 10.0f + 60;
}

#endif
