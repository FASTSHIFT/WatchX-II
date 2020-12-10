#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

/*导出页面接口*/
PAGE_EXPORT(DialPlate);

extern "C"{
    LV_IMG_DECLARE(IMG_Heart);
    LV_IMG_DECLARE(IMG_Power);
    LV_IMG_DECLARE(IMG_CHNSecond);
}

static lv_obj_t* contBatt;
static lv_obj_t* ledBattGrp[10];

static lv_obj_t* contDate;
static lv_obj_t* labelDate;
static lv_obj_t* labelWeek;

static lv_obj_t* contTime;
static lv_label_anim_effect_t labelTimeEffect[4];

static lv_obj_t* imgPower;
static lv_obj_t* ledSecGrp[2];
static lv_task_t* task[2];

static lv_obj_t* contHeartRate;
static lv_obj_t* labelHeartRate;

static lv_obj_t* contSteps;
static lv_obj_t* labelSteps;

static lv_obj_t* imgCHN;

static Clock_Value_t Clock;

static void ContBatt_UpdateBattUsage(uint8_t usage)
{
    int8_t maxIndexTarget = __Map(usage, 0, 100, 0, __Sizeof(ledBattGrp));  
    
    for(int i = 0; i < __Sizeof(ledBattGrp); i++)
    {
        lv_obj_t* led = ledBattGrp[i];
        
        (i < maxIndexTarget) ? lv_led_on(led) : lv_led_off(led);
    }
}

static void ContBatt_AnimCallback(void* obj, int16_t usage)
{
    ContBatt_UpdateBattUsage(usage);
}

