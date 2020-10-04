#include "FileGroup.h"

static void setup()
{
    WatchX_Init();
    vTaskStartScheduler();
}

static void loop()
{
}

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    GPIO_JTAG_Disable();
    //Delay_Init();
    ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
