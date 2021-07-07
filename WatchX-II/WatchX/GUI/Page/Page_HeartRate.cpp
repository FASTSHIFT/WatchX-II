#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(HeartRate);

#define ANIM_TIME 300

#ifndef INT16_MAX
#  define INT16_MAX 32767
#endif

extern "C" {
    LV_IMG_DECLARE(IMG_HeartrateTinyheart);
    LV_IMG_DECLARE(IMG_StopwatchReset);
}

static const lv_coord_t Btn_Width = 70;
static const lv_coord_t Btn_Height = 45;

static lv_obj_t* labelCurrentHR;
static lv_obj_t* labelHRUnit;
static lv_obj_t* imgTinyHeart;

static lv_obj_t* labelHRRange;
static lv_task_t* taskLabelCurrentHR;

static int16_t HR_Beats_Min = INT16_MAX;
static int16_t HR_Beats_Max = 0;

static lv_obj_t* btnRst;
static lv_obj_t* chartHR;
static lv_chart_series_t* ChartHRSer1;

static void LabelCurrentHR_Update(lv_task_t* task)
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

    lv_label_set_text_fmt(labelCurrentHR, "%d", HR_Beats);
    lv_label_set_text_fmt(labelHRRange, "Range %d-%d", HR_Beats_Min, HR_Beats_Max);
    lv_chart_set_next(chartHR, ChartHRSer1, HR_Beats);
}

static void LabelCurrentHR_Create(lv_obj_t* par)
{
    lv_obj_t* label1 = lv_label_create(par,NULL);
    lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_50);
    lv_obj_set_style_local_text_color(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF,0x54,0x2A));
    lv_label_set_text(label1,"00");
    lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label1, NULL, LV_ALIGN_IN_TOP_LEFT, 15, 20);

    lv_obj_t* img = lv_img_create(par,NULL);
    lv_img_set_src(img,&IMG_HeartrateTinyheart);
    lv_obj_align(img,label1, LV_ALIGN_OUT_RIGHT_MID,5,-10);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, img);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_zoom);
    lv_anim_set_values(&a, LV_IMG_ZOOM_NONE, 160);
    lv_anim_set_time(&a, 300);
    lv_anim_set_playback_time(&a, 300);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 800);
    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, lv_anim_path_ease_out);
    lv_anim_set_path(&a, &path);
    lv_anim_start(&a);

    lv_obj_t* label2 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
    lv_obj_set_style_local_text_color(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_label_set_text(label2, "bpm");
    lv_obj_align(label2, label1, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, -5);

    taskLabelCurrentHR = lv_task_create(LabelCurrentHR_Update, 1000, LV_TASK_PRIO_MID, NULL);

    labelCurrentHR = label1;
    labelHRUnit = label2;
    imgTinyHeart = img;
}

static void BtnRst_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        HR_Beats_Min = INT16_MAX;
        HR_Beats_Max = 0;
    }
}

static void BtnRst_Create(lv_obj_t* par)
{
    lv_obj_t* btn = lv_btn_create(par ,NULL);
    lv_obj_set_size(btn, Btn_Width, Btn_Height);
    lv_obj_align(btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 15, -10);
    lv_obj_set_event_cb(btn, BtnRst_EventHandler);
    lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0x54, 0x2A));
    lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_MAKE(0xB0,0x31,0x00));
    lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DISABLED, LV_COLOR_MAKE(0xB0, 0x31, 0x00));
    lv_obj_set_style_local_radius(btn,LV_BTN_PART_MAIN,LV_STATE_DEFAULT,8);
    lv_obj_set_style_local_border_color(btn,LV_BTN_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);
    lv_obj_set_style_local_border_width(btn,LV_BTN_PART_MAIN,LV_STATE_DEFAULT,0);
    lv_obj_align(btn, labelCurrentHR,LV_ALIGN_OUT_RIGHT_MID,80,0);

    lv_obj_t* img = lv_img_create(btn, NULL);
    lv_img_set_src(img, &IMG_StopwatchReset);
    lv_obj_align(img, btn, LV_ALIGN_CENTER, 0, 0);

    btnRst = btn;
}

static void LabelHRRange_Create(lv_obj_t* par)
{
    lv_obj_t* label = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
    lv_obj_set_style_local_text_color(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_label_set_text(label, "Range -");
    lv_obj_align(label, labelCurrentHR, LV_ALIGN_OUT_BOTTOM_LEFT, 0,-10);

    labelHRRange = label;
}

static void ChartHR_Create(lv_obj_t* par)
{
    lv_obj_t* chart = lv_chart_create(par, NULL);
    lv_obj_set_size(chart, 220, 100);
    lv_obj_align(chart, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -20);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   
    lv_obj_set_style_local_bg_opa(chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_OPA_50);
    lv_obj_set_style_local_bg_grad_dir(chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_obj_set_style_local_bg_main_stop(chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 255);  
    lv_obj_set_style_local_bg_grad_stop(chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 0);    
    lv_obj_set_style_local_border_width(chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, 2);
    lv_obj_set_style_local_radius(chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, 5);
    lv_obj_set_style_local_border_color(chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x66, 0x66, 0x66));

    lv_chart_set_div_line_count(chart,0,4);
    lv_obj_set_style_local_line_color(chart, LV_CHART_PART_SERIES_BG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x66, 0x66, 0x66));

    lv_chart_set_y_range(chart,LV_CHART_AXIS_PRIMARY_Y,50, 180);
    lv_chart_set_point_count(chart, 100);

    ChartHRSer1 = lv_chart_add_series(chart, LV_COLOR_MAKE(0xfc,0x5c,0x00));

    chartHR = chart;
}


static void PagePlayAnim(bool playback = false)
{
#define ANIM_Y_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), -lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
#define ANIM_Y_REV_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), APP_WIN_HEIGHT+lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
    lv_anim_timeline_t anim_timeline[] = {
        ANIM_Y_DEF(100, labelCurrentHR),
        ANIM_Y_DEF(100, labelHRUnit),
        ANIM_Y_DEF(100, imgTinyHeart),
        ANIM_Y_DEF(100, btnRst),
        ANIM_Y_REV_DEF(100,labelHRRange),
        ANIM_Y_REV_DEF(120,chartHR),
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
    
    LabelCurrentHR_Create(appWindow);
    BtnRst_Create(appWindow);
    LabelHRRange_Create(appWindow);
    ChartHR_Create(appWindow);
    PagePlayAnim();
}

static void Exit()
{
    lv_task_del(taskLabelCurrentHR);
    PagePlayAnim(true);
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
    if (event == LV_EVENT_LEAVE)
    {
        Page->Pop();
    }
}
