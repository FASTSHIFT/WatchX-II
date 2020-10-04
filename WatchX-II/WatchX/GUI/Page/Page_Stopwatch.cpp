#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(Stopwatch);

#define ANIM_TIME 300

extern "C" {
    LV_IMG_DECLARE(IMG_StopwatchAdd);
    LV_IMG_DECLARE(IMG_StopwatchPause);
    LV_IMG_DECLARE(IMG_StopwatchStart);
    LV_IMG_DECLARE(IMG_StopwatchReset);
}
static lv_obj_t* labelTime;
static lv_obj_t* listHistory;

typedef struct {
    const lv_img_dsc_t* img_sec;
    const lv_color_t color;
    lv_obj_t* btn;
    lv_obj_t* img;
}Btn_TypeDef;

typedef struct {
    lv_obj_t* list_btn;
    const char* text;
    const uint16_t btnVal;
}ListBtn_Typedef;

enum BtnGrpEnum
{
    BtnReset,
    BtnStart,
    BtnAdd
};

static Btn_TypeDef BtnGrp[] = {
    {&IMG_StopwatchReset   ,LV_COLOR_MAKE(0x00,0xFF,0xD7)},
    {&IMG_StopwatchStart   ,LV_COLOR_MAKE(0x46,0xF4,0xB0)},
    {&IMG_StopwatchAdd     ,LV_COLOR_MAKE(0x51,0xFF,0x9C)},
    
};

static const lv_coord_t Btn_Width = 70;
static const lv_coord_t Btn_Height = 45;

static lv_task_t* taskStopwatch = NULL;
static bool SW_IsPause = true;
static uint32_t SW_CurrentTime = 0;
static uint32_t SW_IntervalTime = 0;
static uint32_t SW_LastTime = 0;
static uint8_t  listBtnUnm = 0;

static void SW_ShowTime(uint32_t ms)
{
    uint16_t Min = 0, Sec = 0, Msec = 0;
    Min = ms / 60000;
    Sec = (ms - (Min * 60000)) / 1000;
    Msec = ms - (Sec * 1000) - (Min * 60000);
    lv_label_set_text_fmt(labelTime, "%02d:%02d.%03d", Min, Sec, Msec);
}

static void SW_Pause()
{
    if (SW_IsPause)
    {
        SW_IntervalTime = lv_tick_get() - SW_LastTime;
        lv_task_set_prio(taskStopwatch, LV_TASK_PRIO_MID);  
    }
    else
    {
        SW_LastTime = SW_CurrentTime;
        lv_task_set_prio(taskStopwatch, LV_TASK_PRIO_OFF); 
    }
    SW_IsPause = !SW_IsPause;
}

static void SW_Reset()
{
    SW_LastTime = 0;
    SW_CurrentTime = 0;
    SW_IntervalTime = 0;
    SW_IsPause = true;
    listBtnUnm = 0;
    lv_task_set_prio(taskStopwatch, LV_TASK_PRIO_OFF);
    SW_ShowTime(SW_CurrentTime);
    lv_list_clean(listHistory);
}

static void SW_Update(lv_task_t* task)
{
    SW_CurrentTime = lv_tick_get() - SW_IntervalTime;
    SW_ShowTime(SW_CurrentTime);
}

static void BtnGrp_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    
    if (event == LV_EVENT_CLICKED)
    {
        if (obj == BtnGrp[BtnReset].btn)
        {
            if (SW_IsPause)
            {
                SW_Reset();
                lv_obj_set_state(BtnGrp[BtnReset].btn, LV_STATE_DISABLED);
                listBtnUnm = 0;
            }
        }
        else if (obj == BtnGrp[BtnStart].btn)
        {
             SW_Pause();
             if (SW_IsPause)
             {
                 lv_img_set_src(BtnGrp[BtnStart].img, &IMG_StopwatchStart);
                 lv_obj_set_state(BtnGrp[BtnReset].btn, LV_STATE_DEFAULT);
             }
             else
             {
                 lv_img_set_src(BtnGrp[BtnStart].img, &IMG_StopwatchPause);
                 lv_obj_set_state(BtnGrp[BtnReset].btn, LV_STATE_DISABLED);
             }
        }
        else if (obj == BtnGrp[BtnAdd].btn)
        {
            lv_obj_t* list_btn;
            uint16_t Min = 0, Sec = 0, Msec = 0;
            
            
            if (listBtnUnm <10&&!SW_IsPause)
            {
                Min = SW_CurrentTime / 60000;
                Sec = (SW_CurrentTime - (Min * 60000)) / 1000;
                Msec = SW_CurrentTime - (Sec * 1000) - (Min * 60000);

                char buf[64];
                lv_snprintf(buf, sizeof(buf),"#666666 %d. #%02d:%02d.%03d", listBtnUnm+1, Min, Sec, Msec);
                
                list_btn = lv_list_add_btn(listHistory, NULL, buf);
                lv_obj_set_height(list_btn,40);

                lv_obj_t* label = lv_obj_get_child(list_btn, NULL);
                lv_label_set_recolor(label, true);

                lv_list_focus(list_btn, LV_ANIM_ON);
                listBtnUnm ++;
            }
        }
    }
}

