#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

/*此页面窗口*/
static lv_obj_t* appWindow;

static lv_obj_t* labelTime;
static lv_task_t* taskTimeUpdate;

static void LabelTime_Update(lv_task_t* task)
{
    RTC_Get();
    lv_label_set_text_fmt(labelTime, "%02d:%02d:%02d", calendar.hour, calendar.min, calendar.sec);
}

static void LabelTime_Creat(lv_obj_t* par)
{
    lv_obj_t* label = lv_label_create(par, NULL);
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_auto_realign(label, true);
    
    lv_task_t* task = lv_task_create(LabelTime_Update, 500, LV_TASK_PRIO_MID, NULL);
    
    labelTime = label;
    taskTimeUpdate = task;
}

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup()
{
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);
    
    LabelTime_Creat(appWindow);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    lv_task_del(taskTimeUpdate);
    
    lv_obj_clean(appWindow);
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

/**
  * @brief  页面注册
  * @param  pageID:为此页面分配的ID号
  * @retval 无
  */
void PageRegister_MainMenu(uint8_t pageID)
{
    /*获取分配给此页面的窗口*/
    appWindow = AppWindow_GetCont(pageID);
    
    /*注册至页面调度器*/
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
