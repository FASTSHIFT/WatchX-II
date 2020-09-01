/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
* File Name          : Pedometer.c
* Author             : MEMS Application Team
* Version            : v1.0
* Date               : 15 Mar 2017  
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
#define  DELTA_TIME_STEPS   0 // 1 LSb = 1.6384 sec (0 mean notify every step)
#define  DEBOUNCE_STEPS     2 // 1 LSb = 1 step (3rd steps will be notify) 
#define  RST_DEBOUNCE_TIME 13 // 1 LSb = 80 ms
#define  MIN_THRESHOLD      8 // 1 LSb = FS / 64 (FS: 2g or 4g)
#define  MAX_STEP_NUMBER  100 // 1 step
  

/* Private variables ---------------------------------------------------------*/
static status_t response;  
static u8_t WhoAmI, rst, stepDetect, dummy;
Type1Axis16bit_U number_of_steps;

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
  
  /* Advanced pedometer configurations*/
  /* Enable advanced configuration registers access */
  LIS2DS12_ACC_W_FUNC_CFG(0, LIS2DS12_ACC_FUNC_CFG_EN);
  /* configure advanced function */
  dummy = DELTA_TIME_STEPS;
  LIS2DS12_ACC_WriteReg(0, 0x3A, &dummy, 1); //STEP_COUNT_DELTA 
  dummy = ((RST_DEBOUNCE_TIME & 0x1F) << 3) | (DEBOUNCE_STEPS & 0x07);
  LIS2DS12_ACC_WriteReg(0, 0x2B, &dummy, 1); //PEDO_DEB_REG
  /* Disable advanced configuration registers access */  
  LIS2DS12_ACC_ReadReg(0, 0x3F, &dummy, 1);
  dummy &= ~0x10; 
  LIS2DS12_ACC_WriteReg(0, 0x3F, &dummy, 1);
  /* Select minimum threshold */
  response = LIS2DS12_ACC_W_SC_MTHS(0, MIN_THRESHOLD); 
  if(response==MEMS_ERROR) while(1); //manage here comunication error   
  
  /* Basic pedometer configurations*/
  /* Set ACC full scale @ 4g*/
  response = LIS2DS12_ACC_W_FullScale(0, LIS2DS12_ACC_FS_4G);
  if(response==MEMS_ERROR) while(1); //manage here comunication error  
  /* Select Pedometer full scale @ 4g*/
  response = LIS2DS12_ACC_W_PEDO4G(0, LIS2DS12_ACC_PEDO4G_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error  
  /* Enable step counter */
  response = LIS2DS12_ACC_W_STEP_CNT_ON(0, LIS2DS12_ACC_STEP_CNT_ON_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error 
  
  /* PIN Configuration */
  /* Enable step detection on INT2 pin */
  response = LIS2DS12_ACC_W_INT2_STEP_DET(0, LIS2DS12_ACC_INT2_STEP_DET_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error 
  /* Set INT notification mode */
  response = LIS2DS12_ACC_W_LIR(0, LIS2DS12_ACC_LIR_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* Powre on axl */
  /* Set ACC ODR  HR_14bit 100Hz (pedometer always work at 25Hz)*/
  response = LIS2DS12_ACC_W_ODR(0, LIS2DS12_ACC_ODR_HR_14bit_100Hz);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  while(1)
  {
    /* Polling on int pin */
    if (HAL_GPIO_ReadPin(INT2_GPIO_Port, INT2_Pin))
    {
      /* Check for a step event */  
      LIS2DS12_ACC_R_STEP_DETECT(0, (LIS2DS12_ACC_STEP_DETECT_t*) &stepDetect);
      if ( stepDetect == LIS2DS12_ACC_STEP_DETECT_EV_ON)
      {
        /* Read the step number */  
        LIS2DS12_ACC_Get_StepCounter(0, number_of_steps.u8bit); 
        if ( number_of_steps.i16bit == MAX_STEP_NUMBER )
        {
          /* Reset the pedometer counter */
          LIS2DS12_ACC_W_RST_NSTEP(0, LIS2DS12_ACC_RST_NSTEP_ON);  
        }
      }
    }
  }
}

