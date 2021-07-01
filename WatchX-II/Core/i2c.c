#include "Arduino.h"
#include "i2c.h"

void I2Cx_Init(I2C_Type *I2Cx, uint32_t baudRate)
{
    if(I2Cx == I2C1)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_I2C1, ENABLE);
        pinMode(PB6, OUTPUT_AF_OD);
        pinMode(PB7, OUTPUT_AF_OD);
    }
#if defined(I2C2) && defined(RCC_APB1PERIPH_I2C2)
    else if(I2Cx == I2C2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_I2C2, ENABLE);
        pinMode(PB10, OUTPUT_AF_OD);
        pinMode(PB11, OUTPUT_AF_OD);
    }
#endif
#if defined(I2C3) && defined(RCC_APB1PERIPH_I2C3)  
    else if(I2Cx == I2C3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_I2C3, ENABLE);
        pinMode(PA8, OUTPUT_AF_OD);
        pinMode(PC9, OUTPUT_AF_OD);
    }
#endif
    else
    {
        return;
    }
    
    I2C_InitType i2cInit;
    I2C_DeInit(I2Cx);
    I2C_StructInit(&i2cInit);
    
    i2cInit.I2C_Ack = I2C_Ack_Enable;
    i2cInit.I2C_AddrMode = I2C_AddrMode_7bit;
    i2cInit.I2C_BitRate = baudRate;
    i2cInit.I2C_FmDutyCycle = I2C_FmDutyCycle_2_1;
    i2cInit.I2C_Mode = I2C_Mode_I2CDevice;
    i2cInit.I2C_OwnAddr1 = 0;
    
    I2C_Init(I2Cx, &i2cInit);
    I2C_AcknowledgeConfig(I2Cx, ENABLE); 
    I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);
    
    I2C_Cmd(I2Cx, ENABLE);
}

void I2Cx_ClearADDRFlag(I2C_Type* I2Cx)
{
  __IO uint32_t tmpreg;  
  
  tmpreg = I2Cx->STS1; 
  
  tmpreg = I2Cx->STS2; 
}

void I2Cx_Read(I2C_Type *I2Cx, uint8_t slaveAddr, uint8_t regAddr, void *buf, uint32_t length)
{
    uint8_t *dat = (uint8_t *)buf;
    
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    
    I2C_GenerateSTART(I2Cx, ENABLE);
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_STARTF));
    while(I2Cx->CTRL1 & I2C_CTRL1_STARTGEN);
    
    I2C_Send7bitAddress(I2Cx, (slaveAddr << 1), I2C_Direction_Transmit);
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDRF));
    I2Cx_ClearADDRFlag(I2Cx);
    
    I2C_SendData(I2Cx, regAddr);
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TDE));
    I2C_ClearFlag(I2Cx, I2C_FLAG_TDE);
    
    I2C_GenerateSTART(I2Cx, ENABLE);
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_STARTF));
    
    I2C_Send7bitAddress(I2Cx, (slaveAddr << 1), I2C_Direction_Receive);
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDRF));
    I2Cx_ClearADDRFlag(I2Cx);
    
    while(length --)
    {
        if(!length)
        {
            I2C_AcknowledgeConfig(I2Cx, DISABLE);
            I2C_GenerateSTOP(I2Cx, ENABLE);
        }
        while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RDNE));
        *dat++ = I2C_ReceiveData(I2Cx); 
    }
    while(I2Cx->CTRL1 & I2C_CTRL1_STOPGEN);
}

void I2Cx_Write(I2C_Type *I2Cx, uint8_t slaveAddr, uint8_t regAddr, void *buf, uint32_t length)
{
    uint8_t *dat = (uint8_t *)buf;
    
    I2C_GenerateSTART(I2Cx, ENABLE);
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_STARTF));
    
    I2C_Send7bitAddress(I2Cx, (slaveAddr << 1), I2C_Direction_Transmit);
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDRF));
    I2Cx_ClearADDRFlag(I2Cx);
    
    I2C_SendData(I2Cx, regAddr);
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TDE));
    I2C_ClearFlag(I2Cx, I2C_FLAG_TDE);
    
    while(length --)
    {
        I2C_SendData(I2Cx, *dat++);
        while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TDE));
        I2C_ClearFlag(I2Cx, I2C_FLAG_TDE);
    }
    
    I2C_GenerateSTOP(I2Cx, ENABLE);
    
    while(I2Cx->CTRL1 & I2C_CTRL1_STOPGEN);
//    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TDE));
//    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTFF));
}

uint8_t I2Cx_ReadReg(I2C_Type *I2Cx, uint8_t slaveAddr, uint8_t regAddr)
{
    uint8_t retval = 0;
    I2Cx_Read(I2Cx, slaveAddr, regAddr, &retval, sizeof(uint8_t));
    return retval;
}

void I2Cx_WriteReg(I2C_Type *I2Cx, uint8_t slaveAddr, uint8_t regAddr, uint8_t value)
{
    I2Cx_Write(I2Cx, slaveAddr, regAddr, &value, sizeof(uint8_t));
}