static void LabelTime_Create(lv_obj_t* par)
{
    lv_obj_t* label = lv_label_create(par, NULL);

    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_50);
    lv_obj_set_style_local_text_color(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x87,0xFF,0xCE));

    lv_label_set_text(label,"00:00.000");
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    //lv_obj_set_auto_realign(label,true);

    labelTime = label;
}

static void ListHistory_Create(lv_obj_t* par)
{
    lv_obj_t* list = lv_list_create(par, NULL);

    lv_obj_set_style_local_text_font(list, LV_LIST_PART_BG, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
    lv_obj_set_style_local_border_width(list, LV_LIST_PART_BG, LV_STATE_DEFAULT,0);
    lv_obj_set_size(list, APP_WIN_WIDTH-40, 105);
    lv_obj_align(list, par, LV_ALIGN_CENTER, 0, 0);
    listHistory = list;
}

static void BtnGrp_Create(lv_obj_t* par)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, LV_STATE_DEFAULT, 5);
    lv_style_set_border_color(&style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&style, LV_STATE_DEFAULT, 2);

    for (int i = 0; i < __Sizeof(BtnGrp); i++)
    {
        lv_color_t color = BtnGrp[i].color;
        
        lv_obj_t* btn = lv_btn_create(par,NULL);
        lv_obj_set_size(btn, Btn_Width, Btn_Height);
        lv_obj_align(btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 15+(i * Btn_Width), -10);
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style);
        lv_obj_set_event_cb(btn,BtnGrp_EventHandler);
        lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, color);
        lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_GRAY);
        lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DISABLED, LV_COLOR_GRAY);

        lv_obj_t* img = lv_img_create(btn, NULL);
        lv_img_set_src(img,BtnGrp[i].img_sec);
        lv_obj_align(img, btn, LV_ALIGN_CENTER, 0, 0);

        BtnGrp[i].btn = btn;
        BtnGrp[i].img = img;
    }

    lv_obj_set_state(BtnGrp[BtnReset].btn, LV_STATE_DISABLED);
}

static void PagePlayAnim(bool open)
{
    if (open)
    {
        lv_coord_t y_target = lv_obj_get_y(labelTime);
        lv_obj_set_y(labelTime, -lv_obj_get_height(labelTime));
        LV_OBJ_ADD_ANIM(labelTime, y, y_target, ANIM_TIME);

        y_target = lv_obj_get_y(BtnGrp[0].btn);
        for (int i = 0; i < __Sizeof(BtnGrp); i++)
        {
            lv_obj_set_y(BtnGrp[i].btn, APP_WIN_HEIGHT + lv_obj_get_height(BtnGrp[i].btn));
            LV_OBJ_ADD_ANIM(BtnGrp[i].btn, y, y_target, ANIM_TIME);
        }
    }
    else
    {
        LV_OBJ_ADD_ANIM(BtnGrp[0].btn, y, APP_WIN_HEIGHT, ANIM_TIME);
        LV_OBJ_ADD_ANIM(BtnGrp[1].btn, y, APP_WIN_HEIGHT, ANIM_TIME);
        LV_OBJ_ADD_ANIM(BtnGrp[2].btn, y, APP_WIN_HEIGHT, ANIM_TIME);
        LV_OBJ_ADD_ANIM(labelTime, y, -lv_obj_get_height(labelTime), ANIM_TIME);
    }
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
    ListHistory_Create(appWindow);
    LabelTime_Create(appWindow);
    BtnGrp_Create(appWindow);
    taskStopwatch = lv_task_create(SW_Update, 51, LV_TASK_PRIO_OFF, NULL);
    PagePlayAnim(true);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    SW_Reset();
    lv_task_del(taskStopwatch);
    PagePlayAnim(false);
    PageDelay(ANIM_TIME);
    lv_obj_clean(appWindow);
}

/**
  * @brief  页面事件
  * @param  obj:发生事件的对象
  * @param  event:事件编号
  * @retval 无
  */
static void Event(void* obj, uint8_t event)
{
    if (obj == lv_scr_act())
    {
        if (event == LV_GESTURE_DIR_RIGHT)
        {
            Page->PagePop();
        }
    }
}
