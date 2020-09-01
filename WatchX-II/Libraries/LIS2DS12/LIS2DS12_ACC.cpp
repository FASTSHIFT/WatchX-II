/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
* File Name          : LIS2DS12_ACC_driver.c
* Author             : MEMS Application Team
* Version            : v1.6
* Date               : 31 Mar 2017
* Description        : EKSTM32 main file
*
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LIS2DS12_ACC.h"
#include "Basic/SysConfig.h"
#include "Wire.h"
//#include "i2c.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

//static void Wire_WriteReg(uint8_t addr, uint8_t reg, uint8_t *bufp, uint16_t len)
//{
//    for(int i = 0; i < len; i++)
//    {
//        Wire.beginTransmission(addr);
//        Wire.write(reg);
//        Wire.write(bufp[i]);
//        Wire.endTransmission();
//    }
//}

//static void Wire_ReadReg(uint8_t addr, uint8_t reg, uint8_t *bufp, uint16_t len)
//{
//    for(int i = 0; i < len; i++)
//    {
//        Wire.beginTransmission(addr);
//        Wire.write(reg);
//        Wire.endTransmission();

//        Wire.requestFrom(addr, 1);
//        bufp[i] = Wire.read();
//        Wire.endTransmission();
//    }
//}

/************** Generic Function  *******************/

