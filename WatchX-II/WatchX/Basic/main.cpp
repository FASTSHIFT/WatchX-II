#include "FileGroup.h"
#include "GUI/DisplayPrivate.h"

static void setup()
{
    WatchX_Init();
    Display_Init();
}

static void loop()
{
    Display_Update();
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
    Delay_Init();
    //ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
