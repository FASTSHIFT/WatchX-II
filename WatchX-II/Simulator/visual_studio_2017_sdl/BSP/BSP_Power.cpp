#include "BSP/BSP.h"
#include "GUI/DisplayPrivate.h"

/*��һ�β���ʱ��(ms)*/
static uint32_t Power_LastHandleTime = 0;

/*�Զ��ػ�ʱ��(��)*/
static uint16_t Power_AutoLowPowerTimeout = 60;

/*�Զ��ػ�����ʹ��*/
static bool Power_AutoLowPowerEnable = true;

static uint16_t Power_ADCValue = 3000;

/**
  * @brief  ��Դ��ʼ��
  * @param  ��
  * @retval ��
  */
void Power_Init()
{
    Power_SetAutoLowPowerTimeout(5 * 60);
    Power_HandleTimeUpdate();
}

/**
  * @brief  ���²���ʱ��
  * @param  ��
  * @retval ��
  */
void Power_HandleTimeUpdate()
{
    Power_LastHandleTime = lv_tick_get();
}

/**
  * @brief  �����Զ��ػ�ʱ��
  * @param  sec:ʱ��(��)
  * @retval ��
  */
void Power_SetAutoLowPowerTimeout(uint16_t sec)
{
    Power_AutoLowPowerTimeout = sec;
}

/**
  * @brief  ��ȡ�Զ��ػ�ʱ��
  * @param  ��
  * @retval sec:ʱ��(��)
  */
uint16_t Power_GetAutoLowPowerTimeout()
{
    return Power_AutoLowPowerTimeout;
}

/**
  * @brief  �����Զ��ػ�����ʹ��
  * @param  en:ʹ��
  * @retval ��
  */
void Power_SetAutoLowPowerEnable(bool en)
{
    Power_AutoLowPowerEnable = en;
    Power_HandleTimeUpdate();
}

/**
  * @brief  ִ�йػ�
  * @param  ��
  * @retval ��
  */
void Power_Shutdown()
{
    Backlight_SetGradual(0, 500);
    PageDelay(500);
}

/**
  * @brief  �Զ��ػ����
  * @param  ��
  * @retval ��
  */
void Power_Update()
{    
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
    /*int voltage = __Map(
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
    );*/
    
    return 100;
}

bool Power_GetBattIsCharging()
{
    return false;
}
