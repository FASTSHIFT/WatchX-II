#include "FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Wire.h"

static void setup()
{
    Serial.begin(115200);
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
