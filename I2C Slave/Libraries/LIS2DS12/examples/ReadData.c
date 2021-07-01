/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
* File Name          : ReadData.c
* Author             : MEMS Application Team
* Version            : v1.0
* Date               : 03 Mar 2017  
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
#include "..\..\Acc\LIS2DS12_Driver__Example\trunk\LIS2DS12_Driver\LIS2DS12_ACC_driver.h"

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static status_t response;  
static Type3Axis16bit_U Acceleration;
static u8_t value_XL, WhoAmI, rst;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Test Acquisition of sensor samples */
int example_main(void)
{
    
  response = LIS2DS12_ACC_ReadReg(0, LIS2DS12_ACC_WHO_AM_I_REG, &WhoAmI, 1);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  if ( WhoAmI != LIS2DS12_ACC_WHO_AM_I ) 
  while(1); //manage here device not found  
  
  /* Restore default configuration */
  response = LIS2DS12_ACC_W_SOFT_RESET(0, LIS2DS12_ACC_SOFT_RESET_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  do 
  {
    response = LIS2DS12_ACC_R_SOFT_RESET(0, (LIS2DS12_ACC_SOFT_RESET_t*) &rst);
    if(response==MEMS_ERROR) while(1); //manage here comunication error
  }
  while (rst);

  /* Set ACC full scale @ 2g*/
  response = LIS2DS12_ACC_W_FullScale(0, LIS2DS12_ACC_FS_2G);
  if(response==MEMS_ERROR) while(1); //manage here comunication error

  /* BDU Enable */
  response = LIS2DS12_ACC_W_BDU(0, LIS2DS12_ACC_BDU_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error

  /* Set ACC ODR  HR_14bit 100Hz*/
  response = LIS2DS12_ACC_W_ODR(0, LIS2DS12_ACC_ODR_HR_14bit_200Hz);
  if(response==MEMS_ERROR) while(1); //manage here comunication error

  /*
   * Read samples in polling mode (no int)
   */
  while(1)
  {
    /*
     * Read ACC output only if new ACC value is available
     */
    response =  LIS2DS12_ACC_R_DRDY(0, (LIS2DS12_ACC_DRDY_t*) &value_XL);
    if(response==MEMS_ERROR) while(1); //manage here comunication error  
    if (LIS2DS12_ACC_DRDY_EV_ON == value_XL)
    {
      LIS2DS12_ACC_Get_Acceleration(0, Acceleration.i16bit);
    }
  }
}



