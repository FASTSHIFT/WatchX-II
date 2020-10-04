#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

/**
  * @brief  变量初始化
  * @param  无
  * @retval true成功 false失败
  */
static bool Value_Init()
{
    return true;
}

/**
  * @brief  手表初始化
  * @param  无
  * @retval 无
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
    
    //I2C_Scan(true);
    I2Cx_Init(SENSOR_I2C, 100 * 1000);
    
    IMU_Init();
    
    RTC_Init();
    //RTC_Set(2020, 10, 2, 19, 13, 55);
    //SD_Init();
    
    Tasks_Init();
    
    Delay_Init();
    
    Serial.println("OS start scheduler...");
    Serial.print("\r\n+===================================================+\r\n");
}
