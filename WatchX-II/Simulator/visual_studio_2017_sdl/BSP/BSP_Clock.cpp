#include "BSP/BSP.h"
#include <Windows.h>

void Clock_GetValue(Clock_Value_t* clock_value)
{
    SYSTEMTIME sys;
    GetLocalTime(&sys);

    clock_value->year = sys.wYear;
    clock_value->month = sys.wMonth;
    clock_value->date = sys.wDay;
    clock_value->week = sys.wDayOfWeek;
    clock_value->hour = sys.wHour;
    clock_value->min = sys.wMinute;
    clock_value->sec = sys.wSecond;
    clock_value->ms = sys.wMilliseconds;
}

void Clock_SetValue(Clock_Value_t* clock_value)
{

}
