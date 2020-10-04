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
        vTaskDelay(5);
    }
}

static void Task_Sensor(void *pvParameters)
{
    //IMU_Init();
    //ParticleSensor_Init();
    for(;;)
    {
        //TouchPad_Update();
        //ParticleSensor_Update();
        //IMU_Update();
        
        Button_Update();
        Power_Update();
        
        vTaskDelay(20);
    }
}

static void TimerTask_20msUpdate(TimerHandle_t xTimer)
{
    
}

void Tasks_Init()
{
    xTaskReg(Display, KByteToWord(16), 1);
    xTaskReg(Sensor,  KByteToWord(2),  0);
    
    xTimerReg(TimerTask_20msUpdate, 20);
}
