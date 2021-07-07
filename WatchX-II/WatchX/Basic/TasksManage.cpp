#include "Basic/TasksManage.h"
#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

static TaskHandle_t TaskHandle_Display;
static TaskHandle_t TaskHandle_Sensor;

static void Task_Display(void *pvParameters)
{
    Display_Init();
    for(;;)
    {
        Display_Update();
        Button_Update();
        vTaskDelay(10);
    }
}

static void Task_Sensor(void *pvParameters)
{
    for(;;)
    {
#if (WX_USE_BUILT_IN_I2C != 0)   
        TouchPad_Update();
        IMU_Update();
        //ParticleSensor_Update();
#else
        ComMaster_Update();
#endif
        //Button_Update();
        Power_Update();
        
        vTaskDelay(10);
    }
}

static void TimerTask_20msUpdate(TimerHandle_t xTimer)
{
    
}

void Tasks_Init()
{
    xTaskReg(Display, KByteToWord(16), WX_USE_BUILT_IN_I2C);
    xTaskReg(Sensor,  KByteToWord(2),  0);
    
    xTimerReg(TimerTask_20msUpdate, 20);
}
