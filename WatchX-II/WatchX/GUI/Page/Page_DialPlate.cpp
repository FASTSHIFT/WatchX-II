#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(DialPlate);

static lv_obj_t* contBatt;
static lv_obj_t* ledBattGrp[10];

static lv_obj_t* contDate;
static lv_obj_t* labelDate;
static lv_obj_t* labelWeek;

static lv_obj_t* contTime;

static lv_label_anim_effect_t labelTimeEffect[4];
static lv_obj_t* ledSecGrp[2];
static lv_task_t* task[2];

static lv_obj_t* contHeartRate;
static lv_obj_t* labelHeartRate;

static lv_obj_t* contSteps;
static lv_obj_t* labelSteps;

static lv_obj_t* imgCHN;

static void ContBatt_Creat(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_size(cont, 222, 20);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
//    lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_RIGHT, -(APP_WIN_WIDTH - lv_obj_get_width(cont)) / 2, 5);
//    lv_obj_set_auto_realign(cont, true);
    lv_obj_set_style_local_bg_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    contBatt = cont;
    
    LV_IMG_DECLARE(IMG_Power);
    lv_obj_t* img = lv_img_create(cont, NULL);
    lv_img_set_src(img, &IMG_Power);
    lv_obj_align(img, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
    
    const lv_coord_t led_w = (lv_obj_get_width(contBatt) - lv_obj_get_width(img)) / 10 - 2;
    const lv_coord_t led_h = lv_obj_get_height(contBatt);
    
    lv_obj_t* led = lv_led_create(contBatt, NULL);
    lv_obj_set_size(led, led_w, led_h);
    lv_obj_set_style_local_radius(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_align(led, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    ledBattGrp[0] = led;
    
    for(int i = 1; i < __Sizeof(ledBattGrp); i++)
    {
        led = lv_led_create(contBatt, ledBattGrp[0]);
        lv_obj_align(led, ledBattGrp[i - 1], LV_ALIGN_OUT_LEFT_MID, -2, 0);
        ledBattGrp[i] = led;
        
        (i < 5) ? lv_led_on(led) : lv_led_off(led);
    }
}

static void ContWeek_Creat(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_size(cont, lv_obj_get_width(par) / 2, lv_obj_get_height(par));
    lv_obj_align(cont, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    lv_obj_set_style_local_radius(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    
    lv_obj_t* label = lv_label_create(cont, NULL);
    lv_label_set_text(label, "SUN");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    labelWeek = label;
}

static void ContDate_Creat(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_size(cont, 222, 32);
    lv_obj_align(cont, contBatt, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_style_local_radius(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 2);
    contDate = cont;
    
    lv_obj_t* label = lv_label_create(cont, NULL);
    lv_label_set_text(label, "00.00.00");
    lv_obj_align(label, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);
    labelDate = label;
    
    ContWeek_Creat(contDate);
}

static void LabelDate_Update()
{
    lv_label_set_text_fmt(labelDate, "%02d.%02d.%02d", calendar.w_year % 100, calendar.w_month, calendar.w_date);
    
    const char* week_str[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    lv_label_set_text(labelWeek, week_str[calendar.week]);
}

static void LabelTime_Update(lv_anim_enable_t anim_enable = LV_ANIM_ON)
{
    RTC_Get();
    
    /*分-个位*/
    lv_label_anim_effect_check_value(&labelTimeEffect[3], calendar.min % 10, anim_enable);
    /*分-十位*/
    lv_label_anim_effect_check_value(&labelTimeEffect[2], calendar.min / 10, anim_enable);
    
    /*时-个位*/
    lv_label_anim_effect_check_value(&labelTimeEffect[1], calendar.hour % 10, anim_enable);
    /*时-十位*/
    lv_label_anim_effect_check_value(&labelTimeEffect[0], calendar.hour / 10, anim_enable);
    
    lv_led_toggle(ledSecGrp[0]);
    lv_led_toggle(ledSecGrp[1]);
}

static void LabelTime_Creat(lv_obj_t* par)
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
    lv_obj_set_style_local_radius(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_border_width(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_color(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_align(led, NULL, LV_ALIGN_CENTER, 0, -15);
    ledSecGrp[0] = led;
    
    led = lv_led_create(par, led);
    lv_obj_align(led, NULL, LV_ALIGN_CENTER, 0, 15);
    ledSecGrp[1] = led;
}

static void ContTime_Creat(lv_obj_t* par)
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
    
    LabelTime_Creat(contTime);
}

static void ContHeartRate_Creat(lv_obj_t* par)
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
    lv_label_set_text(label, "HRT");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    
    label = lv_label_create(contHeartRate, NULL);
    lv_label_set_text(label, "60.0");
    lv_obj_align(label, cont, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    labelHeartRate = label;
    
    cont = lv_cont_create(contHeartRate, NULL);
    lv_obj_set_size(cont, lv_obj_get_width(contHeartRate) / 4, lv_obj_get_height(contHeartRate));
    lv_obj_align(cont, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    lv_obj_set_style_local_bg_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    
    LV_IMG_DECLARE(IMG_Heart);
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
    lv_anim_path_set_cb(&path, lv_anim_path_ease_in_out);
    lv_anim_set_path(&a, &path);
    
    lv_anim_start(&a);
}

static void LabelSteps_Update()
{
    IMU_Update();
    lv_label_set_text_fmt(labelSteps, "%05d", IMU_GetSteps());
}

static void ContSteps_Creat(lv_obj_t* par)
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
    lv_label_set_text(label, "STP");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    
    label = lv_label_create(contSteps, NULL);
    lv_label_set_text(label, "00000");
    lv_obj_align(label, cont, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    labelSteps = label;
}

static void ImgCHN_Creat(lv_obj_t* par)
{
    LV_IMG_DECLARE(IMG_CHNSecond);
    
    lv_obj_t* img = lv_img_create(par, NULL);
    lv_img_set_src(img, &IMG_CHNSecond);
    lv_obj_align(img, contTime, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 5);
    imgCHN = img;
}

static void PagePlayAnim(bool open)
{
    int cnt = 3;
    
    int step = open ? 0 : (cnt - 1);
    
    int dir = open ? +1 : -1;
    
    lv_opa_t opa_target = open ? LV_OPA_COVER : LV_OPA_TRANSP;
    
#define WIDTH_ANIM_DEF(obj, time)\
do{\
    lv_coord_t now_width = lv_obj_get_width(obj);\
    if(open)lv_obj_set_width(obj, 0);\
    lv_obj_set_hidden(obj, false);\
    LV_OBJ_ADD_ANIM(\
        obj, width,\
        open ? now_width : 0,\
        (time)\
    );\
}while(0)

    while(cnt--)
    {
        switch(step)
        {
            case 0:
                WIDTH_ANIM_DEF(contBatt, LV_ANIM_TIME_DEFAULT);
                PageDelay(LV_ANIM_TIME_DEFAULT);
                step += dir;
                break;
            case 1:
                WIDTH_ANIM_DEF(contDate, LV_ANIM_TIME_DEFAULT);
                WIDTH_ANIM_DEF(contTime, LV_ANIM_TIME_DEFAULT);
                WIDTH_ANIM_DEF(contHeartRate, LV_ANIM_TIME_DEFAULT);
                WIDTH_ANIM_DEF(contSteps, LV_ANIM_TIME_DEFAULT);
                PageDelay(LV_ANIM_TIME_DEFAULT);
                step += dir;
                break;
                
            case 2:
                const int bounce_time = 500;
                lv_obj_add_anim(
                    labelDate, NULL, 
                    (lv_anim_exec_xcb_t)lv_obj_set_opa_scale,
                    lv_obj_get_opa_scale(labelDate), opa_target,
                    bounce_time,
                    NULL,
                    lv_anim_path_bounce
                );
            
                for(int i = 0; i < __Sizeof(labelTimeEffect); i++)
                {
                    lv_obj_t* label;
                    
                    label = labelTimeEffect[i].label_1;
                    
                    lv_obj_add_anim(
                        label, NULL, 
                        (lv_anim_exec_xcb_t)lv_obj_set_opa_scale,
                        lv_obj_get_opa_scale(label), opa_target,
                        bounce_time,
                        NULL,
                        lv_anim_path_bounce
                    );
                    
                    label = labelTimeEffect[i].label_2;
                    
                    lv_obj_add_anim(
                        label, NULL, 
                        (lv_anim_exec_xcb_t)lv_obj_set_opa_scale,
                        lv_obj_get_opa_scale(label), opa_target,
                        bounce_time,
                        NULL,
                        lv_anim_path_bounce
                    );
                }

                lv_obj_add_anim(
                    labelHeartRate, NULL, 
                    (lv_anim_exec_xcb_t)lv_obj_set_opa_scale,
                    lv_obj_get_opa_scale(labelHeartRate), opa_target,
                    bounce_time,
                    NULL,
                    lv_anim_path_bounce
                );
            
                lv_obj_add_anim(
                    labelSteps, NULL, 
                    (lv_anim_exec_xcb_t)lv_obj_set_opa_scale,
                    lv_obj_get_opa_scale(labelSteps), opa_target,
                    bounce_time,
                    NULL,
                    lv_anim_path_bounce
                );

                lv_obj_add_anim(
                    imgCHN, NULL, 
                    (lv_anim_exec_xcb_t)lv_obj_set_opa_scale,
                    lv_obj_get_opa_scale(imgCHN), opa_target,
                    bounce_time,
                    NULL,
                    lv_anim_path_bounce
                );
                step += dir;
                PageDelay(bounce_time);
                break;
        }
    }
}

static void Task_1000msUpdate(lv_task_t* task)
{
    LabelDate_Update();
    LabelSteps_Update();
}

static void Task_500msUpdate(lv_task_t* task)
{
    LabelTime_Update();
}


static void Tasks_Creat()
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
    
    ContBatt_Creat(appWindow);
    lv_obj_set_hidden(contBatt, true);
    
    ContDate_Creat(appWindow);
    lv_obj_set_hidden(contDate, true);
    lv_obj_set_opa_scale(labelDate, LV_OPA_0);
    
    ContTime_Creat(appWindow);
    lv_obj_set_hidden(contTime, true);
    lv_obj_set_hidden(ledSecGrp[0], true);
    lv_obj_set_hidden(ledSecGrp[1], true);
    for(int i = 0; i < __Sizeof(labelTimeEffect); i++)
    {
        lv_obj_set_opa_scale(labelTimeEffect[i].label_1, LV_OPA_0);
        lv_obj_set_opa_scale(labelTimeEffect[i].label_2, LV_OPA_0);
    }
    
    ContHeartRate_Creat(appWindow);
    lv_obj_set_hidden(contHeartRate, true);
    lv_obj_set_opa_scale(labelHeartRate, LV_OPA_0);
    
    ContSteps_Creat(appWindow);
    lv_obj_set_hidden(contSteps, true);
    lv_obj_set_opa_scale(labelSteps, LV_OPA_0);
    
    ImgCHN_Creat(appWindow);
    lv_obj_set_opa_scale(imgCHN, LV_OPA_0);
    
    LabelTime_Update(LV_ANIM_OFF);
    LabelDate_Update();
    LabelSteps_Update();
    
    PagePlayAnim(true);
    
    lv_obj_set_hidden(ledSecGrp[0], false);
    lv_obj_set_hidden(ledSecGrp[1], false);
    
    Tasks_Creat();
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
    
    PagePlayAnim(false);
    
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
            Page->PagePush(PAGE_MainMenu);
        }
    }
}
