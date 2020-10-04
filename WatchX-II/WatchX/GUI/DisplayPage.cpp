#include "GUI/DisplayPrivate.h"

/*实例化页面调度器*/
static PageManager page(PAGE_MAX);

/*页面导入*/
#define PAGE_IMPORT(name)\
do{\
    extern void PageRegister_##name(PageManager* page_p, uint8_t pageID);\
    PageRegister_##name(&page, PAGE_##name);\
}while(0)

static void page_gestute_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_GESTURE)
    {
        lv_gesture_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        page.PageEventTransmit(obj, dir);
    }
}

/**
  * @brief  页面初始化
  * @param  无
  * @retval 无
  */
void DisplayPage_Init()
{
    PAGE_IMPORT(MainMenu);        //主菜单
    PAGE_IMPORT(DialPlate);       //表盘
    PAGE_IMPORT(Calculator);      //计算器
    PAGE_IMPORT(Stopwatch);       //秒表
    
    page.PagePush(PAGE_DialPlate);   //进入第一个页面
    
    lv_obj_set_event_cb(lv_scr_act(), page_gestute_event_cb);
}

/**
  * @brief  显示更新
  * @param  无
  * @retval 无
  */
void Display_Update()
{
    lv_task_handler();
    page.Running();
}

/**
  * @brief  页面阻塞延时，保持lvgl更新
  * @param  无
  * @retval 无
  */
void PageDelay(uint32_t ms)
{
    uint32_t lastTime = lv_tick_get();
    while(lv_tick_get() - lastTime <= ms)
    {
        lv_task_handler();
    }
}
