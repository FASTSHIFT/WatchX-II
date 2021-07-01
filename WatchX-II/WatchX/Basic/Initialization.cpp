#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

/**
  * @brief  ������ʼ��
  * @param  ��
  * @retval true�ɹ� falseʧ��
  */
static bool Value_Init()
{
    return true;
}

/**
  * @brief  �ֱ��ʼ��
  * @param  ��
  * @retval ��
  */
void WatchX_Init()
{
    Serial.begin(115200);
    Serial.println("\r\n+=================== WatchX OS =====================+\r\n");
    Serial.println("@FreeRTOS Kernel "tskKERNEL_VERSION_NUMBER);
    Serial.println(
        "-Hardware:"WX_HARDWARE_VERSION"\r\n"
        "-Software:"WX_SOFTWARE_VERSION"\r\n"
    );
    Serial.print(WX_NAME"("__DATE__" "__TIME__" Build) init...\r\n");
    
    Value_Init();
    Power_Init();
    Button_Init();

#if (WX_USE_BUILT_IN_I2C != 0)
    extern void I2C_Scan(bool startScan);
    I2C_Scan(true);
    I2Cx_Init(SENSOR_I2C, 100 * 1000);
#else
    ComMaster_Init();
#endif
    TouchPad_Init();
    IMU_Init();
    ParticleSensor_Init();
    
    ADC_DMA_Init();
    
    RTC_Init();
    //RTC_Set(2020, 10, 2, 19, 13, 55);
    //SD_Init();
    
    Tasks_Init();
   
    Serial.println("OS start scheduler...");
    Serial.print("\r\n+===================================================+\r\n");
    
    Delay_Init();
    vTaskStartScheduler();
}
