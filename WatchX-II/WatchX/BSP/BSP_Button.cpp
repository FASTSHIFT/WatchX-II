#include "BSP.h"
#include "ButtonEvent/ButtonEvent.h"

static ButtonEvent btPOWER;

static void Button_EventHander(ButtonEvent* btn, int event)
{
    if(btn == &btPOWER)
    {
        if(event == ButtonEvent::EVENT_ButtonLongPressed)
        {
            Power_Shutdown();
        }
    }
}

void Button_Init()
{
    pinMode(KEY_Pin, INPUT);
    btPOWER.EventAttach(Button_EventHander);
}

static void Button_EventMonitor()
{
    btPOWER.EventMonitor(digitalRead(KEY_Pin));
}

void Button_Update()
{
    __IntervalExecute(Button_EventMonitor(), 10);
}
