#ifndef __TASKSMANAGE_H
#define __TASKSMANAGE_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"
#include "cpu_utils.h"

/*Ext Function*/
#define TSK_DEBUG_PRINTF(format, ...) Serial.printf(format, ##__VA_ARGS__)

#define KByteToWord(kb)                      ((int)(kb * 1024 / 4))
#define uxTaskGetFreeStackByte(TaskHandle_x) (uxTaskGetStackHighWaterMark(TaskHandle_x)*4)


#define xTaskReg(task,stack,priority) \
do{\
    xTaskCreate(Task_##task,#task,stack,NULL,priority,&TaskHandle_##task);\
    TSK_DEBUG_PRINTF("Create: task:%s, stack:%d, priority:%d\r\n",#task,stack,priority);\
}while(0)

#define xTimerReg(func,time)\
do{\
    TimerHandle_t xTimer = xTimerCreate(#func,time,pdTRUE,0,func);\
    if(xTimer)xTimerStart(xTimer,0);\
    TSK_DEBUG_PRINTF("Create: timer:%s, time:%d\r\n",#func,time);\
}while(0)

void Tasks_Init();

/*Task Functions*/

/*TaskHandle*/

/*Timer Functions*/

/*TimerHandle*/

/*QueueHandle*/

#endif
