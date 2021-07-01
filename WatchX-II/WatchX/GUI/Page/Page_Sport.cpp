#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(Sport);
#define ANIM_TIME 300
#ifndef INT16_MAX
#  define INT16_MAX 32767
#endif
static const lv_coord_t Btn_Width = 70;
static const lv_coord_t Btn_Height = 45;
static lv_obj_t* stopWatchLabel;
static lv_obj_t* btn_begin;
static lv_obj_t* btn_begin_img;
static lv_obj_t* btn_stop;
static lv_obj_t* label_step;
static lv_obj_t* label_heartrate;
static lv_obj_t* chartHR;
static lv_chart_series_t* ChartHRSer1;
static lv_task_t* taskLabelCurrentHR;
static lv_obj_t* labelsCont;

static int16_t HR_Beats_Min = INT16_MAX;
static int16_t HR_Beats_Max = 0;

extern "C" {
    LV_IMG_DECLARE(IMG_HeartrateTinyheart);
    LV_IMG_DECLARE(IMG_StopwatchPause);
    LV_IMG_DECLARE(IMG_StopwatchStart);
    LV_IMG_DECLARE(IMG_Stop);
    LV_IMG_DECLARE(IMG_TinyStep);
}

static lv_task_t* taskStopwatch = NULL;
static bool SW_IsPause = true;
static uint32_t SW_CurrentTime = 0;
static uint32_t SW_IntervalTime = 0;
static uint32_t SW_LastTime = 0;
static uint8_t  HistoryRecCnt = 0;

static void SW_ShowTime(uint32_t ms)
{
    uint16_t Min = 0, Sec = 0, Hour;
    Min = ms / 60000;
    Sec = (ms - (Min * 60000)) / 1000;
    Hour = Min / 60;
    lv_label_set_text_fmt(stopWatchLabel, "%02d:%02d:%02d", Hour,Min, Sec);
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
}




static void LabelCurrentHR_Update()
{
    int16_t HR_Beats = ParticleSensor_GetBeats();

    if (HR_Beats < 1)
    {
        return;
    }

    if (HR_Beats < HR_Beats_Min)
    {
        HR_Beats_Min = HR_Beats;
    }
    if (HR_Beats > HR_Beats_Max)
    {
        HR_Beats_Max = HR_Beats;
    }

    lv_label_set_text_fmt(label_heartrate, "%d", HR_Beats);
    lv_chart_set_next(chartHR, ChartHRSer1, HR_Beats);
}

static void SW_Update(lv_task_t* task)
{
    SW_CurrentTime = lv_tick_get() - SW_IntervalTime;
    SW_ShowTime(SW_CurrentTime);
    LabelCurrentHR_Update();
    lv_label_set_text_fmt(label_step, "%05d", IMU_GetSteps());
}

static void Btns_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (obj == btn_stop)
        {
            if (SW_IsPause)
            {
                SW_Reset();
                lv_obj_set_state(btn_stop, LV_STATE_DISABLED);
            }
        }
        else if (obj == btn_begin)
        {
            SW_Pause();
            if (SW_IsPause)
            {
                lv_img_set_src(btn_begin_img, &IMG_StopwatchStart);
                lv_obj_set_state(btn_stop, LV_STATE_DEFAULT);
            }
            else
            {
                lv_img_set_src(btn_begin_img, &IMG_StopwatchPause);
                lv_obj_set_state(btn_stop, LV_STATE_DISABLED);
            }
        }
    }
}

static void StopWatchLabel_Create(lv_obj_t* par)
{
    lv_obj_t* label = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_50);
    lv_obj_set_style_local_text_color(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xa1, 0x77, 0xff));//a177ff
    lv_label_set_text(label, "00:00:00");
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);
    stopWatchLabel = label;
}

static void Btns_Create(lv_obj_t* par)
{
    lv_obj_t* btn1 = lv_btn_create(par, NULL);
    lv_obj_set_size(btn1, Btn_Width, Btn_Height);
    
    lv_obj_set_event_cb(btn1, Btns_EventHandler);
    lv_obj_set_style_local_bg_color(btn1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xa1, 0x77, 0xff));
    lv_obj_set_style_local_bg_color(btn1, LV_BTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_MAKE(0x4b, 0x00, 0xca));//4b00ca
    lv_obj_set_style_local_radius(btn1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);
    lv_obj_set_style_local_border_width(btn1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_align(btn1, NULL, LV_ALIGN_IN_BOTTOM_MID, Btn_Width/2+5, -10);

    lv_obj_t* img1 = lv_img_create(btn1, NULL);
    lv_img_set_src(img1, &IMG_StopwatchStart);
    lv_obj_align(img1, btn1, LV_ALIGN_CENTER, 0, 0);

    btn_begin = btn1;
    btn_begin_img = img1;
    lv_obj_t* btn2 = lv_btn_create(par, NULL);
    lv_obj_set_size(btn2, Btn_Width, Btn_Height);
    
    lv_obj_set_event_cb(btn2, Btns_EventHandler);
    lv_obj_set_style_local_bg_color(btn2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xa1, 0x77, 0xff));
    lv_obj_set_style_local_bg_color(btn2, LV_BTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_MAKE(0x4b, 0x00, 0xca));
    lv_obj_set_style_local_bg_color(btn2, LV_BTN_PART_MAIN, LV_STATE_DISABLED, LV_COLOR_MAKE(0x4b, 0x00, 0xca));
    lv_obj_set_style_local_radius(btn2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);
    lv_obj_set_style_local_border_width(btn2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_align(btn2, NULL, LV_ALIGN_IN_BOTTOM_MID, -Btn_Width/2-5, -10);

    lv_obj_t* img2 = lv_img_create(btn2, NULL);
    lv_img_set_src(img2, &IMG_Stop);
    lv_obj_align(img2, btn2, LV_ALIGN_CENTER, 0, 0);

    btn_stop = btn2;
}