static void ContBatt_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_size(cont, 222, 20);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
    lv_obj_set_style_local_bg_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    contBatt = cont;
    
    lv_obj_t* img = lv_img_create(cont, NULL);
    lv_img_set_src(img, &IMG_Power);
    lv_obj_align(img, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
    imgPower = img;
    
    const lv_coord_t led_w = (lv_obj_get_width(contBatt) - lv_obj_get_width(img)) / 10 - 2;
    const lv_coord_t led_h = lv_obj_get_height(contBatt);
    
    lv_obj_t* led = lv_led_create(contBatt, NULL);
    lv_obj_set_size(led, led_w, led_h);
    lv_obj_set_style_local_radius(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_align(led, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    ledBattGrp[0] = led;
    lv_led_off(led);
    
    for(int i = 1; i < __Sizeof(ledBattGrp); i++)
    {
        led = lv_led_create(contBatt, ledBattGrp[0]);
        lv_obj_align(led, ledBattGrp[i - 1], LV_ALIGN_OUT_LEFT_MID, -2, 0);
        lv_led_off(led);
        ledBattGrp[i] = led;
    }
}

static void ContWeek_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_size(cont, lv_obj_get_width(par) / 2, lv_obj_get_height(par));
    lv_obj_align(cont, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    lv_obj_set_style_local_radius(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    
    lv_obj_t* label = lv_label_create(cont, NULL);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_RexBold_28);
    lv_label_set_text(label, "SUN");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    labelWeek = label;
}

static void ContDate_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_size(cont, 222, 32);
    lv_obj_align(cont, contBatt, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_local_radius(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 2);
    contDate = cont;
    
    lv_obj_t* label = lv_label_create(cont, NULL);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_RexBold_28);
    lv_label_set_text(label, "00.00.00");
    lv_obj_align(label, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);
    labelDate = label;
    
    ContWeek_Create(contDate);
}

static void LabelDate_Update()
{
    lv_label_set_text_fmt(labelDate, "%02d.%02d.%02d", Clock.year % 100, Clock.month, Clock.date);
    
    const char* week_str[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    lv_label_set_text(labelWeek, week_str[Clock.week]);
}

static void LabelTime_Update(lv_anim_enable_t anim_enable = LV_ANIM_ON)
{
    Clock_GetValue(&Clock);
    
    /*分-个位*/
    lv_label_anim_effect_check_value(&labelTimeEffect[3], Clock.min % 10, anim_enable);
    /*分-十位*/
    lv_label_anim_effect_check_value(&labelTimeEffect[2], Clock.min / 10, anim_enable);
    
    /*时-个位*/
    lv_label_anim_effect_check_value(&labelTimeEffect[1], Clock.hour % 10, anim_enable);
    /*时-十位*/
    lv_label_anim_effect_check_value(&labelTimeEffect[0], Clock.hour / 10, anim_enable);
    
    lv_led_toggle(ledSecGrp[0]);
    lv_led_toggle(ledSecGrp[1]);
}

static void LabelTime_Create(lv_obj_t* par)
{
    const lv_coord_t x_mod[4] = {-70, -30, 30, 70};
    for(int i = 0; i < __Sizeof(labelTimeEffect); i++)
    {
        lv_obj_t * label = lv_label_create(par, NULL);
        lv_label_set_text(label, "0");
        lv_obj_align(label, NULL, LV_ALIGN_CENTER, x_mod[i], 0);
        lv_obj_set_auto_realign(label, true);
        
        lv_label_anim_effect_init(&labelTimeEffect[i], par, label);
    }
    
    lv_obj_t* led = lv_led_create(par, NULL);
    lv_obj_set_size(led, 8, 8);
    lv_obj_set_hidden(led, true);
    lv_obj_set_style_local_radius(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_align(led, NULL, LV_ALIGN_CENTER, 0, -15);
    ledSecGrp[0] = led;
    
    led = lv_led_create(par, led);
    lv_obj_align(led, NULL, LV_ALIGN_CENTER, 0, 15);
    ledSecGrp[1] = led;
}

static void ContTime_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    
    lv_obj_set_size(cont, 222, 93);
    lv_obj_align(cont, contDate, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    
    lv_obj_set_style_local_radius(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 2);
    lv_obj_set_style_local_text_font(cont, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_RexBold_89);
    lv_obj_set_style_local_text_color(cont, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    
    contTime = cont;
    
    LabelTime_Create(contTime);
}

static void LabelHeartRate_Update()
{
    lv_label_set_text_fmt(labelHeartRate, "%04.01f", ParticleSensor_GetBeats());
}

static void ContHeartRate_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_size(cont, 150, 32);
    lv_obj_align(cont, contTime, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    contHeartRate = cont;
    
    cont = lv_cont_create(contHeartRate, NULL);
    lv_obj_set_size(cont, lv_obj_get_width(contHeartRate) / 3, lv_obj_get_height(contHeartRate));
    lv_obj_align(cont, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_style_local_bg_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    
    lv_obj_t* label = lv_label_create(cont, NULL);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_RexBold_28);
    lv_label_set_text(label, "HRT");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    
    label = lv_label_create(contHeartRate, NULL);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_RexBold_28);
    lv_label_set_text(label, "00.0");
    lv_obj_align(label, cont, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    labelHeartRate = label;
    
    cont = lv_cont_create(contHeartRate, NULL);
    lv_obj_set_size(cont, lv_obj_get_width(contHeartRate) / 4, lv_obj_get_height(contHeartRate));
    lv_obj_align(cont, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    lv_obj_set_style_local_bg_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_t* img = lv_img_create(cont, NULL);
    lv_img_set_src(img, &IMG_Heart);
    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
    
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
}

static void LabelSteps_Update()
{
    lv_label_set_text_fmt(labelSteps, "%05d", IMU_GetSteps());
}

static void ContSteps_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_size(cont, 150, 32);
    lv_obj_align(cont, contHeartRate, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    contSteps = cont;
    
    cont = lv_cont_create(contSteps, NULL);
    lv_obj_set_size(cont, lv_obj_get_width(contSteps) / 3, lv_obj_get_height(contSteps));
    lv_obj_align(cont, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_style_local_bg_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    
    lv_obj_t* label = lv_label_create(cont, NULL);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_RexBold_28);
    lv_label_set_text(label, "STP");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    
    label = lv_label_create(contSteps, NULL);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_RexBold_28);
    lv_label_set_text(label, "00000");
    lv_obj_align(label, cont, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    labelSteps = label;
}

static void ImgCHN_Create(lv_obj_t* par)
{   
    lv_obj_t* img = lv_img_create(par, NULL);
    lv_img_set_src(img, &IMG_CHNSecond);
    lv_obj_align(img, contTime, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 5);
    imgCHN = img;
}

static void PagePlayAnim(bool playback = false)
{
#define ANIM_WIDTH_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(width), 0,  lv_obj_get_width(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
#define ANIM_OPA_DEF(start_time, obj)   {start_time, obj, LV_ANIM_EXEC(opa_scale), LV_OPA_TRANSP, LV_OPA_COVER, 500, lv_anim_path_bounce}

    lv_anim_timeline_t anim_timeline[] = {
        ANIM_WIDTH_DEF(0,   contBatt),
        ANIM_WIDTH_DEF(100, contDate),
        ANIM_WIDTH_DEF(200, contTime),
        ANIM_WIDTH_DEF(300, contHeartRate),
        ANIM_WIDTH_DEF(400, contSteps),

        {400, NULL, (lv_anim_exec_xcb_t)ContBatt_AnimCallback, 0, Power_GetBattUsage(), 400, lv_anim_path_linear},

        ANIM_OPA_DEF(800, labelTimeEffect[0].label_1),
        ANIM_OPA_DEF(800, labelTimeEffect[1].label_1),
        ANIM_OPA_DEF(800, labelTimeEffect[2].label_1),
        ANIM_OPA_DEF(800, labelTimeEffect[3].label_1),
        ANIM_OPA_DEF(800, labelTimeEffect[0].label_2),
        ANIM_OPA_DEF(800, labelTimeEffect[1].label_2),
        ANIM_OPA_DEF(800, labelTimeEffect[2].label_2),
        ANIM_OPA_DEF(800, labelTimeEffect[3].label_2),
        ANIM_OPA_DEF(800, labelDate),
        ANIM_OPA_DEF(800, labelHeartRate),
        ANIM_OPA_DEF(800, labelSteps),
        ANIM_OPA_DEF(800, imgCHN),
    };

    uint32_t playtime = lv_anim_timeline_start(anim_timeline, __Sizeof(anim_timeline), playback);
    PageDelay(playtime);
}

static void Task_1000msUpdate(lv_task_t* task)
{
    LabelDate_Update();
    LabelSteps_Update();
    LabelHeartRate_Update();
    
    if(Power_GetBattIsCharging())
    {
        lv_obj_set_hidden(imgPower, !lv_obj_get_hidden(imgPower));
    }
    else
    {
        lv_obj_set_hidden(imgPower, false);
    }
}

static void Task_500msUpdate(lv_task_t* task)
{
    LabelTime_Update();
    ContBatt_UpdateBattUsage(Power_GetBattUsage());
}


static void Tasks_Create()
{
    task[0] = lv_task_create(Task_500msUpdate, 500, LV_TASK_PRIO_MID, NULL);
    Task_500msUpdate(task[0]);
    
    task[1] = lv_task_create(Task_1000msUpdate, 1000, LV_TASK_PRIO_MID, NULL);
    Task_1000msUpdate(task[1]);
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
    
    ContBatt_Create(appWindow);
    ContDate_Create(appWindow);
    ContTime_Create(appWindow);
    
    ContHeartRate_Create(appWindow);
    
    ContSteps_Create(appWindow);
    
    ImgCHN_Create(appWindow);
    
    LabelTime_Update(LV_ANIM_OFF);
    LabelDate_Update();
    LabelSteps_Update();
    
    PagePlayAnim();
    
    lv_obj_set_hidden(ledSecGrp[0], false);
    lv_obj_set_hidden(ledSecGrp[1], false);
    
    Tasks_Create();
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    lv_obj_set_hidden(ledSecGrp[0], true);
    lv_obj_set_hidden(ledSecGrp[1], true);
    
    lv_task_del(task[0]);
    lv_task_del(task[1]);
    
    PagePlayAnim(true);
    
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
    if(obj == lv_scr_act())
    {
        if(event == LV_GESTURE_DIR_LEFT)
        {
            Page->Push(PAGE_MainMenu);
        }
    }
}
