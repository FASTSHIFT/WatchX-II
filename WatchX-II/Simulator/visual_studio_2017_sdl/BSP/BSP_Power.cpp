#include "BSP/BSP.h"
#include "GUI/DisplayPrivate.h"

/*上一次操作时间(ms)*/
static uint32_t Power_LastHandleTime = 0;

/*自动关机时间(秒)*/
static uint16_t Power_AutoLowPowerTimeout = 60;

/*自动关机功能使能*/
static bool Power_AutoLowPowerEnable = true;

static uint16_t Power_ADCValue = 0;

/**
  * @brief  电源初始化
  * @param  无
  * @retval 无
  */
void Power_Init()
{
    Power_SetAutoLowPowerTimeout(5 * 60);
    Power_HandleTimeUpdate();
}

/**
  * @brief  更新操作时间
  * @param  无
  * @retval 无
  */
void Power_HandleTimeUpdate()
{
    Power_LastHandleTime = lv_tick_get();
}

/**
  * @brief  设置自动关机时间
  * @param  sec:时间(秒)
  * @retval 无
  */
void Power_SetAutoLowPowerTimeout(uint16_t sec)
{
    Power_AutoLowPowerTimeout = sec;
}

/**
  * @brief  获取自动关机时间
  * @param  无
  * @retval sec:时间(秒)
  */
uint16_t Power_GetAutoLowPowerTimeout()
{
    return Power_AutoLowPowerTimeout;
}

/**
  * @brief  设置自动关机功能使能
  * @param  en:使能
  * @retval 无
  */
void Power_SetAutoLowPowerEnable(bool en)
{
    Power_AutoLowPowerEnable = en;
    Power_HandleTimeUpdate();
}

/**
  * @brief  执行关机
  * @param  无
  * @retval 无
  */
void Power_Shutdown()
{
    Backlight_SetGradual(0, 500);
    PageDelay(500);
}

/**
  * @brief  自动关机监控
  * @param  无
  * @retval 无
  */
void Power_Update()
{
    Power_ADCValue = 4095 / 2;
    
    if(!Power_AutoLowPowerEnable)
        return;
    
    if(Power_AutoLowPowerTimeout == 0)
        return;
    
    if(lv_tick_elaps(Power_LastHandleTime)  >= (Power_AutoLowPowerTimeout * 1000))
    {
        Power_Shutdown();
    }
}

uint8_t Power_GetBattUsage()
{
    int voltage = __Map(
        Power_ADCValue,
        0, 4095,
        0, 3300
    );
    
    voltage *= 2;
    
    __LimitValue(voltage, 3000, 4100);
    
    int usage = __Map(
        voltage,
        3000, 4100,
        0, 100
    );
    
    return usage;
}

bool Power_GetBattIsCharging()
{
    return false;
}
