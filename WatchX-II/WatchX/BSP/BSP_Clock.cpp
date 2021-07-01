#include "BSP/BSP.h"
#include "rtc.h"

void Clock_GetValue(Clock_Value_t* clock_value)
{
    RTC_Get();
    clock_value->year = calendar.w_year;
    clock_value->month = calendar.w_month;
    clock_value->date = calendar.w_date;
    clock_value->week = calendar.week;
    clock_value->hour = calendar.hour;
    clock_value->min = calendar.min;
    clock_value->sec = calendar.sec;
    clock_value->ms = 0;
}

void Clock_SetValue(Clock_Value_t* clock_value)
{
    RTC_Set(
        clock_value->year,
        clock_value->month,
        clock_value->date,
        clock_value->hour,
        clock_value->min,
        clock_value->sec
    );
}
