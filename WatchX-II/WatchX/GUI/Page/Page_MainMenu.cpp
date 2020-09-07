#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

/*此页面窗口*/
static lv_obj_t* appWindow;

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup()
{
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);
    
    
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    
}

/**
  * @brief  页面事件
  * @param  btn:发出事件的按键
  * @param  event:事件编号
  * @retval 无
  */
static void Event(void* btn, int event)
{
}

PAGE_EXPORT(MainMenu);
