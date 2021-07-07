#include "BSP.h"
#include "ButtonEvent/ButtonEvent.h"
#include "GUI/DisplayPrivate.h"

static ButtonEvent btPOWER;

static void Button_EventHander(ButtonEvent* btn, int event)
{
    if(btn == &btPOWER)
    {
        if(event == ButtonEvent::EVENT_ButtonClick)
        {
            page.EventTransmit(btn, LV_EVENT_LEAVE);
        }
        else if(event == ButtonEvent::EVENT_ButtonLongPressed)
        {
            Power_Shutdown();
        }
    }
}

void Button_Init()
{
    pinMode(KEY_Pin, INPUT_PULLUP);
    btPOWER.EventAttach(Button_EventHander);
}

static void Button_EventMonitor()
{
    btPOWER.EventMonitor(!digitalRead(KEY_Pin));
}

void Button_Update()
{
    __IntervalExecute(Button_EventMonitor(), 10);
}
