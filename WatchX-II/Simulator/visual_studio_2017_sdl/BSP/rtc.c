/**
  ******************************************************************************
  * File   : RTC/Calendar/rtc.h
  * Version: V1.2.3
  * Date   : 2020-08-15
  * Brief  : This file provides template for calendar API.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#include <windows.h>

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_Calendar
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};            //Monthly correction data sheet.
const uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //Month data table of Pingnian
_calendar_obj calendar;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  RTC Init.
  * @param  None
  * @retval None
  */
void RTC_Init(void)
{
    
}

/**
  * @brief  Judeg the Leap year or Pingnian.
  *         Month      1  2  3  4  5  6  7  8  9  10 11 12
  *         Leap year  31 29 31 30 31 30 31 31 30 31 30 31
  *         Pingnian   31 28 31 30 31 30 31 31 30 31 30 31
  * @param  year
  * @retval 1: Leap year
            2: Pingnian
  */
uint8_t Is_Leap_Year(uint16_t year)
{
    if(year % 4 == 0)
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0) return 1;
            else return 0;
        }
        else return 1;
    }
    else return 0;
}

/**
  * @brief  Set time. Convert the input clock to a second.
  *         The time basic : 1970.1.1
  *         legitimate year: 1970 ~ 2099
  * @param  syear: Year
  *         smon : Month
  *         sday : Day
  *         hour
  *         min
  *         sec
  * @retval 0: Set time right.
  *         1: Set time failed.
  */
uint8_t RTC_Set(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{

    return 0;
}

/**
  * @brief  Set RTC alarm.
  *         The time basic : 1970.1.1
  *         legitimate year: 1970 ~ 2099
  * @param  syear : Year
  *         smon : Month
  *         sday : Day
  *         hour
  *         min
  *         sec
  * @retval 0: Set Alarm right.
  *         1: Set Alarm failed.
  */
uint8_t RTC_Alarm_Set(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{

    return 0;
}

/**
  * @brief  Get current time.
  * @param  None.
  * @retval None.
  */
void RTC_Get(void)
{
    SYSTEMTIME sys;
    GetLocalTime(&sys);

    calendar.w_year = sys.wYear;
    calendar.w_month = sys.wMonth;
    calendar.w_date = sys.wDay;
    calendar.week = sys.wDayOfWeek;
    calendar.hour = sys.wHour;
    calendar.min = sys.wMinute;
    calendar.sec = sys.wSecond;
}

/**
  * @brief  Get current week by Input Leap year\mouth\day.
  * @param  syear : Year
  *         smon  : Month
  *         sday  : Day
  * @retval week number.
  */
uint8_t RTC_Get_Week(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t temp2;
    uint8_t yearH, yearL;

    yearH = year / 100;
    yearL = year % 100;
    if (yearH > 19)yearL += 100;
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];
    if (yearL % 4 == 0 && month < 3)
        temp2--;
    return(temp2 % 7);
}

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
