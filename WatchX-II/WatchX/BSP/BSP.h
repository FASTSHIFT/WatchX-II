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

/*Backlight*/
void Backlight_Init();
uint16_t Backlight_GetValue();
void Backlight_SetValue(int16_t val);
void Backlight_SetGradual(uint16_t target, uint16_t time = 500);
void Backlight_UpdateBKP(void);
uint16_t Backlight_GetBKP(void);

/*IMU*/
void IMU_Init();
void IMU_Update();
int16_t IMU_GetSteps();

/*ParticleSensor*/
void ParticleSensor_Init();
void ParticleSensor_Update();
float ParticleSensor_GetBeats();

/*SD*/
void SD_Init();

/*Button*/
void Button_Init();
void Button_Update();

/*Power*/
void Power_Init();
void Power_HandleTimeUpdate();
void Power_SetAutoLowPowerTimeout(uint16_t sec);
uint16_t Power_GetAutoLowPowerTimeout();
void Power_SetAutoLowPowerEnable(bool en);
void Power_Shutdown();
void Power_Update();
uint8_t Power_GetBattUsage();
bool Power_GetBattIsCharging();

/*MotorLRA*/
void MotorLRA_Init();
void MotorLRA_WriteData(int16_t data);
void MotorLRA_Update();
void MotorLRA_SetEnable(bool en);
void MotorLRA_Vibrate(float strength, uint32_t time);
void MotorLRA_ToneSetVolume(uint16_t volume);
uint16_t MotorLRA_ToneGetVolume();
void MotorLRA_NoTone();
void MotorLRA_Tone(float freq);
void MotorLRA_Tone(float freq, uint32_t time);

/*TouchPad*/
void TouchPad_Init();
void TouchPad_Update();
bool TouchPad_GetPressed();
void TouchPad_GetPoints(int16_t* x, int16_t* y);

/*ComMaster*/
typedef struct ComMaster_UserCallback
{
    uint8_t ID;
    bool (*Func)(void*, uint8_t);
    const char* Name;
    struct ComMaster_UserCallback* Next;
} ComMaster_UserCallback_TypeDef;
void ComMaster_Init();
void ComMaster_Update();
void ComMaster_AddUserCallbackNode(ComMaster_UserCallback_TypeDef* node);

#define ComMaster_MakeNode(name,dataPack)\
static ComMaster_UserCallback_TypeDef name##_Node;\
static bool name##_NodeCallback(void* buf, uint8_t size)\
{\
    if(size != sizeof(dataPack))\
        return false;\
    memcpy(&(dataPack), buf, size);\
    return true;\
}
#define ComMaster_InitNode(name,id)\
do{\
    name##_Node.ID = (id) & 0x0F;\
    name##_Node.Func = name##_NodeCallback;\
    name##_Node.Name = #name;\
    name##_Node.Next = NULL;\
    ComMaster_AddUserCallbackNode(&name##_Node);\
}while(0)

#endif
