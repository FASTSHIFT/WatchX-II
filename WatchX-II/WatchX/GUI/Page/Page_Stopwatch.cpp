#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(Stopwatch);

extern "C" {
    LV_IMG_DECLARE(IMG_StopwatchAdd);
    LV_IMG_DECLARE(IMG_StopwatchPause);
    LV_IMG_DECLARE(IMG_StopwatchStart);
    LV_IMG_DECLARE(IMG_StopwatchReset);
}
static lv_obj_t* labelTime;
static lv_obj_t* listHistory;

typedef struct
{
    const lv_img_dsc_t* img_sec;
    const lv_color_t color;
    lv_obj_t* btn;
    lv_obj_t* img;
} Btn_TypeDef;

typedef struct
{
    lv_obj_t* list_btn;
    const char* text;
    const uint16_t btnVal;
} ListBtn_Typedef;

enum BtnGrpEnum
{
    BtnReset,
    BtnStart,
    BtnAdd
};

static Btn_TypeDef BtnGrp[] =
{
    {&IMG_StopwatchReset, LV_COLOR_MAKE(0x00, 0xFF, 0xD7)},
    {&IMG_StopwatchStart, LV_COLOR_MAKE(0x46, 0xF4, 0xB0)},
    {&IMG_StopwatchAdd, LV_COLOR_MAKE(0x51, 0xFF, 0x9C)},
};

static const lv_coord_t Btn_Width = 70;
static const lv_coord_t Btn_Height = 45;

static lv_task_t* taskStopwatch = NULL;
static bool SW_IsPause = true;
static uint32_t SW_CurrentTime = 0;
static uint32_t SW_IntervalTime = 0;
static uint32_t SW_LastTime = 0;
static uint8_t  HistoryRecCnt = 0;

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
    HistoryRecCnt = 0;
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
                HistoryRecCnt = 0;
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
            lv_obj_t* label;
            uint16_t Min = 0, Sec = 0, Msec = 0;

            if (HistoryRecCnt < 10 && !SW_IsPause)
            {
                Min = SW_CurrentTime / 60000;
                Sec = (SW_CurrentTime - (Min * 60000)) / 1000;
                Msec = SW_CurrentTime - (Sec * 1000) - (Min * 60000);

                char buf[64];
                lv_snprintf(buf, sizeof(buf), "#87FFCE %d. #%02d:%02d.%03d", HistoryRecCnt + 1, Min, Sec, Msec);

                list_btn = lv_list_add_btn(listHistory, NULL, buf);
                label = lv_obj_get_child(list_btn, NULL);
                lv_label_set_recolor(label, true);
                lv_list_focus(list_btn, LV_ANIM_ON);
                lv_obj_set_style_local_text_decor(list_btn, LV_BTN_PART_MAIN, LV_STATE_FOCUSED, LV_TEXT_DECOR_NONE);
                HistoryRecCnt ++;
            }
        }
    }
}

static void LabelTime_Create(lv_obj_t* par)
{
    lv_obj_t* label = lv_label_create(par, NULL);

    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_50);
    lv_obj_set_style_local_text_color(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x87, 0xFF, 0xCE));

    lv_label_set_text(label, "00:00.000");
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    //lv_obj_set_auto_realign(label,true);


    labelTime = label;
}

