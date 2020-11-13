/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
* File Name          : ReadDRDY.c
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
#include "stm32f1xx_hal.h"

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static status_t response;  
static Type3Axis16bit_U Acceleration;
static u8_t WhoAmI, rst;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Test Acquisition of sensor samples */
int example_main(void)
{
  /* Check device ID */  
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
    
  /* Select interrupt pin */  
  response = LIS2DS12_ACC_W_INT1_DRDY(0, LIS2DS12_ACC_INT1_DRDY_ON);
  //response = LIS2DS12_ACC_W_INT2_DRDY(0, LIS2DS12_ACC_INT2_DRDY_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* Select interrupt pin polarity */
  response = LIS2DS12_ACC_W_H_LACTIVE(0, LIS2DS12_ACC_H_LACTIVE_high);
  if(response==MEMS_ERROR) while(1); //manage here comunication error

  /* Set DRDY mode */
  response = LIS2DS12_ACC_W_DRDY_PULSED(0, LIS2DS12_ACC_DRDY_PULSED_OFF);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* Set ACC full scale @ 2g*/
  response = LIS2DS12_ACC_W_FullScale(0, LIS2DS12_ACC_FS_2G);
  if(response==MEMS_ERROR) while(1); //manage here comunication error

  /* BDU Enable */
  response = LIS2DS12_ACC_W_BDU(0, LIS2DS12_ACC_BDU_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error

  /* Set ACC ODR  HR_14bit 100Hz*/
  response = LIS2DS12_ACC_W_ODR(0, LIS2DS12_ACC_ODR_HR_50Hz);
  if(response==MEMS_ERROR) while(1); //manage here comunication error

  /*
   * Read samples 
   */
  while(1)
  {
    //Polling on int pin driven by DRDY signal 
    if (HAL_GPIO_ReadPin(INT1_GPIO_Port, INT1_Pin))
    //if (HAL_GPIO_ReadPin(INT2_GPIO_Port, INT2_Pin))
    {
      LIS2DS12_ACC_Get_Acceleration(0, Acceleration.i16bit);
    }
  }
}
