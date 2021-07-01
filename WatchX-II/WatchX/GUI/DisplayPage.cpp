#include "GUI/DisplayPrivate.h"

/*ʵ����ҳ�������*/
static PageManager page(PAGE_MAX);

/*ҳ�浼��*/
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
        page.EventTransmit(obj, dir);
    }
}

/**
  * @brief  ҳ���ʼ��
  * @param  ��
  * @retval ��
  */
void DisplayPage_Init()
{
    PAGE_IMPORT(MainMenu);        //���˵�
    PAGE_IMPORT(DialPlate);       //����
    PAGE_IMPORT(Calculator);      //������
    PAGE_IMPORT(Stopwatch);       //���
    PAGE_IMPORT(HeartRate);       //����
    PAGE_IMPORT(Settings);        //����
    PAGE_IMPORT(TimeSet);         //ʱ������
    PAGE_IMPORT(BacklightSet);    //��������
    PAGE_IMPORT(Game);            //��Ϸ
    PAGE_IMPORT(Sleep);           //˯��
    PAGE_IMPORT(Music);     //���ֲ�����
    PAGE_IMPORT(Sport);     //�˶�
    //PAGE_IMPORT(Game2048);
    page.Push(PAGE_DialPlate);    //�����һ��ҳ��
    //page.Push(PAGE_Game2048);
    lv_obj_set_event_cb(lv_scr_act(), page_gestute_event_cb);
}

/**
  * @brief  ��ʾ����
  * @param  ��
  * @retval ��
  */
void Display_Update()
{
    page.Running();
    lv_task_handler();
}

/**
  * @brief  ҳ��������ʱ������lvgl����
  * @param  ��
  * @retval ��
  */
void PageDelay(uint32_t ms)
{
    uint32_t lastTime = lv_tick_get();
    
    while(lv_tick_elaps(lastTime) <= ms)
    {
        lv_task_handler();
    }
}
