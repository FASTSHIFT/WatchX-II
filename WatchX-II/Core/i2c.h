
#ifndef __HARDWAREI2C_H__
#define __HARDWAREI2C_H__

#include "stdint.h"
#include "mcu_type.h"


void I2Cx_Init(I2C_Type *I2Cx, uint32_t baudRate);
void I2Cx_Read(I2C_Type *I2Cx, uint8_t slaveAddr, uint8_t regAddr, void *buf, uint32_t length);
void I2Cx_Write(I2C_Type *I2Cx, uint8_t slaveAddr, uint8_t regAddr, void *buf, uint32_t length);
uint8_t I2Cx_ReadReg(I2C_Type *I2Cx, uint8_t slaveAddr, uint8_t regAddr);
void I2Cx_WriteReg(I2C_Type *I2Cx, uint8_t slaveAddr, uint8_t regAddr, uint8_t value);

#endif

