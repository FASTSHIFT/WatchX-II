/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
* File Name          : TAP.c
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
#define  TAP_THRESHOLD   9 // 1 LSb = FS / 32  ( @4g -> 4g / 32 = 0.125 g/LSb )
#define  TAP_SHOCK       3 // 1 LSb =  8 / ODR ( @400Hz ->  8/400 = 20 ms/LSb )
#define  TAP_QUIET       2 // 1 LSb =  4 / ODR ( @400Hz ->  4/400 = 10 ms/LSb )
  
typedef enum {
 	X_EVENT, 		 
 	Y_EVENT, 		 
	Z_EVENT, 		 
} AxisEvent_t;

typedef enum {
	TYPE_SINGLE, 		 
	TYPE_DOUBLE     
} TypeEvent_t;

/* Private variables ---------------------------------------------------------*/
static status_t response;  
static u8_t WhoAmI, rst, TapStatus;
TypeEvent_t TypeEvent;
AxisEvent_t AxisEvent;

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
  
  /* TAP Configuration */
  /* Set Tap threshold  */
  LIS2DS12_ACC_W_TAP_THS(0, TAP_THRESHOLD);
  
  /* Set Tap Quiet - interrupt duration if LIR_OFF - */
  LIS2DS12_ACC_W_QUIET(0, TAP_QUIET);
  
  /* Set Tap shock  */
  LIS2DS12_ACC_W_SHOCK(0, TAP_SHOCK);
  
  /* Enable Tap axis */
  LIS2DS12_ACC_W_TAP_Z_EN(0, LIS2DS12_ACC_TAP_Z_EN_ON);
  LIS2DS12_ACC_W_TAP_Y_EN(0, LIS2DS12_ACC_TAP_Y_EN_ON);
  LIS2DS12_ACC_W_TAP_X_EN(0, LIS2DS12_ACC_TAP_X_EN_ON);
  
  /* TAP evaluation */
  LIS2DS12_ACC_W_SINGLE_DOUBLE_TAP(0, LIS2DS12_ACC_SINGLE_DOUBLE_TAP_EV_OFF);
  
  /* Tap on INT1 */
  LIS2DS12_ACC_W_INT1_D_TAP(0, LIS2DS12_ACC_INT1_D_TAP_OFF);
  LIS2DS12_ACC_W_INT1_S_TAP(0, LIS2DS12_ACC_INT1_S_TAP_ON); 
  
  /* PIN Configuration */
  /* Select interrupt pin polarity */
  response = LIS2DS12_ACC_W_H_LACTIVE(0, LIS2DS12_ACC_H_LACTIVE_high);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* Set INT notification mode */
  response = LIS2DS12_ACC_W_LIR(0, LIS2DS12_ACC_LIR_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* Powre on axl */
  /* Set ACC ODR  HR_14bit 100Hz*/
  response = LIS2DS12_ACC_W_ODR(0, LIS2DS12_ACC_ODR_LP_10bit_400Hz);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  while(1)
  {
    //Polling on int pin driven by Single TAP interrupt signal 
    if ( HAL_GPIO_ReadPin( INT1_GPIO_Port, INT1_Pin ) )
    {
      LIS2DS12_ACC_ReadReg(0, LIS2DS12_ACC_TAP_SRC, &TapStatus, 1);
      if ( TapStatus & LIS2DS12_ACC_TAP_IA_EV_ON )
      {
        if ( TapStatus & LIS2DS12_ACC_Z_TAP_EV_ON )
        {
          AxisEvent = Z_EVENT;
        }
        else if ( TapStatus & LIS2DS12_ACC_Y_TAP_EV_ON )    
        {
          AxisEvent = Y_EVENT;
        }
        else if ( TapStatus & LIS2DS12_ACC_X_TAP_EV_ON )   
        {
          AxisEvent = X_EVENT;
        }
        
        if ( TapStatus & LIS2DS12_ACC_DOUBLE_TAP_DUP2_EV_ON )  
        {
          TypeEvent = TYPE_DOUBLE;
        }
        else if ( TapStatus & LIS2DS12_ACC_SINGLE_TAP_DUP2_EV_ON )    
        {
          TypeEvent = TYPE_SINGLE;
        }
      }
    }
  }
}
