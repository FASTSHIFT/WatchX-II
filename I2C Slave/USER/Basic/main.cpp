#include "FileGroup.h"
#include "Wire.h"

void setup()
{
    Serial.begin(115200);
    
    pinMode(LED_Pin, OUTPUT);
    
    ComSlave_Init();

    Wire.begin();
    I2C_Scan(true);
    
    TouchPad_Init();
    IMU_Init();
    ParticleSensor_Init();
}

void loop()
{
    __IntervalExecute(TouchPad_Update(), 20);
    __IntervalExecute(IMU_Update(), 100);
    __IntervalExecute(ParticleSensor_Update(), 50);

    ComSlave_Update();
    
    togglePin(LED_Pin);
}

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    GPIO_JTAG_Disable();
//  SysClock_Init(F_CPU_128MHz);
    Delay_Init();
    ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