/*******************************************************************************
* Function Name     : LIS2DS12_ACC_WriteReg
* Description       : Generic Writing function. It must be fullfilled with either
*                   : I2C or SPI writing function
* Input             : Register Address, ptr to buffer to be written,
*                                 length of buffer
* Output            : None
* Return            : None
*******************************************************************************/
status_t LIS2DS12_ACC_WriteReg(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len)
{
    //To be completed with either I2c or SPI writing function
    //i.e.   HAL_I2C_Mem_Write(&hi2c1, LIS2DS12_ACC_I2C_ADDRESS_HIGH, Reg, I2C_MEMADD_SIZE_8BIT,
    //             Bufp, len, 1000);
//    Wire_WriteReg(LIS2DS12_ACC_I2C_ADDRESS_HIGH, Reg, Bufp, len);
    I2Cx_Write(SENSOR_I2C, LIS2DS12_ACC_I2C_ADDRESS_HIGH, Reg, Bufp, len);
    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name     : LIS2DS12_ACC_ReadReg
* Description       : Generic Reading function. It must be fullfilled with either
*                   : I2C or SPI writing function
* Input             : Register Address, ptr to buffer to be read,
*                                 length of buffer
* Output            : None
* Return            : None
*******************************************************************************/
status_t LIS2DS12_ACC_ReadReg(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len)
{
    //To be completed with either I2c or SPI writing function
    //i.e. HAL_I2C_Mem_Read(&hi2c1, LIS2DS12_ACC_I2C_ADDRESS_HIGH, Reg, I2C_MEMADD_SIZE_8BIT,
    //     Bufp, len, 1000);
    //Wire_ReadReg(LIS2DS12_ACC_I2C_ADDRESS_HIGH, Reg, Bufp, len);
    I2Cx_Read(SENSOR_I2C, LIS2DS12_ACC_I2C_ADDRESS_HIGH, Reg, Bufp, len);
    return MEMS_SUCCESS;
}

/**************** Base Function  *******************/

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_WHO_AM_I_BIT
* Description    : Read WHO_AM_I_BIT
* Input          : Pointer to uint8_t
* Output         : Status of WHO_AM_I_BIT
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_WHO_AM_I_BIT(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WHO_AM_I_REG, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_WHO_AM_I_BIT_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_WHO_AM_I_BIT_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_ODR
* Description    : Write ODR
* Input          : LIS2DS12_ACC_ODR_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_ODR(void *handle, LIS2DS12_ACC_ODR_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL1, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_ODR_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL1, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_ODR
* Description    : Read ODR
* Input          : Pointer to LIS2DS12_ACC_ODR_t
* Output         : Status of ODR see LIS2DS12_ACC_ODR_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_ODR(void *handle, LIS2DS12_ACC_ODR_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL1, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_ODR_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_FullScale
* Description    : Write FS
* Input          : LIS2DS12_ACC_FS_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_FullScale(void *handle, LIS2DS12_ACC_FS_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL1, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_FS_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL1, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FullScale
* Description    : Read FS
* Input          : Pointer to LIS2DS12_ACC_FS_t
* Output         : Status of FS see LIS2DS12_ACC_FS_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FullScale(void *handle, LIS2DS12_ACC_FS_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL1, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FS_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_BDU
* Description    : Write BDU
* Input          : LIS2DS12_ACC_BDU_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_BDU(void *handle, LIS2DS12_ACC_BDU_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL1, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_BDU_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL1, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_BDU
* Description    : Read BDU
* Input          : Pointer to LIS2DS12_ACC_BDU_t
* Output         : Status of BDU see LIS2DS12_ACC_BDU_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_BDU(void *handle, LIS2DS12_ACC_BDU_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL1, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_BDU_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : status_t LIS2DS12_ACC_Get_Raw_Acceleration(uint8_t *buff)
* Description    : Read Acceleration output register
* Input          : pointer to [uint8_t]
* Output         : Acceleration buffer uint8_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_Get_Raw_Acceleration(void *handle, uint8_t *buff)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_OUT_X_L, buff, 6))
        return MEMS_ERROR;
    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : status_t LIS2DS12_ACC_Get_Acceleration(void *handle, int *buff, uint8_t from_fifo)
* Description    : Read GetAccData output register
* Input          : pointer to [uint8_t]
* Output         : values are expressed in mg
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
/*
 * Following is the table of sensitivity values for each case.
 * Values are espressed in ug/digit.
 */
const long long LIS2DS12_ACC_Sensitivity_List[3][4] =
{
    /* HR 14bit */
    {
        244,  /* FS @2g */
        1952,  /* FS @16g */
        488,  /* FS @4g */
        976,  /* FS @8g */
    },

    /* HF 12bit */
    {
        976,  /* FS @2g */
        7813,  /* FS @16g */
        1952,  /* FS @4g */
        3906,  /* FS @8g */
    },

    /* LP 10bit */
    {
        3906,  /* FS @2g */
        31250, /* FS @16g */
        7813,  /* FS @4g */
        15625, /* FS @8g */
    },
};
/*
 * Values returned are espressed in mg.
 */
status_t LIS2DS12_ACC_Get_Acceleration(void *handle, int16_t *buff)
{
    Type3Axis16bit_U raw_data_tmp;
    uint8_t ctrl1_reg, odr, hf, fs;
    uint8_t shift = 0, mode = 0;

    /* Read out current odr, fs, hf setting */
    LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL1, &ctrl1_reg, 1);
    odr = (ctrl1_reg >> 4) & 0xF;
    hf  = (ctrl1_reg >> 1) & 0x1;
    fs  = (ctrl1_reg >> 2) & 0x3;

    /* Determine which of the 3 cases the device is in */
    if (odr >= 8)
    {
        /* LP_10 case */
        shift = 6;
        mode = 2;
    }
    else if (odr > 4 && odr < 8 && hf == 1)
    {
        /* HF_12 case */
        shift = 4;
        mode = 1;
    }
    else
    {
        /* HR_14 case */
        shift = 2;
        mode = 0;
    }

    /* Read out raw accelerometer samples */
    LIS2DS12_ACC_Get_Raw_Acceleration(handle, raw_data_tmp.u8bit);

    /* Apply proper shift and sensitivity */
    buff[0] = ((raw_data_tmp.i16bit[0] >> shift) * LIS2DS12_ACC_Sensitivity_List[mode][fs] + 500) / 1000;
    buff[1] = ((raw_data_tmp.i16bit[1] >> shift) * LIS2DS12_ACC_Sensitivity_List[mode][fs] + 500) / 1000;
    buff[2] = ((raw_data_tmp.i16bit[2] >> shift) * LIS2DS12_ACC_Sensitivity_List[mode][fs] + 500) / 1000;

    return MEMS_SUCCESS;
}

/**************** Advanced Function  *******************/

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_MODULE8_BIT
* Description    : Read MODULE8_BIT
* Input          : Pointer to uint8_t
* Output         : Status of MODULE8_BIT
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_MODULE8_BIT(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_MODULE_8BIT, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_MODULE8_BIT_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_MODULE8_BIT_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_HF_ODR
* Description    : Write HF_ODR
* Input          : LIS2DS12_ACC_HF_ODR_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_HF_ODR(void *handle, LIS2DS12_ACC_HF_ODR_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL1, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_HF_ODR_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL1, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_HF_ODR
* Description    : Read HF_ODR
* Input          : Pointer to LIS2DS12_ACC_HF_ODR_t
* Output         : Status of HF_ODR see LIS2DS12_ACC_HF_ODR_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_HF_ODR(void *handle, LIS2DS12_ACC_HF_ODR_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL1, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_HF_ODR_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_SIM
* Description    : Write SIM
* Input          : LIS2DS12_ACC_SIM_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_SIM(void *handle, LIS2DS12_ACC_SIM_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_SIM_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SIM
* Description    : Read SIM
* Input          : Pointer to LIS2DS12_ACC_SIM_t
* Output         : Status of SIM see LIS2DS12_ACC_SIM_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SIM(void *handle, LIS2DS12_ACC_SIM_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SIM_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_I2C_DISABLE
* Description    : Write I2C_DISABLE
* Input          : LIS2DS12_ACC_I2C_DISABLE_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_I2C_DISABLE(void *handle, LIS2DS12_ACC_I2C_DISABLE_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_I2C_DISABLE_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_I2C_DISABLE
* Description    : Read I2C_DISABLE
* Input          : Pointer to LIS2DS12_ACC_I2C_DISABLE_t
* Output         : Status of I2C_DISABLE see LIS2DS12_ACC_I2C_DISABLE_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_I2C_DISABLE(void *handle, LIS2DS12_ACC_I2C_DISABLE_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_I2C_DISABLE_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_IF_ADD_INC
* Description    : Write IF_ADD_INC
* Input          : LIS2DS12_ACC_IF_ADD_INC_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_IF_ADD_INC(void *handle, LIS2DS12_ACC_IF_ADD_INC_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_IF_ADD_INC_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_IF_ADD_INC
* Description    : Read IF_ADD_INC
* Input          : Pointer to LIS2DS12_ACC_IF_ADD_INC_t
* Output         : Status of IF_ADD_INC see LIS2DS12_ACC_IF_ADD_INC_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_IF_ADD_INC(void *handle, LIS2DS12_ACC_IF_ADD_INC_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_IF_ADD_INC_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_FDS_SLOPE
* Description    : Write FDS_SLOPE
* Input          : LIS2DS12_ACC_FDS_SLOPE_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_FDS_SLOPE(void *handle, LIS2DS12_ACC_FDS_SLOPE_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_FDS_SLOPE_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FDS_SLOPE
* Description    : Read FDS_SLOPE
* Input          : Pointer to LIS2DS12_ACC_FDS_SLOPE_t
* Output         : Status of FDS_SLOPE see LIS2DS12_ACC_FDS_SLOPE_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FDS_SLOPE(void *handle, LIS2DS12_ACC_FDS_SLOPE_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FDS_SLOPE_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FUNC_CFG
* Description    : Read FUNC_CFG_EN
* Input          : Pointer to LIS2DS12_ACC_FUNC_CFG_t
* Output         : Status of IF_ADD_INC see LIS2DS12_ACC_FUNC_CFG_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FUNC_CFG(void *handle, LIS2DS12_ACC_FUNC_CFG_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FUNC_CFG_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_FUNC_CFG
* Description    : Write FUNC_CFG_EN
* Input          : LIS2DS12_ACC_FUNC_CFG_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_FUNC_CFG(void *handle, LIS2DS12_ACC_FUNC_CFG_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_FUNC_CFG_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_SOFT_RESET
* Description    : Write SOFT_RESET
* Input          : LIS2DS12_ACC_SOFT_RESET_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_SOFT_RESET(void *handle, LIS2DS12_ACC_SOFT_RESET_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_SOFT_RESET_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SOFT_RESET
* Description    : Read SOFT_RESET
* Input          : Pointer to LIS2DS12_ACC_SOFT_RESET_t
* Output         : Status of SOFT_RESET see LIS2DS12_ACC_SOFT_RESET_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SOFT_RESET(void *handle, LIS2DS12_ACC_SOFT_RESET_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SOFT_RESET_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_BOOT
* Description    : Write BOOT
* Input          : LIS2DS12_ACC_BOOT_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_BOOT(void *handle, LIS2DS12_ACC_BOOT_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_BOOT_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL2, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_BOOT
* Description    : Read BOOT
* Input          : Pointer to LIS2DS12_ACC_BOOT_t
* Output         : Status of BOOT see LIS2DS12_ACC_BOOT_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_BOOT(void *handle, LIS2DS12_ACC_BOOT_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL2, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_BOOT_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_PP_OD
* Description    : Write PP_OD
* Input          : LIS2DS12_ACC_PP_OD_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_PP_OD(void *handle, LIS2DS12_ACC_PP_OD_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_PP_OD_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_PP_OD
* Description    : Read PP_OD
* Input          : Pointer to LIS2DS12_ACC_PP_OD_t
* Output         : Status of PP_OD see LIS2DS12_ACC_PP_OD_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_PP_OD(void *handle, LIS2DS12_ACC_PP_OD_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_PP_OD_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_H_LACTIVE
* Description    : Write H_LACTIVE
* Input          : LIS2DS12_ACC_H_LACTIVE_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_H_LACTIVE(void *handle, LIS2DS12_ACC_H_LACTIVE_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_H_LACTIVE_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_H_LACTIVE
* Description    : Read H_LACTIVE
* Input          : Pointer to LIS2DS12_ACC_H_LACTIVE_t
* Output         : Status of H_LACTIVE see LIS2DS12_ACC_H_LACTIVE_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_H_LACTIVE(void *handle, LIS2DS12_ACC_H_LACTIVE_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_H_LACTIVE_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_LIR
* Description    : Write LIR
* Input          : LIS2DS12_ACC_LIR_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_LIR(void *handle, LIS2DS12_ACC_LIR_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_LIR_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_LIR
* Description    : Read LIR
* Input          : Pointer to LIS2DS12_ACC_LIR_t
* Output         : Status of LIR see LIS2DS12_ACC_LIR_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_LIR(void *handle, LIS2DS12_ACC_LIR_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_LIR_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_TAP_Z_EN
* Description    : Write TAP_Z_EN
* Input          : LIS2DS12_ACC_TAP_Z_EN_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_TAP_Z_EN(void *handle, LIS2DS12_ACC_TAP_Z_EN_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_TAP_Z_EN_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_TAP_Z_EN
* Description    : Read TAP_Z_EN
* Input          : Pointer to LIS2DS12_ACC_TAP_Z_EN_t
* Output         : Status of TAP_Z_EN see LIS2DS12_ACC_TAP_Z_EN_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_TAP_Z_EN(void *handle, LIS2DS12_ACC_TAP_Z_EN_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_TAP_Z_EN_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_TAP_Y_EN
* Description    : Write TAP_Y_EN
* Input          : LIS2DS12_ACC_TAP_Y_EN_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_TAP_Y_EN(void *handle, LIS2DS12_ACC_TAP_Y_EN_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_TAP_Y_EN_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_TAP_Y_EN
* Description    : Read TAP_Y_EN
* Input          : Pointer to LIS2DS12_ACC_TAP_Y_EN_t
* Output         : Status of TAP_Y_EN see LIS2DS12_ACC_TAP_Y_EN_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_TAP_Y_EN(void *handle, LIS2DS12_ACC_TAP_Y_EN_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_TAP_Y_EN_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_TAP_X_EN
* Description    : Write TAP_X_EN
* Input          : LIS2DS12_ACC_TAP_X_EN_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_TAP_X_EN(void *handle, LIS2DS12_ACC_TAP_X_EN_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_TAP_X_EN_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_TAP_X_EN
* Description    : Read TAP_X_EN
* Input          : Pointer to LIS2DS12_ACC_TAP_X_EN_t
* Output         : Status of TAP_X_EN see LIS2DS12_ACC_TAP_X_EN_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_TAP_X_EN(void *handle, LIS2DS12_ACC_TAP_X_EN_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_TAP_X_EN_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_ST
* Description    : Write ST
* Input          : LIS2DS12_ACC_ST_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_ST(void *handle, LIS2DS12_ACC_ST_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_ST_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL3, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_ST
* Description    : Read ST
* Input          : Pointer to LIS2DS12_ACC_ST_t
* Output         : Status of ST see LIS2DS12_ACC_ST_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_ST(void *handle, LIS2DS12_ACC_ST_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL3, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_ST_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT1_DRDY
* Description    : Write INT1_DRDY
* Input          : LIS2DS12_ACC_INT1_DRDY_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT1_DRDY(void *handle, LIS2DS12_ACC_INT1_DRDY_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT1_DRDY_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT1_DRDY
* Description    : Read INT1_DRDY
* Input          : Pointer to LIS2DS12_ACC_INT1_DRDY_t
* Output         : Status of INT1_DRDY see LIS2DS12_ACC_INT1_DRDY_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT1_DRDY(void *handle, LIS2DS12_ACC_INT1_DRDY_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT1_DRDY_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT1_FTH
* Description    : Write INT1_FTH
* Input          : LIS2DS12_ACC_INT1_FTH_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT1_FTH(void *handle, LIS2DS12_ACC_INT1_FTH_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT1_FTH_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT1_FTH
* Description    : Read INT1_FTH
* Input          : Pointer to LIS2DS12_ACC_INT1_FTH_t
* Output         : Status of INT1_FTH see LIS2DS12_ACC_INT1_FTH_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT1_FTH(void *handle, LIS2DS12_ACC_INT1_FTH_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT1_FTH_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT1_6D
* Description    : Write INT1_6D
* Input          : LIS2DS12_ACC_INT1_6D_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT1_6D(void *handle, LIS2DS12_ACC_INT1_6D_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT1_6D_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT1_6D
* Description    : Read INT1_6D
* Input          : Pointer to LIS2DS12_ACC_INT1_6D_t
* Output         : Status of INT1_6D see LIS2DS12_ACC_INT1_6D_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT1_6D(void *handle, LIS2DS12_ACC_INT1_6D_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, (uint8_t *)value, 1) )
        return MEMS_ERROR;
         
    *(uint8_t*)value &= LIS2DS12_ACC_INT1_6D_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT1_D_TAP
* Description    : Write INT1_TAP
* Input          : LIS2DS12_ACC_INT1_D_TAP_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT1_D_TAP(void *handle, LIS2DS12_ACC_INT1_D_TAP_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT1_D_TAP_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT1_TAP
* Description    : Read INT1_TAP
* Input          : Pointer to LIS2DS12_ACC_INT1_TAP_t
* Output         : Status of INT1_TAP see LIS2DS12_ACC_INT1_TAP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT1_D_TAP(void *handle, LIS2DS12_ACC_INT1_D_TAP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT1_D_TAP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT1_FF
* Description    : Write INT1_FF
* Input          : LIS2DS12_ACC_INT1_FF_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT1_FF(void *handle, LIS2DS12_ACC_INT1_FF_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT1_FF_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT1_FF
* Description    : Read INT1_FF
* Input          : Pointer to LIS2DS12_ACC_INT1_FF_t
* Output         : Status of INT1_FF see LIS2DS12_ACC_INT1_FF_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT1_FF(void *handle, LIS2DS12_ACC_INT1_FF_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT1_FF_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT1_WU
* Description    : Write INT1_WU
* Input          : LIS2DS12_ACC_INT1_WU_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT1_WU(void *handle, LIS2DS12_ACC_INT1_WU_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT1_WU_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT1_WU
* Description    : Read INT1_WU
* Input          : Pointer to LIS2DS12_ACC_INT1_WU_t
* Output         : Status of INT1_WU see LIS2DS12_ACC_INT1_WU_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT1_WU(void *handle, LIS2DS12_ACC_INT1_WU_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT1_WU_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT1_S_TAP
* Description    : Write INT1_S_TAP
* Input          : LIS2DS12_ACC_INT1_S_TAP_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT1_S_TAP(void *handle, LIS2DS12_ACC_INT1_S_TAP_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT1_S_TAP_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT1_S_TAP
* Description    : Read INT1_S_TAP
* Input          : Pointer to LIS2DS12_ACC_INT1_S_TAP_t
* Output         : Status of INT1_S_TAP see LIS2DS12_ACC_INT1_S_TAP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT1_S_TAP(void *handle, LIS2DS12_ACC_INT1_S_TAP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT1_S_TAP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT1_MASTER_DRDY
* Description    : Write INT1_DRDY
* Input          : LIS2DS12_ACC_INT1_DRDY_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT1_MASTER_DRDY(void *handle, LIS2DS12_ACC_INT1_MASTER_DRDY_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT1_DRDY_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL4, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT1_MASTER_DRDY
* Description    : Read INT1_DRDY
* Input          : Pointer to LIS2DS12_ACC_INT1_DRDY_t
* Output         : Status of INT1_DRDY see LIS2DS12_ACC_INT1_DRDY_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT1_MASTER_DRDY(void *handle, LIS2DS12_ACC_INT1_MASTER_DRDY_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL4, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT1_DRDY_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT2_DRDY
* Description    : Write INT2_DRDY
* Input          : LIS2DS12_ACC_INT2_DRDY_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT2_DRDY(void *handle, LIS2DS12_ACC_INT2_DRDY_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT2_DRDY_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT2_DRDY
* Description    : Read INT2_DRDY
* Input          : Pointer to LIS2DS12_ACC_INT2_DRDY_t
* Output         : Status of INT2_DRDY see LIS2DS12_ACC_INT2_DRDY_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT2_DRDY(void *handle, LIS2DS12_ACC_INT2_DRDY_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT2_DRDY_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT2_FTH
* Description    : Write INT2_FTH
* Input          : LIS2DS12_ACC_INT2_FTH_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT2_FTH(void *handle, LIS2DS12_ACC_INT2_FTH_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT2_FTH_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT2_FTH
* Description    : Read INT2_FTH
* Input          : Pointer to LIS2DS12_ACC_INT2_FTH_t
* Output         : Status of INT2_FTH see LIS2DS12_ACC_INT2_FTH_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT2_FTH(void *handle, LIS2DS12_ACC_INT2_FTH_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT2_FTH_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT2_STEP_DET
* Description    : Write INT2_STEP_DET
* Input          : LIS2DS12_ACC_INT2_STEP_DET_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT2_STEP_DET(void *handle, LIS2DS12_ACC_INT2_STEP_DET_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT2_STEP_DET_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT2_STEP_DET
* Description    : Read INT2_STEP_DET
* Input          : Pointer to LIS2DS12_ACC_INT2_STEP_DET_t
* Output         : Status of INT2_STEP_DET see LIS2DS12_ACC_INT2_STEP_DET_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT2_STEP_DET(void *handle, LIS2DS12_ACC_INT2_STEP_DET_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT2_STEP_DET_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT2_SIG_MOT
* Description    : Write INT2_SIG_MOT
* Input          : LIS2DS12_ACC_INT2_SIG_MOT_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT2_SIG_MOT(void *handle, LIS2DS12_ACC_INT2_SIG_MOT_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT2_SIG_MOT_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT2_SIG_MOT
* Description    : Read INT2_SIG_MOT
* Input          : Pointer to LIS2DS12_ACC_INT2_SIG_MOT_t
* Output         : Status of INT2_SIG_MOT see LIS2DS12_ACC_INT2_SIG_MOT_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT2_SIG_MOT(void *handle, LIS2DS12_ACC_INT2_SIG_MOT_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT2_SIG_MOT_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT2_TILT
* Description    : Write INT2_TILT
* Input          : LIS2DS12_ACC_INT2_TILT_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT2_TILT(void *handle, LIS2DS12_ACC_INT2_TILT_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT2_TILT_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT2_TILT
* Description    : Read INT2_TILT
* Input          : Pointer to LIS2DS12_ACC_INT2_TILT_t
* Output         : Status of INT2_TILT see LIS2DS12_ACC_INT2_TILT_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT2_TILT(void *handle, LIS2DS12_ACC_INT2_TILT_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT2_TILT_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT2_ON_INT1
* Description    : Write INT2_ON_INT1
* Input          : LIS2DS12_ACC_INT2_ON_INT1_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT2_ON_INT1(void *handle, LIS2DS12_ACC_INT2_ON_INT1_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT2_ON_INT1_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT2_ON_INT1
* Description    : Read INT2_ON_INT1
* Input          : Pointer to LIS2DS12_ACC_INT2_ON_INT1_t
* Output         : Status of INT2_ON_INT1 see LIS2DS12_ACC_INT2_ON_INT1_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT2_ON_INT1(void *handle, LIS2DS12_ACC_INT2_ON_INT1_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT2_ON_INT1_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT2_BOOT
* Description    : Write INT2_BOOT
* Input          : LIS2DS12_ACC_INT2_BOOT_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT2_BOOT(void *handle, LIS2DS12_ACC_INT2_BOOT_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT2_BOOT_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT2_BOOT
* Description    : Read INT2_BOOT
* Input          : Pointer to LIS2DS12_ACC_INT2_BOOT_t
* Output         : Status of INT2_BOOT see LIS2DS12_ACC_INT2_BOOT_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT2_BOOT(void *handle, LIS2DS12_ACC_INT2_BOOT_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT2_BOOT_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_DRDY_PULSED
* Description    : Write DRDY_PULSED
* Input          : LIS2DS12_ACC_DRDY_PULSED_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_DRDY_PULSED(void *handle, LIS2DS12_ACC_DRDY_PULSED_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_DRDY_PULSED_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_CTRL5, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_DRDY_PULSED
* Description    : Read DRDY_PULSED
* Input          : Pointer to LIS2DS12_ACC_DRDY_PULSED_t
* Output         : Status of DRDY_PULSED see LIS2DS12_ACC_DRDY_PULSED_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_DRDY_PULSED(void *handle, LIS2DS12_ACC_DRDY_PULSED_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_CTRL5, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_DRDY_PULSED_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_PullUP_Disc
* Description    : Write IF_CS_PU_DIS
* Input          : LIS2DS12_ACC_IF_CS_PU_DIS_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_PullUP_Disc(void *handle, LIS2DS12_ACC_IF_CS_PU_DIS_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_CTRL, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_IF_CS_PU_DIS_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FIFO_CTRL, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_PullUP_Disc
* Description    : Read IF_CS_PU_DIS
* Input          : Pointer to LIS2DS12_ACC_IF_CS_PU_DIS_t
* Output         : Status of IF_CS_PU_DIS see LIS2DS12_ACC_IF_CS_PU_DIS_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_PullUP_Disc(void *handle, LIS2DS12_ACC_IF_CS_PU_DIS_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_CTRL, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_IF_CS_PU_DIS_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_MODULE_TO_FIFO
* Description    : Write MODULE_TO_FIFO
* Input          : LIS2DS12_ACC_MODULE_TO_FIFO_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_MODULE_TO_FIFO(void *handle, LIS2DS12_ACC_MODULE_TO_FIFO_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_CTRL, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_MODULE_TO_FIFO_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FIFO_CTRL, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_MODULE_TO_FIFO
* Description    : Read MODULE_TO_FIFO
* Input          : Pointer to LIS2DS12_ACC_MODULE_TO_FIFO_t
* Output         : Status of MODULE_TO_FIFO see LIS2DS12_ACC_MODULE_TO_FIFO_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_MODULE_TO_FIFO(void *handle, LIS2DS12_ACC_MODULE_TO_FIFO_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_CTRL, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_MODULE_TO_FIFO_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_FMODE
* Description    : Write FMODE
* Input          : LIS2DS12_ACC_FMODE_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_FMODE(void *handle, LIS2DS12_ACC_FMODE_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_CTRL, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_FMODE_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FIFO_CTRL, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FMODE
* Description    : Read FMODE
* Input          : Pointer to LIS2DS12_ACC_FMODE_t
* Output         : Status of FMODE see LIS2DS12_ACC_FMODE_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FMODE(void *handle, LIS2DS12_ACC_FMODE_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_CTRL, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FMODE_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_Temperature
* Description    : Read TEMP_BIT
* Input          : Pointer to uint8_t
* Output         : Status of TEMP_BIT
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_Temperature(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_OUT_T, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_TEMP_BIT_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_TEMP_BIT_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_DRDY
* Description    : Read DRDY
* Input          : Pointer to LIS2DS12_ACC_DRDY_t
* Output         : Status of DRDY see LIS2DS12_ACC_DRDY_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_DRDY(void *handle, LIS2DS12_ACC_DRDY_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_DRDY_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FF_IA
* Description    : Read FF_IA
* Input          : Pointer to LIS2DS12_ACC_FF_IA_t
* Output         : Status of FF_IA see LIS2DS12_ACC_FF_IA_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FF_IA(void *handle, LIS2DS12_ACC_FF_IA_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FF_IA_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_6D_IA
* Description    : Read 6D_IA
* Input          : Pointer to LIS2DS12_ACC_6D_IA_t
* Output         : Status of 6D_IA see LIS2DS12_ACC_6D_IA_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_6D_IA(void *handle, LIS2DS12_ACC_6D_IA_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_6D_IA_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SINGLE_TAP
* Description    : Read SINGLE_TAP
* Input          : Pointer to LIS2DS12_ACC_SINGLE_TAP_t
* Output         : Status of SINGLE_TAP see LIS2DS12_ACC_SINGLE_TAP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SINGLE_TAP(void *handle, LIS2DS12_ACC_SINGLE_TAP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SINGLE_TAP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_DOUBLE_TAP
* Description    : Read DOUBLE_TAP
* Input          : Pointer to LIS2DS12_ACC_DOUBLE_TAP_t
* Output         : Status of DOUBLE_TAP see LIS2DS12_ACC_DOUBLE_TAP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_DOUBLE_TAP(void *handle, LIS2DS12_ACC_DOUBLE_TAP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_DOUBLE_TAP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SLEEP_STATE
* Description    : Read SLEEP_STATE
* Input          : Pointer to LIS2DS12_ACC_SLEEP_STATE_t
* Output         : Status of SLEEP_STATE see LIS2DS12_ACC_SLEEP_STATE_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SLEEP_STATE(void *handle, LIS2DS12_ACC_SLEEP_STATE_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SLEEP_STATE_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_WU_IA
* Description    : Read WU_IA
* Input          : Pointer to LIS2DS12_ACC_WU_IA_t
* Output         : Status of WU_IA see LIS2DS12_ACC_WU_IA_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_WU_IA(void *handle, LIS2DS12_ACC_WU_IA_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_WU_IA_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FIFO_THS
* Description    : Read FIFO_THS
* Input          : Pointer to LIS2DS12_ACC_FIFO_THS_t
* Output         : Status of FIFO_THS see LIS2DS12_ACC_FIFO_THS_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FIFO_THS(void *handle, LIS2DS12_ACC_FIFO_THS_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FIFO_THS_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_FifoThsld
* Description    : Write FTH
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_FifoThsld(void *handle, uint8_t newValue)
{
    uint8_t value;

    newValue = newValue << LIS2DS12_ACC_FTH_POSITION; //mask
    newValue &= LIS2DS12_ACC_FTH_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_THS, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_FTH_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FIFO_THS, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FifoThsld
* Description    : Read FTH
* Input          : Pointer to uint8_t
* Output         : Status of FTH
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FifoThsld(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_THS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FTH_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_FTH_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FIFO_OVR
* Description    : Read FIFO_OVR
* Input          : Pointer to LIS2DS12_ACC_FIFO_OVR_t
* Output         : Status of FIFO_OVR see LIS2DS12_ACC_FIFO_OVR_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FIFO_OVR(void *handle, LIS2DS12_ACC_FIFO_OVR_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FIFO_OVR_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FTH
* Description    : Read FTH
* Input          : Pointer to LIS2DS12_ACC_FTH_STATUS_t
* Output         : Status of FTH see LIS2DS12_ACC_FTH_STATUS_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FTH(void *handle, LIS2DS12_ACC_FTH_STATUS_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FTH_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SamplesNum
* Description    : Read SAMPLE
* Input          : Pointer to uint16_t
* Output         : Status of SAMPLE
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SamplesNum(void *handle, uint16_t *value)
{
    uint8_t valueH, valueL;

    /* Low part */
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_SAMPLES, (uint8_t *)&valueL, 1) )
        return MEMS_ERROR;

    valueL &= LIS2DS12_ACC_SAMPLE_L_MASK; //coerce
    valueL = valueL >> LIS2DS12_ACC_SAMPLE_L_POSITION; //mask

    /* High part */
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FIFO_SRC, (uint8_t *)&valueH, 1) )
        return MEMS_ERROR;

    valueH &= LIS2DS12_ACC_SAMPLE_H_MASK; //coerce
    valueH = valueH >> LIS2DS12_ACC_SAMPLE_H_POSITION; //mask

    *value = ((valueH << 8) & 0x100) | valueL;

    return MEMS_SUCCESS;

}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_TAP_THS
* Description    : Write TAP_THS
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_TAP_THS(void *handle, uint8_t newValue)
{
    uint8_t value;

    newValue = newValue << LIS2DS12_ACC_TAP_THS_POSITION; //mask
    newValue &= LIS2DS12_ACC_TAP_THS_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_6D_THS, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_TAP_THS_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_TAP_6D_THS, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_TAP_THS
* Description    : Read TAP_THS
* Input          : Pointer to uint8_t
* Output         : Status of TAP_THS
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_TAP_THS(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_6D_THS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_TAP_THS_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_TAP_THS_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_6D_THS
* Description    : Write 6D_THS
* Input          : LIS2DS12_ACC_6D_THS_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_6D_THS(void *handle, LIS2DS12_ACC_6D_THS_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_6D_THS, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_6D_THS_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_TAP_6D_THS, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_6D_THS
* Description    : Read 6D_THS
* Input          : Pointer to LIS2DS12_ACC_6D_THS_t
* Output         : Status of 6D_THS see LIS2DS12_ACC_6D_THS_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_6D_THS(void *handle, LIS2DS12_ACC_6D_THS_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_6D_THS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_6D_THS_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_4D_EN
* Description    : Write 4D_EN
* Input          : LIS2DS12_ACC_4D_EN_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_4D_EN(void *handle, LIS2DS12_ACC_4D_EN_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_6D_THS, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_4D_EN_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_TAP_6D_THS, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_4D_EN
* Description    : Read 4D_EN
* Input          : Pointer to LIS2DS12_ACC_4D_EN_t
* Output         : Status of 4D_EN see LIS2DS12_ACC_4D_EN_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_4D_EN(void *handle, LIS2DS12_ACC_4D_EN_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_6D_THS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_4D_EN_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_SHOCK
* Description    : Write SHOCK
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_SHOCK(void *handle, uint8_t newValue)
{
    uint8_t value;

    newValue = newValue << LIS2DS12_ACC_SHOCK_POSITION; //mask
    newValue &= LIS2DS12_ACC_SHOCK_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_INT_DUR, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_SHOCK_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_INT_DUR, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SHOCK
* Description    : Read SHOCK
* Input          : Pointer to uint8_t
* Output         : Status of SHOCK
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SHOCK(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_INT_DUR, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SHOCK_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_SHOCK_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_QUIET
* Description    : Write QUIET
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_QUIET(void *handle, uint8_t newValue)
{
    uint8_t value;

    newValue = newValue << LIS2DS12_ACC_QUIET_POSITION; //mask
    newValue &= LIS2DS12_ACC_QUIET_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_INT_DUR, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_QUIET_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_INT_DUR, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_QUIET
* Description    : Read QUIET
* Input          : Pointer to uint8_t
* Output         : Status of QUIET
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_QUIET(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_INT_DUR, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_QUIET_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_QUIET_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_LAT
* Description    : Write LAT
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_LAT(void *handle, uint8_t newValue)
{
    uint8_t value;

    newValue = newValue << LIS2DS12_ACC_LAT_POSITION; //mask
    newValue &= LIS2DS12_ACC_LAT_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_INT_DUR, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_LAT_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_INT_DUR, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_LAT
* Description    : Read LAT
* Input          : Pointer to uint8_t
* Output         : Status of LAT
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_LAT(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_INT_DUR, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_LAT_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_LAT_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_WU_THS
* Description    : Write WU_THS
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_WU_THS(void *handle, uint8_t newValue)
{
    uint8_t value;

    newValue = newValue << LIS2DS12_ACC_WU_THS_POSITION; //mask
    newValue &= LIS2DS12_ACC_WU_THS_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_THS, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_WU_THS_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_WAKE_UP_THS, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_WU_THS
* Description    : Read WU_THS
* Input          : Pointer to uint8_t
* Output         : Status of WU_THS
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_WU_THS(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_THS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_WU_THS_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_WU_THS_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_SLEEP_ON
* Description    : Write SLEEP_ON
* Input          : LIS2DS12_ACC_SLEEP_ON_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_SLEEP_ON(void *handle, LIS2DS12_ACC_SLEEP_ON_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_THS, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_SLEEP_ON_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_WAKE_UP_THS, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SLEEP_ON
* Description    : Read SLEEP_ON
* Input          : Pointer to LIS2DS12_ACC_SLEEP_ON_t
* Output         : Status of SLEEP_ON see LIS2DS12_ACC_SLEEP_ON_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SLEEP_ON(void *handle, LIS2DS12_ACC_SLEEP_ON_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_THS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SLEEP_ON_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_SINGLE_DOUBLE_TAP
* Description    : Write SINGLE_DOUBLE_TAP
* Input          : LIS2DS12_ACC_SINGLE_DOUBLE_TAP_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_SINGLE_DOUBLE_TAP(void *handle, LIS2DS12_ACC_SINGLE_DOUBLE_TAP_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_THS, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_SINGLE_DOUBLE_TAP_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_WAKE_UP_THS, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SINGLE_DOUBLE_TAP
* Description    : Read SINGLE_DOUBLE_TAP
* Input          : Pointer to LIS2DS12_ACC_SINGLE_DOUBLE_TAP_t
* Output         : Status of SINGLE_DOUBLE_TAP see LIS2DS12_ACC_SINGLE_DOUBLE_TAP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SINGLE_DOUBLE_TAP(void *handle, LIS2DS12_ACC_SINGLE_DOUBLE_TAP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_THS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SINGLE_DOUBLE_TAP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_SleepDuration
* Description    : Write SLEEP_DUR
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_SleepDuration(void *handle, uint8_t newValue)
{
    uint8_t value;

    newValue = newValue << LIS2DS12_ACC_SLEEP_DUR_POSITION; //mask
    newValue &= LIS2DS12_ACC_SLEEP_DUR_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_SLEEP_DUR_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SleepDuration
* Description    : Read SLEEP_DUR
* Input          : Pointer to uint8_t
* Output         : Status of SLEEP_DUR
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SleepDuration(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SLEEP_DUR_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_SLEEP_DUR_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_INT1_FIFO_FULL
* Description    : Write INT1_FIFO_FULL
* Input          : LIS2DS12_ACC_INT1_FIFO_FULL_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_INT1_FIFO_FULL(void *handle, LIS2DS12_ACC_INT1_FIFO_FULL_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_INT1_FIFO_FULL_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_INT1_FIFO_FULL
* Description    : Read INT1_FIFO_FULL
* Input          : Pointer to LIS2DS12_ACC_INT1_FIFO_FULL_t
* Output         : Status of INT1_FIFO_FULL see LIS2DS12_ACC_INT1_FIFO_FULL_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_INT1_FIFO_FULL(void *handle, LIS2DS12_ACC_INT1_FIFO_FULL_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_INT1_FIFO_FULL_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_WakeUpDuration
* Description    : Write WU_DUR
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_WakeUpDuration(void *handle, uint8_t newValue)
{
    uint8_t value;

    newValue = newValue << LIS2DS12_ACC_WU_DUR_POSITION; //mask
    newValue &= LIS2DS12_ACC_WU_DUR_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_WU_DUR_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_WakeUpDuration
* Description    : Read WU_DUR
* Input          : Pointer to uint8_t
* Output         : Status of WU_DUR
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_WakeUpDuration(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_WU_DUR_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_WU_DUR_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_FreeFallThreshold
* Description    : Write FF_THS
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_FreeFallThreshold(void *handle, uint8_t newValue)
{
    uint8_t value;

    newValue = newValue << LIS2DS12_ACC_FF_THS_POSITION; //mask
    newValue &= LIS2DS12_ACC_FF_THS_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FREE_FALL, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_FF_THS_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FREE_FALL, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FreeFallThreshold
* Description    : Read FF_THS
* Input          : Pointer to uint8_t
* Output         : Status of FF_THS
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FreeFallThreshold(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FREE_FALL, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FF_THS_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_FF_THS_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_FreeFallDuration
* Description    : Write FF_DUR
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_FreeFallDuration(void *handle, uint8_t newValue)
{
    uint8_t valueH, valueL;
    uint8_t value;

    valueL = newValue & 0x1F;
    valueH = (newValue >> 5) & 0x1;

    /* Low part  */
    valueL = valueL << LIS2DS12_ACC_FF_DUR_LOW_POSITION; //mask
    valueL &= LIS2DS12_ACC_FF_DUR_LOW_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FREE_FALL, &value, 1) )
        return MEMS_ERROR;

    value = (value & ~LIS2DS12_ACC_FF_DUR_LOW_MASK) | valueL;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FREE_FALL, &value, 1) )
        return MEMS_ERROR;

    /* High part */
    valueH = valueH << LIS2DS12_ACC_FF_DUR_HIGH_POSITION; //mask
    valueH &= LIS2DS12_ACC_FF_DUR_HIGH_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, &value, 1) )
        return MEMS_ERROR;

    value = (value & ~LIS2DS12_ACC_FF_DUR_HIGH_MASK) | valueH;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FreeFallDuration
* Description    : Read FF_DUR
* Input          : Pointer to uint8_t
* Output         : Status of FF_DUR
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FreeFallDuration(void *handle, uint8_t *value)
{
    uint8_t value_tmp;

    /* Low part */
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FREE_FALL, (uint8_t *)&value_tmp, 1) )
        return MEMS_ERROR;

    value_tmp &= LIS2DS12_ACC_FF_DUR_LOW_MASK; //coerce
    *value = value_tmp >> LIS2DS12_ACC_FF_DUR_LOW_POSITION; //mask

    /* High part */
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_DUR, (uint8_t *)&value_tmp, 1) )
        return MEMS_ERROR;

    value_tmp &= LIS2DS12_ACC_FF_DUR_HIGH_MASK; //coerce
    *value = *value | (value_tmp >> LIS2DS12_ACC_FF_DUR_HIGH_POSITION); //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_DRDY_DUP
* Description    : Read DRDY
* Input          : Pointer to LIS2DS12_ACC_DRDY_DUP_t
* Output         : Status of DRDY see LIS2DS12_ACC_DRDY_DUP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_DRDY_DUP(void *handle, LIS2DS12_ACC_DRDY_DUP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS_DUP, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_DRDY_DUP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FF_IA_DUP
* Description    : Read FF_IA
* Input          : Pointer to LIS2DS12_ACC_FF_IA_DUP_t
* Output         : Status of FF_IA see LIS2DS12_ACC_FF_IA_DUP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FF_IA_DUP(void *handle, LIS2DS12_ACC_FF_IA_DUP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS_DUP, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FF_IA_DUP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_6D_IA_DUP
* Description    : Read 6D_IA
* Input          : Pointer to LIS2DS12_ACC_6D_IA_DUP_t
* Output         : Status of 6D_IA see LIS2DS12_ACC_6D_IA_DUP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_6D_IA_DUP(void *handle, LIS2DS12_ACC_6D_IA_DUP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS_DUP, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_6D_IA_DUP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SINGLE_TAP_DUP
* Description    : Read SINGLE_TAP
* Input          : Pointer to LIS2DS12_ACC_SINGLE_TAP_DUP_t
* Output         : Status of SINGLE_TAP see LIS2DS12_ACC_SINGLE_TAP_DUP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SINGLE_TAP_DUP(void *handle, LIS2DS12_ACC_SINGLE_TAP_DUP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS_DUP, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SINGLE_TAP_DUP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_DOUBLE_TAP_DUP
* Description    : Read DOUBLE_TAP
* Input          : Pointer to LIS2DS12_ACC_DOUBLE_TAP_DUP_t
* Output         : Status of DOUBLE_TAP see LIS2DS12_ACC_DOUBLE_TAP_DUP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_DOUBLE_TAP_DUP(void *handle, LIS2DS12_ACC_DOUBLE_TAP_DUP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS_DUP, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_DOUBLE_TAP_DUP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SLEEP_STATE_DUP
* Description    : Read SLEEP_STATE
* Input          : Pointer to LIS2DS12_ACC_SLEEP_STATE_DUP_t
* Output         : Status of SLEEP_STATE see LIS2DS12_ACC_SLEEP_STATE_DUP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SLEEP_STATE_DUP(void *handle, LIS2DS12_ACC_SLEEP_STATE_DUP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS_DUP, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SLEEP_STATE_DUP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_WU_IA_DUP
* Description    : Read WU_IA
* Input          : Pointer to LIS2DS12_ACC_WU_IA_DUP_t
* Output         : Status of WU_IA see LIS2DS12_ACC_WU_IA_DUP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_WU_IA_DUP(void *handle, LIS2DS12_ACC_WU_IA_DUP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS_DUP, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_WU_IA_DUP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_OVR_DUP
* Description    : Read OVR
* Input          : Pointer to LIS2DS12_ACC_OVR_DUP_t
* Output         : Status of OVR see LIS2DS12_ACC_OVR_DUP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_OVR_DUP(void *handle, LIS2DS12_ACC_OVR_DUP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STATUS_DUP, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_OVR_DUP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_Z_WU
* Description    : Read Z_WU
* Input          : Pointer to LIS2DS12_ACC_Z_WU_t
* Output         : Status of Z_WU see LIS2DS12_ACC_Z_WU_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_Z_WU(void *handle, LIS2DS12_ACC_Z_WU_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_Z_WU_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_Y_WU
* Description    : Read Y_WU
* Input          : Pointer to LIS2DS12_ACC_Y_WU_t
* Output         : Status of Y_WU see LIS2DS12_ACC_Y_WU_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_Y_WU(void *handle, LIS2DS12_ACC_Y_WU_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_Y_WU_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_X_WU
* Description    : Read X_WU
* Input          : Pointer to LIS2DS12_ACC_X_WU_t
* Output         : Status of X_WU see LIS2DS12_ACC_X_WU_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_X_WU(void *handle, LIS2DS12_ACC_X_WU_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_X_WU_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_WU_IA_DUP2
* Description    : Read WU_IA
* Input          : Pointer to LIS2DS12_ACC_WU_IA_DUP2_t
* Output         : Status of WU_IA see LIS2DS12_ACC_WU_IA_DUP2_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_WU_IA_DUP2(void *handle, LIS2DS12_ACC_WU_IA_DUP2_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_WU_IA_DUP2_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SLEEP_STATE_IA
* Description    : Read SLEEP_STATE_IA
* Input          : Pointer to LIS2DS12_ACC_SLEEP_STATE_IA_t
* Output         : Status of SLEEP_STATE_IA see LIS2DS12_ACC_SLEEP_STATE_IA_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SLEEP_STATE_IA(void *handle, LIS2DS12_ACC_SLEEP_STATE_IA_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SLEEP_STATE_IA_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FF_IA_DUP2
* Description    : Read FF_IA
* Input          : Pointer to LIS2DS12_ACC_FF_IA_DUP2_t
* Output         : Status of FF_IA see LIS2DS12_ACC_FF_IA_DUP2_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FF_IA_DUP2(void *handle, LIS2DS12_ACC_FF_IA_DUP2_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_WAKE_UP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FF_IA_DUP2_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_Z_TAP
* Description    : Read Z_TAP
* Input          : Pointer to LIS2DS12_ACC_Z_TAP_t
* Output         : Status of Z_TAP see LIS2DS12_ACC_Z_TAP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_Z_TAP(void *handle, LIS2DS12_ACC_Z_TAP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_Z_TAP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_Y_TAP
* Description    : Read Y_TAP
* Input          : Pointer to LIS2DS12_ACC_Y_TAP_t
* Output         : Status of Y_TAP see LIS2DS12_ACC_Y_TAP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_Y_TAP(void *handle, LIS2DS12_ACC_Y_TAP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_Y_TAP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_X_TAP
* Description    : Read X_TAP
* Input          : Pointer to LIS2DS12_ACC_X_TAP_t
* Output         : Status of X_TAP see LIS2DS12_ACC_X_TAP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_X_TAP(void *handle, LIS2DS12_ACC_X_TAP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_X_TAP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_TAP_SIGN
* Description    : Read TAP_SIGN
* Input          : Pointer to LIS2DS12_ACC_TAP_SIGN_t
* Output         : Status of TAP_SIGN see LIS2DS12_ACC_TAP_SIGN_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_TAP_SIGN(void *handle, LIS2DS12_ACC_TAP_SIGN_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_TAP_SIGN_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_DOUBLE_TAP_DUP2
* Description    : Read DOUBLE_TAP
* Input          : Pointer to LIS2DS12_ACC_DOUBLE_TAP_DUP2_t
* Output         : Status of DOUBLE_TAP see LIS2DS12_ACC_DOUBLE_TAP_DUP2_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_DOUBLE_TAP_DUP2(void *handle, LIS2DS12_ACC_DOUBLE_TAP_DUP2_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_DOUBLE_TAP_DUP2_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SINGLE_TAP_DUP2
* Description    : Read SINGLE_TAP
* Input          : Pointer to LIS2DS12_ACC_SINGLE_TAP_DUP2_t
* Output         : Status of SINGLE_TAP see LIS2DS12_ACC_SINGLE_TAP_DUP2_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SINGLE_TAP_DUP2(void *handle, LIS2DS12_ACC_SINGLE_TAP_DUP2_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SINGLE_TAP_DUP2_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_TAP_IA
* Description    : Read TAP_IA
* Input          : Pointer to LIS2DS12_ACC_TAP_IA_t
* Output         : Status of TAP_IA see LIS2DS12_ACC_TAP_IA_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_TAP_IA(void *handle, LIS2DS12_ACC_TAP_IA_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_TAP_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_TAP_IA_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_XL
* Description    : Read XL
* Input          : Pointer to LIS2DS12_ACC_XL_t
* Output         : Status of XL see LIS2DS12_ACC_XL_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_XL(void *handle, LIS2DS12_ACC_XL_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_6D_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_XL_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_XH
* Description    : Read XH
* Input          : Pointer to LIS2DS12_ACC_XH_t
* Output         : Status of XH see LIS2DS12_ACC_XH_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_XH(void *handle, LIS2DS12_ACC_XH_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_6D_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_XH_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_YL
* Description    : Read YL
* Input          : Pointer to LIS2DS12_ACC_YL_t
* Output         : Status of YL see LIS2DS12_ACC_YL_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_YL(void *handle, LIS2DS12_ACC_YL_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_6D_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_YL_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_YH
* Description    : Read YH
* Input          : Pointer to LIS2DS12_ACC_YH_t
* Output         : Status of YH see LIS2DS12_ACC_YH_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_YH(void *handle, LIS2DS12_ACC_YH_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_6D_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_YH_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_ZL
* Description    : Read ZL
* Input          : Pointer to LIS2DS12_ACC_ZL_t
* Output         : Status of ZL see LIS2DS12_ACC_ZL_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_ZL(void *handle, LIS2DS12_ACC_ZL_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_6D_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_ZL_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_ZH
* Description    : Read ZH
* Input          : Pointer to LIS2DS12_ACC_ZH_t
* Output         : Status of ZH see LIS2DS12_ACC_ZH_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_ZH(void *handle, LIS2DS12_ACC_ZH_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_6D_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_ZH_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_6D_IA_DUP2
* Description    : Read 6D_IA
* Input          : Pointer to LIS2DS12_ACC_6D_IA_DUP2_t
* Output         : Status of 6D_IA see LIS2DS12_ACC_6D_IA_DUP2_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_6D_IA_DUP2(void *handle, LIS2DS12_ACC_6D_IA_DUP2_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_6D_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_6D_IA_DUP2_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_SC_MTHS
* Description    : Write SC_MTHS
* Input          : uint8_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_SC_MTHS(void *handle, uint8_t newValue)
{
    uint8_t value;

    newValue = newValue << LIS2DS12_ACC_SC_MTHS_POSITION; //mask
    newValue &= LIS2DS12_ACC_SC_MTHS_MASK; //coerce

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STEP_C_MINTHS, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_SC_MTHS_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_STEP_C_MINTHS, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SC_MTHS
* Description    : Read SC_MTHS
* Input          : Pointer to uint8_t
* Output         : Status of SC_MTHS
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SC_MTHS(void *handle, uint8_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STEP_C_MINTHS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SC_MTHS_MASK; //coerce
    *value = *value >> LIS2DS12_ACC_SC_MTHS_POSITION; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_PEDO4G
* Description    : Write PEDO4G
* Input          : LIS2DS12_ACC_PEDO4G_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_PEDO4G(void *handle, LIS2DS12_ACC_PEDO4G_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STEP_C_MINTHS, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_PEDO4G_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_STEP_C_MINTHS, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_PEDO4G
* Description    : Read PEDO4G
* Input          : Pointer to LIS2DS12_ACC_PEDO4G_t
* Output         : Status of PEDO4G see LIS2DS12_ACC_PEDO4G_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_PEDO4G(void *handle, LIS2DS12_ACC_PEDO4G_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STEP_C_MINTHS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_PEDO4G_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_RST_NSTEP
* Description    : Write RST_NSTEP
* Input          : LIS2DS12_ACC_RST_NSTEP_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_RST_NSTEP(void *handle, LIS2DS12_ACC_RST_NSTEP_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STEP_C_MINTHS, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_RST_NSTEP_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_STEP_C_MINTHS, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_RST_NSTEP
* Description    : Read RST_NSTEP
* Input          : Pointer to LIS2DS12_ACC_RST_NSTEP_t
* Output         : Status of RST_NSTEP see LIS2DS12_ACC_RST_NSTEP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_RST_NSTEP(void *handle, LIS2DS12_ACC_RST_NSTEP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STEP_C_MINTHS, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_RST_NSTEP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_CK_GATE_FUNC
* Description    : Read CK_GATE_FUNC
* Input          : Pointer to LIS2DS12_ACC_CK_GATE_FUNC_t
* Output         : Status of CK_GATE_FUNC see LIS2DS12_ACC_CK_GATE_FUNC_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_CK_GATE_FUNC(void *handle, LIS2DS12_ACC_CK_GATE_FUNC_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CK_GATE, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_CK_GATE_FUNC_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_STEP_DETECT
* Description    : Read STEP_DETECT
* Input          : Pointer to LIS2DS12_ACC_STEP_DETECT_t
* Output         : Status of STEP_DETECT see LIS2DS12_ACC_STEP_DETECT_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_STEP_DETECT(void *handle, LIS2DS12_ACC_STEP_DETECT_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CK_GATE, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_STEP_DETECT_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_RST_PEDO
* Description    : Read RST_PEDO
* Input          : Pointer to LIS2DS12_ACC_RST_PEDO_t
* Output         : Status of RST_PEDO see LIS2DS12_ACC_RST_PEDO_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_RST_PEDO(void *handle, LIS2DS12_ACC_RST_PEDO_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CK_GATE, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_RST_PEDO_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_RST_SIGN_MOT
* Description    : Read RST_SIGN_MOT
* Input          : Pointer to LIS2DS12_ACC_RST_SIGN_MOT_t
* Output         : Status of RST_SIGN_MOT see LIS2DS12_ACC_RST_SIGN_MOT_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_RST_SIGN_MOT(void *handle, LIS2DS12_ACC_RST_SIGN_MOT_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CK_GATE, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_RST_SIGN_MOT_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SIG_MOT_DETECT
* Description    : Read SIG_MOT_DETECT
* Input          : Pointer to LIS2DS12_ACC_SIG_MOT_DETECT_t
* Output         : Status of SIG_MOT_DETECT see LIS2DS12_ACC_SIG_MOT_DETECT_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SIG_MOT_DETECT(void *handle, LIS2DS12_ACC_SIG_MOT_DETECT_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CK_GATE, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SIG_MOT_DETECT_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_FS_SRC
* Description    : Read FS_SRC
* Input          : Pointer to LIS2DS12_ACC_FS_SRC_t
* Output         : Status of FS_SRC see LIS2DS12_ACC_FS_SRC_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_FS_SRC(void *handle, LIS2DS12_ACC_FS_SRC_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CK_GATE, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_FS_SRC_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_TILT_INT
* Description    : Read TILT_INT
* Input          : Pointer to LIS2DS12_ACC_TILT_INT_t
* Output         : Status of TILT_INT see LIS2DS12_ACC_TILT_INT_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_TILT_INT(void *handle, LIS2DS12_ACC_TILT_INT_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CK_GATE, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_TILT_INT_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SENS_HUB_END_OP
* Description    : Read SENS_HUB_END_OP
* Input          : Pointer to LIS2DS12_ACC_SENS_HUB_END_OP_t
* Output         : Status of SENS_HUB_END_OP see LIS2DS12_ACC_SENS_HUB_END_OP_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SENS_HUB_END_OP(void *handle, LIS2DS12_ACC_SENS_HUB_END_OP_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SENS_HUB_END_OP_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_MODULE_READY
* Description    : Read MODULE_READY
* Input          : Pointer to LIS2DS12_ACC_MODULE_READY_t
* Output         : Status of MODULE_READY see LIS2DS12_ACC_MODULE_READY_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_MODULE_READY(void *handle, LIS2DS12_ACC_MODULE_READY_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_MODULE_READY_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_RST_TILT
* Description    : Read RST_TILT
* Input          : Pointer to LIS2DS12_ACC_RST_TILT_t
* Output         : Status of RST_TILT see LIS2DS12_ACC_RST_TILT_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_RST_TILT(void *handle, LIS2DS12_ACC_RST_TILT_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_SRC, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_RST_TILT_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_STEP_CNT_ON
* Description    : Write STEP_CNT_ON
* Input          : LIS2DS12_ACC_STEP_CNT_ON_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_STEP_CNT_ON(void *handle, LIS2DS12_ACC_STEP_CNT_ON_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_STEP_CNT_ON_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_STEP_CNT_ON
* Description    : Read STEP_CNT_ON
* Input          : Pointer to LIS2DS12_ACC_STEP_CNT_ON_t
* Output         : Status of STEP_CNT_ON see LIS2DS12_ACC_STEP_CNT_ON_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_STEP_CNT_ON(void *handle, LIS2DS12_ACC_STEP_CNT_ON_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_STEP_CNT_ON_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_SIGN_MOT_ON
* Description    : Write SIGN_MOT_ON
* Input          : LIS2DS12_ACC_SIGN_MOT_ON_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_SIGN_MOT_ON(void *handle, LIS2DS12_ACC_SIGN_MOT_ON_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_SIGN_MOT_ON_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_SIGN_MOT_ON
* Description    : Read SIGN_MOT_ON
* Input          : Pointer to LIS2DS12_ACC_SIGN_MOT_ON_t
* Output         : Status of SIGN_MOT_ON see LIS2DS12_ACC_SIGN_MOT_ON_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_SIGN_MOT_ON(void *handle, LIS2DS12_ACC_SIGN_MOT_ON_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_SIGN_MOT_ON_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_MASTER_ON
* Description    : Write MASTER_ON
* Input          : LIS2DS12_ACC_MASTER_ON_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_MASTER_ON(void *handle, LIS2DS12_ACC_MASTER_ON_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_MASTER_ON_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_MASTER_ON
* Description    : Read MASTER_ON
* Input          : Pointer to LIS2DS12_ACC_MASTER_ON_t
* Output         : Status of MASTER_ON see LIS2DS12_ACC_MASTER_ON_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_MASTER_ON(void *handle, LIS2DS12_ACC_MASTER_ON_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_MASTER_ON_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_TUD_EN
* Description    : Write TUD_EN
* Input          : LIS2DS12_ACC_TUD_EN_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_TUD_EN(void *handle, LIS2DS12_ACC_TUD_EN_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_TUD_EN_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_TUD_EN
* Description    : Read TUD_EN
* Input          : Pointer to LIS2DS12_ACC_TUD_EN_t
* Output         : Status of TUD_EN see LIS2DS12_ACC_TUD_EN_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_TUD_EN(void *handle, LIS2DS12_ACC_TUD_EN_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_TUD_EN_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_TILT_ON
* Description    : Write TILT_ON
* Input          : LIS2DS12_ACC_TILT_ON_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_TILT_ON(void *handle, LIS2DS12_ACC_TILT_ON_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_TILT_ON_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_TILT_ON
* Description    : Read TILT_ON
* Input          : Pointer to LIS2DS12_ACC_TILT_ON_t
* Output         : Status of TILT_ON see LIS2DS12_ACC_TILT_ON_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_TILT_ON(void *handle, LIS2DS12_ACC_TILT_ON_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_TILT_ON_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_W_MODULE_ON
* Description    : Write MODULE_ON
* Input          : LIS2DS12_ACC_MODULE_ON_t
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t  LIS2DS12_ACC_W_MODULE_ON(void *handle, LIS2DS12_ACC_MODULE_ON_t newValue)
{
    uint8_t value;

    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    value &= ~LIS2DS12_ACC_MODULE_ON_MASK;
    value |= newValue;

    if( !LIS2DS12_ACC_WriteReg(handle, LIS2DS12_ACC_FUNC_CTRL, &value, 1) )
        return MEMS_ERROR;

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LIS2DS12_ACC_R_MODULE_ON
* Description    : Read MODULE_ON
* Input          : Pointer to LIS2DS12_ACC_MODULE_ON_t
* Output         : Status of MODULE_ON see LIS2DS12_ACC_MODULE_ON_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_R_MODULE_ON(void *handle, LIS2DS12_ACC_MODULE_ON_t *value)
{
    if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_FUNC_CTRL, (uint8_t *)value, 1) )
        return MEMS_ERROR;

    *(uint8_t*)value &= LIS2DS12_ACC_MODULE_ON_MASK; //mask

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : status_t LIS2DS12_ACC_Get_ExternalSensor(uint8_t *buff)
* Description    : Read ExternalSensor output register
* Input          : pointer to [uint8_t]
* Output         : ExternalSensor buffer uint8_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_Get_ExternalSensor(void *handle, uint8_t *buff)
{
    uint8_t i, j, k;
    uint8_t numberOfByteForDimension;

    numberOfByteForDimension = 6 / 3;

    k = 0;
    for (i = 0; i < 3; i++ )
    {
        for (j = 0; j < numberOfByteForDimension; j++ )
        {
            if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_SENSORHUB_OUT1 + k, &buff[k], 1))
                return MEMS_ERROR;
            k++;
        }
    }

    return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : status_t LIS2DS12_ACC_Get_StepCounter(uint8_t *buff)
* Description    : Read StepCounter output register
* Input          : pointer to [uint8_t]
* Output         : StepCounter buffer uint8_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LIS2DS12_ACC_Get_StepCounter(void *handle, uint8_t *buff)
{
    uint8_t i, j, k;
    uint8_t numberOfByteForDimension;

    numberOfByteForDimension = 2 / 1;

    k = 0;
    for (i = 0; i < 1; i++ )
    {
        for (j = 0; j < numberOfByteForDimension; j++ )
        {
            if( !LIS2DS12_ACC_ReadReg(handle, LIS2DS12_ACC_STEP_C_L + k, &buff[k], 1))
                return MEMS_ERROR;
            k++;
        }
    }

    return MEMS_SUCCESS;
}

