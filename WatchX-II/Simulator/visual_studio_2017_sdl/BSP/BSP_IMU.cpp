#include "BSP/BSP.h"

static uint16_t stepCnt = 0;


void IMU_Init(void)
{
    stepCnt = 0;
}

void IMU_Update()
{
}

int16_t IMU_GetSteps()
{
    return stepCnt++;
}
