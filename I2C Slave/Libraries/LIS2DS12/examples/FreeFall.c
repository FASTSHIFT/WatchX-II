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
#define  FREE_FALL_DURATION     2 // ( 1 LSB = 1 TODR -> @ 50 Hz 20 ms)
#define  FREE_FALL_THRESHOLD    7 //LSB @ 2g in any case ( 0.061 mg/LSB )
/* Private variables ---------------------------------------------------------*/
static status_t response;  
static u8_t WhoAmI, rst, FreeFallStatus;

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
  
  /* Set ACC full scale @ 2g*/
  response = LIS2DS12_ACC_W_FullScale(0, LIS2DS12_ACC_FS_4G);
  if(response==MEMS_ERROR) while(1); //manage here comunication error  
  
  /* FreeFall Configuration */
  /* Set FreeFall Duration */
  response = LIS2DS12_ACC_W_FreeFallDuration(0, FREE_FALL_DURATION);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* Set FreeFall Threshold */
  response = LIS2DS12_ACC_W_FreeFallThreshold(0, FREE_FALL_THRESHOLD);
  if(response==MEMS_ERROR) while(1); //manage here comunication error  
  
  /* Enable FreeFall Interrupt pin */
  response = LIS2DS12_ACC_W_INT1_FF(0, LIS2DS12_ACC_INT1_FF_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error    
  
  /* PIN Configuration */
  /* Select interrupt pin polarity */
  response = LIS2DS12_ACC_W_H_LACTIVE(0, LIS2DS12_ACC_H_LACTIVE_high);
  if(response==MEMS_ERROR) while(1); //manage here comunication error

  /* Set INT notification mode */
  response = LIS2DS12_ACC_W_LIR(0, LIS2DS12_ACC_LIR_OFF);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* Powre on axl */
  /* Set ACC ODR  HR_14bit 100Hz*/
  response = LIS2DS12_ACC_W_ODR(0, LIS2DS12_ACC_ODR_HR_50Hz);
  if(response==MEMS_ERROR) while(1); //manage here comunication error

  /*
   * Read samples 
   */
  while(1)
  {
    //Polling on int pin driven by FF interrupt signal 
    if ( HAL_GPIO_ReadPin( INT1_GPIO_Port, INT1_Pin ) )
    {
        LIS2DS12_ACC_R_FF_IA_DUP2( 0, (LIS2DS12_ACC_FF_IA_DUP2_t*) &FreeFallStatus );
    }
  }
}

























