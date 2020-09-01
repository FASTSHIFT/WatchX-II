/******************** (C) COPYRIGHT 2017 STMicroelectronics ********************
* File Name          : FFIFO_bypass_to_cont.c
* Author             : MEMS Application Team
* Version            : v1.0
* Date               : 22 Mar 2017 
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
#include "..\..\Acc\LIS2DH12_Driver_Example\trunk\LIS2DH12_Driver\LIS2DH12_ACC_driver.h"
#include "stm32f1xx_hal.h"

/* Private macro -------------------------------------------------------------*/
#define  TAP_THRESHOLD   9 // 1 LSb = FS / 32  ( @4g -> 4g / 32 = 0.125 g/LSb )
#define  TAP_SHOCK       3 // 1 LSb =  8 / ODR ( @400Hz ->  8/400 = 20 ms/LSb )
#define FIFO_LEVEL     128 // in case of module stored in FIFO 1 LSb =3 samples

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
static u8_t WhoAmI, rst;
//static u8_t ovr_FIFO_flag;
static u8_t level_fifo_flag; 
static u8_t ovr_FIFO_flag;
static u16_t number_of_sample;
Type3Axis16bit_U fifo_data[256];
TypeEvent_t TypeEvent;
AxisEvent_t AxisEvent;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Main */
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
  
  /* configure interrupt generator */
  /* TAP Configuration */
  /* Set Tap threshold  */
  LIS2DS12_ACC_W_TAP_THS(0, TAP_THRESHOLD);
  /* Set Tap shock  */
  LIS2DS12_ACC_W_SHOCK(0, TAP_SHOCK);
  /* Enable Tap axis */
  LIS2DS12_ACC_W_TAP_Z_EN(0, LIS2DS12_ACC_TAP_Z_EN_OFF);
  LIS2DS12_ACC_W_TAP_Y_EN(0, LIS2DS12_ACC_TAP_Y_EN_ON);
  LIS2DS12_ACC_W_TAP_X_EN(0, LIS2DS12_ACC_TAP_X_EN_OFF);
  /* TAP evaluation */
  LIS2DS12_ACC_W_SINGLE_DOUBLE_TAP(0, LIS2DS12_ACC_SINGLE_DOUBLE_TAP_EV_OFF);

  /* Configure FIFO */
  /* Enable Modue On FIFO*/
  /*Note: in case of module stored in FIFO 1LSb =3samples  */
  //response = LIS2DS12_ACC_W_MODULE_ON(0, LIS2DS12_ACC_MODULE_ON_ON);
  //if(response==MEMS_ERROR) while(1); //manage here comunication error
  //response = LIS2DS12_ACC_W_MODULE_TO_FIFO(0, LIS2DS12_ACC_MODULE_TO_FIFO_ON);
  //if(response==MEMS_ERROR) while(1); //manage here comunication error  
  /* Set FIFO threshold */
  response = LIS2DS12_ACC_W_FifoThsld(0, FIFO_LEVEL);
  if(response==MEMS_ERROR) while(1); //manage here comunication error  
  /* Set FIFO Mode */
  response = LIS2DS12_ACC_W_FMODE(0, LIS2DS12_ACC_FMODE_BYPASS_to_CONT);   
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* PIN Configuration */
  /* Tap on INT1 */
  LIS2DS12_ACC_W_INT1_D_TAP(0, LIS2DS12_ACC_INT1_D_TAP_OFF);
  LIS2DS12_ACC_W_INT1_S_TAP(0, LIS2DS12_ACC_INT1_S_TAP_ON); 
  /* Config FIFO threshold / Full on INT pin */
  //response = LIS2DS12_ACC_W_INT1_FIFO_FULL(0, LIS2DS12_ACC_INT1_FIFO_FULL_ON);
  LIS2DS12_ACC_W_INT2_FTH(0, LIS2DS12_ACC_INT2_FTH_ON);
  //response = LIS2DS12_ACC_W_INT1_FTH(0, LIS2DS12_ACC_INT1_FTH_ON);
  /* Set INT notification mode */
  response = LIS2DS12_ACC_W_LIR(0, LIS2DS12_ACC_LIR_ON);
  if(response==MEMS_ERROR) while(1); //manage here comunication error      
  /* Select interrupt pin polarity */
  response = LIS2DS12_ACC_W_H_LACTIVE(0, LIS2DS12_ACC_H_LACTIVE_high);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  /* Power on device */
  /* Set ACC full scale @ 4g*/
  response = LIS2DS12_ACC_W_FullScale(0, LIS2DS12_ACC_FS_4G);
  if(response==MEMS_ERROR) while(1); //manage here comunication error  
  /* Set ACC ODR  HR_14bit 100Hz*/
  response = LIS2DS12_ACC_W_ODR(0, LIS2DS12_ACC_ODR_LP_10bit_400Hz);
  if(response==MEMS_ERROR) while(1); //manage here comunication error
  
  while(1)
  {
    /* Choose one of the trigger below for download the FIFO data */  
    /* Trigger on threshold: register polling method */    
    LIS2DS12_ACC_R_FTH(0, (LIS2DS12_ACC_FTH_STATUS_t*) &level_fifo_flag);   
    if ( level_fifo_flag == LIS2DS12_ACC_FTH_EV_ON ) 
    /* Trigger on FIFO Full: register polling method */   
    //LIS2DS12_ACC_R_FIFO_OVR(0, (LIS2DS12_ACC_FIFO_OVR_t*) &ovr_FIFO_flag);
    //if ( ovr_FIFO_flag == LIS2DS12_ACC_FIFO_OVR_EV_ON )
    /* Trigger on INT PIN */  
    if ( HAL_GPIO_ReadPin( INT2_GPIO_Port, INT2_Pin ) )       
    {
        /* Check the samples available */
        /*Note: in case of module stored in FIFO 1LSb =3samples  */
        LIS2DS12_ACC_R_SamplesNum(0, &number_of_sample);
        /* Download FIFO */
        LIS2DS12_ACC_ReadReg(0, LIS2DS12_ACC_OUT_X_L, fifo_data[0].u8bit, 6*number_of_sample);  
    }
    
    /* Trigger on INT PIN */  
    if ( HAL_GPIO_ReadPin( INT1_GPIO_Port, INT1_Pin ) ) 
    { 
      /* Tap intterupt is detect */  
      /* FIFO automatically start in continuous mode */  
      /* FIFO stay in continuous mode till the relative source register is read */
      /* LIS2DS12_ACC_ReadReg(0, LIS2DS12_ACC_TAP_SRC, &TapStatus, 1); */
    }
  }
}