static void StepAndHeartrateLabel_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_width(cont, APP_WIN_WIDTH);
    lv_obj_set_height(cont, 30);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_MID, 0, 80);

    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);

    lv_obj_t* label1 = lv_label_create(cont, NULL);
    lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
    lv_obj_set_style_local_text_color(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_label_set_text(label1, "--");
    lv_obj_align(label1, NULL , LV_ALIGN_IN_TOP_MID, 60,0);

    lv_obj_t* img1 = lv_img_create(cont,NULL);
    lv_img_set_src(img1, &IMG_HeartrateTinyheart);
    lv_obj_align(img1, label1, LV_ALIGN_OUT_LEFT_MID, 0, 0);

    lv_obj_t* label2 = lv_label_create(cont, NULL);
    lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
    lv_obj_set_style_local_text_color(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_label_set_text(label2, "--");
    lv_obj_align(label2, NULL, LV_ALIGN_IN_TOP_MID, -60, 0);

    lv_obj_t* img2 = lv_img_create(cont, NULL);
    lv_img_set_src(img2, &IMG_TinyStep);
    lv_obj_align(img2, label2, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    labelsCont = cont;
    label_heartrate = label1;
    label_step = label2;
}

static void ChartHR_Create(lv_obj_t* par)
{
    lv_obj_t* chart = lv_chart_create(par, NULL);
    lv_obj_set_size(chart, 200, 50);
    lv_obj_align(chart, NULL, LV_ALIGN_IN_TOP_MID, 0, 120);
    lv_chart_set_type(chart, LV_CHART_TYPE_COLUMN);
    lv_obj_set_style_local_border_width(chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, 2);
    lv_obj_set_style_local_radius(chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_border_color(chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x66, 0x66, 0x66));

    lv_chart_set_div_line_count(chart, 0, 4);
    lv_obj_set_style_local_line_color(chart, LV_CHART_PART_SERIES_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x66, 0x66, 0x66));

    lv_chart_set_y_range(chart, LV_CHART_AXIS_PRIMARY_Y, 50, 150);
    lv_chart_set_point_count(chart, 100);

    ChartHRSer1 = lv_chart_add_series(chart, LV_COLOR_MAKE(0xa1, 0x77, 0xff));//a177ff

    chartHR = chart;
}

static void PagePlayAnim(bool playback = false)
{
#define ANIM_Y_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), -lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
#define ANIM_Y_REV_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), APP_WIN_HEIGHT+lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}

    static lv_coord_t list_width_end = lv_obj_get_width(chartHR);
    static lv_coord_t list_height_end = lv_obj_get_height(chartHR);

    lv_anim_timeline_t anim_timeline[] = {
        ANIM_Y_DEF(0,stopWatchLabel),
        ANIM_Y_REV_DEF(0,btn_begin),
        ANIM_Y_REV_DEF(0,btn_stop),
        ANIM_Y_DEF(0,labelsCont),
        {100, chartHR, LV_ANIM_EXEC(width), 0, list_width_end, 200, lv_anim_path_ease_out},
        {300, chartHR, LV_ANIM_EXEC(height), 2, list_height_end, 200, lv_anim_path_ease_out},
    };

    uint32_t playtime = lv_anim_timeline_start(anim_timeline, __Sizeof(anim_timeline), playback);
    PageDelay(playtime);
    StatusBar_SetName(Page->GetCurrentName());
    StatusBar_SetEnable(!playback);
}

static void Setup()
{
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);
    StopWatchLabel_Create(appWindow);
    StepAndHeartrateLabel_Create(appWindow);
    Btns_Create(appWindow);
    ChartHR_Create(appWindow);
    taskStopwatch = lv_task_create(SW_Update, 500, (SW_IsPause ? LV_TASK_PRIO_OFF : LV_TASK_PRIO_MID), NULL);
    PagePlayAnim();
}

static void Exit()
{
    PagePlayAnim(true);
    lv_task_del(taskStopwatch);
    lv_obj_clean(appWindow);
}


static void Event(void* obj, uint8_t event)
{
    if (obj == lv_scr_act())
    {
        if (event == LV_GESTURE_DIR_RIGHT)
        {
            Page->Pop();
        }
    }
}