static void ListHistory_Create(lv_obj_t* par)
{
    lv_obj_t* list = lv_list_create(par, NULL);

    lv_obj_set_style_local_text_font(list, LV_LIST_PART_BG, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
    lv_obj_set_style_local_border_width(list, LV_LIST_PART_BG, LV_STATE_DEFAULT, 2);
    lv_obj_set_style_local_radius(list, LV_LIST_PART_BG, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_border_color(list, LV_LIST_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x66, 0x66, 0x66));
    lv_obj_set_size(list, APP_WIN_WIDTH - 36, 105);
    lv_obj_align(list, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_auto_realign(list, true);
    listHistory = list;

}

static void BtnGrp_Create(lv_obj_t* par)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, LV_STATE_DEFAULT, 5);
    lv_style_set_border_color(&style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&style, LV_STATE_DEFAULT, 2);

    lv_style_set_bg_color(&style, LV_STATE_PRESSED, LV_COLOR_GRAY);
    lv_style_set_bg_color(&style, LV_STATE_DISABLED, LV_COLOR_GRAY);

    for (int i = 0; i < __Sizeof(BtnGrp); i++)
    {
        lv_color_t color = BtnGrp[i].color;

        lv_obj_t* btn = lv_btn_create(par, NULL);
        lv_obj_set_size(btn, Btn_Width, Btn_Height);
        lv_obj_align(btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 15 + (i * Btn_Width), -10);
        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style);
        lv_obj_set_event_cb(btn, BtnGrp_EventHandler);
        lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, color);

        lv_obj_t* img = lv_img_create(btn, NULL);
        lv_img_set_src(img, BtnGrp[i].img_sec);
        lv_obj_align(img, btn, LV_ALIGN_CENTER, 0, 0);

        BtnGrp[i].btn = btn;
        BtnGrp[i].img = img;
    }

    lv_obj_set_state(BtnGrp[BtnReset].btn, LV_STATE_DISABLED);
}

static void PagePlayAnim(bool playback = false)
{
    static lv_coord_t time_y_end = lv_obj_get_y(labelTime);
    static lv_coord_t btn_y_end = lv_obj_get_y(BtnGrp[0].btn);
    lv_coord_t btn_y_start = APP_WIN_HEIGHT + lv_obj_get_height(BtnGrp[0].btn);

    static lv_coord_t list_width_end = lv_obj_get_width(listHistory);
    static lv_coord_t list_height_end = lv_obj_get_height(listHistory);

    lv_anim_timeline_t anim_timeline[] =
    {
        {0, labelTime, LV_ANIM_EXEC(y), -lv_obj_get_height(labelTime), time_y_end, 300, lv_anim_path_ease_out},
        {0, labelTime, LV_ANIM_EXEC(opa_scale), LV_OPA_TRANSP, LV_OPA_COVER, 300, lv_anim_path_ease_out},

        {100, BtnGrp[0].btn, LV_ANIM_EXEC(y), btn_y_start, btn_y_end, 300, lv_anim_path_ease_out},
        {150, BtnGrp[1].btn, LV_ANIM_EXEC(y), btn_y_start, btn_y_end, 300, lv_anim_path_ease_out},
        {200, BtnGrp[2].btn, LV_ANIM_EXEC(y), btn_y_start, btn_y_end, 300, lv_anim_path_ease_out},

        {100, listHistory, LV_ANIM_EXEC(width), 0, list_width_end, 200, lv_anim_path_ease_out},
        {300, listHistory, LV_ANIM_EXEC(height), 2, list_height_end, 200, lv_anim_path_ease_out},
    };

    uint32_t playtime = lv_anim_timeline_start(anim_timeline, __Sizeof(anim_timeline), playback);
    PageDelay(playtime);
}

static void SetupOnce()
{
    ListHistory_Create(appWindow);
    LabelTime_Create(appWindow);
    BtnGrp_Create(appWindow);
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

    __ExecuteOnce(SetupOnce());

    taskStopwatch = lv_task_create(SW_Update, 51, (SW_IsPause ? LV_TASK_PRIO_OFF : LV_TASK_PRIO_MID), NULL);
    lv_obj_set_hidden(appWindow, false);
    PagePlayAnim();
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    PagePlayAnim(true);
    lv_task_del(taskStopwatch);
    lv_obj_set_hidden(appWindow, true);
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
        if (event == LV_GESTURE_DIR_LEFT || event == LV_GESTURE_DIR_RIGHT)
        {
            Page->Pop();
        }
    }
    if (event == LV_EVENT_LEAVE)
    {
        Page->Pop();
    }
}
