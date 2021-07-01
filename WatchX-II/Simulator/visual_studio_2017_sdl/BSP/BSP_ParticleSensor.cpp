#include "BSP/BSP.h"
#include <stdlib.h>

static int lastBeats = 800;

void ParticleSensor_Init()
{
    
}

void ParticleSensor_Update()
{
    
}

float ParticleSensor_GetBeats()
{
    int breats = ((uint16_t)rand() % 600 + 600);
    __ValueCloseTo(lastBeats, breats, rand() % 10);
    return lastBeats / 10.0f;
}
