#ifndef __BSP_H
#define __BSP_H

#include "Basic/SysConfig.h"
#include "Basic/CommonMacro.h"
#include <stdint.h>

#ifdef ARDUINO
#   include "Arduino.h"
#else
#   include "BSP/rtc.h"
#endif

/*IMU*/
void IMU_Init();
void IMU_Update();
int16_t IMU_GetSteps();

/*ParticleSensor*/
void ParticleSensor_Init();
void ParticleSensor_Update();

/*I2C*/
void I2C_Scan(bool startScan);

/*TouchPad*/
void TouchPad_Init();
void TouchPad_Update();
bool TouchPad_GetPressed();
void TouchPad_GetPoints(int16_t* x, int16_t* y);

/*ComSlave*/
typedef struct ComSlave_UserCallback
{
    uint8_t ID;
    uint8_t (*Func)(void*, uint8_t);
    const char* Name;
} ComSlave_UserCallback_TypeDef;
void ComSlave_Init();
void ComSlave_Update();
bool ComSlave_ReqExecuteUserCallbackNode(const ComSlave_UserCallback_TypeDef* node);
#define ComSlave_MakeNode(name, dataPack)\
static ComSlave_UserCallback_TypeDef name##_Node;\
static uint8_t name##_NodeCallback(void* buf, uint8_t size)\
{\
    if(size < sizeof(dataPack))\
        return 0;\
    memcpy(buf, &(dataPack), sizeof(dataPack));\
    return sizeof(dataPack);\
}
#define ComSlave_InitNode(name, id)\
do{\
    name##_Node.ID = (id) & 0x0F;\
    name##_Node.Func = name##_NodeCallback;\
    name##_Node.Name = #name;\
}while(0)

#endif
