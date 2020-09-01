#ifndef __BSP_H
#define __BSP_H

#include "Basic/SysConfig.h"
#include "Arduino.h"

/*Backlight*/
void Backlight_Init();
uint16_t Backlight_GetValue();
void Backlight_SetValue(int16_t val);
void Backlight_SetGradual(uint16_t target, uint16_t time = 500);
void Backlight_UpdateBKP(void);
uint16_t Backlight_GetBKP(void);

void IMU_Init();
void IMU_Update();

void ParticleSensor_Init();
void ParticleSensor_Update();

void I2C_Scan(bool startScan);

void SD_Init();

#endif
