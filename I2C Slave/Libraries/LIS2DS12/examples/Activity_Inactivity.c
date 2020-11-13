/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
* File Name          : Activity_Inactivity.c
* Author             : MEMS Application Team
* Version            : v1.0
* Date               : 14 Mar 2017  
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
#define  WAKE_UP_THRESHOLD 2 // 1 LSB =  FS / 64  ( @4g -> 4g / 64 = 0.0625 g/LSb )
#define  SLEEP_DUR         1 // 1 LSB = 512 / ODR ( @100Hz ->  8/100 = 5.12 s/LSb )
#define  WAKE_UP_DUR       5 // 1 LSB =   1 / ODR ( @100Hz ->  4/100 = 10 ms/LSb )
  
typedef enum {
	SLEEP, 		 
	ACTIVE     
} xl_status_t;

/* Private variables ---------------------------------------------------------*/
static status_t response;  
static u8_t WhoAmI, rst, sleep_state_IA;
xl_status_t xl_status = SLEEP;
Type3Axis16bit_U Acceleration;

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
  
  /* Set ACC full scale @ 4g*/
  response = LIS2DS12_ACC_W_FullScale(0, LIS2DS12_ACC_FS_4G);
  if(response==MEMS_ERROR) while(1); //manage here comunication error  
  
  /* Activity-Inactivity Configuration */
  /* Set Activity threshold */
  response = LIS2DS12_ACC_W_WU_THS(0, WAKE_UP_THRESHOLD);
  if(response==MEMS_ERROR) while(1); //manage here comunication error 
  
  /* Set time before sleep */
  response = LIS2DS12_ACC_W_SleepDuration(0, SLEEP_DUR);
  if(response==MEMS_ERROR) while(1); //manage here comunication error 
  
  /* Set time before Wake Up */
  response = LIS2DS12_ACC_W_WakeUpDuration(0, WAKE_UP_DUR);
  if(response==MEMS_ERROR) while(1); //manage here comunication error 

  /* Enable Activity-Inactivity */
  response = LIS2DS12_ACC_W_SLEEP_ON(0, LIS2DS12_ACC_SLEEP_ON_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error 
  
  /* PIN Configuration */
  /* Activity on INT1 */
  response = LIS2DS12_ACC_W_INT1_WU(0, LIS2DS12_ACC_INT1_WU_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error 
  
  /* Select interrupt pin polarity */
  response = LIS2DS12_ACC_W_H_LACTIVE(0, LIS2DS12_ACC_H_LACTIVE_high);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* Route DRDY on INT2 (Only used to see ODR changes)*/
  response = LIS2DS12_ACC_W_INT2_DRDY(0, LIS2DS12_ACC_INT2_DRDY_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error 
  
  /* Powre on axl */
  /* Set ACC ODR  HR_14bit 100Hz*/
  response = LIS2DS12_ACC_W_ODR(0, LIS2DS12_ACC_ODR_HR_14bit_100Hz);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  while(1)
  {
    //Polling on int pin driven by DRDY signal (Only used to see ODR changes)
    if (HAL_GPIO_ReadPin(INT2_GPIO_Port, INT2_Pin))
    {
      LIS2DS12_ACC_Get_Acceleration(0, Acceleration.i16bit);
    } 
    
    //check when the status change in sleep
    if (xl_status == ACTIVE)
    {
      LIS2DS12_ACC_R_SLEEP_STATE_IA(0,(LIS2DS12_ACC_SLEEP_STATE_IA_t*) &sleep_state_IA);
      if (sleep_state_IA == LIS2DS12_ACC_SLEEP_STATE_IA_EV_ON)
      {
        xl_status = SLEEP;
      }
    }
    
    //check when the status change in active
    if (xl_status == SLEEP)
    {
      if ( HAL_GPIO_ReadPin( INT1_GPIO_Port, INT1_Pin ) )
      {
        xl_status = ACTIVE;
      }
    }
  }
}
