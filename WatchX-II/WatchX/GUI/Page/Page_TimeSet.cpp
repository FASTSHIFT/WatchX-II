#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include <stdio.h>

/*导出页面*/
PAGE_EXPORT(TimeSet);

static lv_obj_t* confirmBtn;
static lv_obj_t* switchBtn;
static lv_obj_t* switchBtnLabel;
static lv_obj_t* timesetCont;
static lv_obj_t* datesetCont;

static bool isDateset = false;

typedef struct
{
    const char* text;
    lv_coord_t pos;
    lv_obj_t* roller;
    lv_obj_t* label;
 }Roller_TypeDef;

static Roller_TypeDef rollerTimeGrp[] =
{
    {"hour",24},
    {"min",60},
    {"sec",60}
};
static Roller_TypeDef rollerDateGrp[] =
{
    {"year",30},
    {"mon",12},
    {"day",31}
};
static const char* rollersTimeset_Str =
"00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n"
"10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n"
"20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n"
"30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n"
"40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n"
"50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n";

static const char* rollersDateset_Str =
"01\n02\n03\n04\n05\n06\n07\n08\n09\n"
"10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n"
"20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n"
"30\n31\n";

#define RollersTimeset_SetEnd(buf, pos) (buf)[3*pos-1] = '\0'

static void BtnGrp_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (obj == confirmBtn)
        {
            Clock_Value_t clock;
            
            clock.year = lv_roller_get_selected(rollerDateGrp[0].roller) + 2001;
            clock.month = lv_roller_get_selected(rollerDateGrp[1].roller) + 1;
            clock.date = lv_roller_get_selected(rollerDateGrp[2].roller) + 1;
            clock.hour = lv_roller_get_selected(rollerTimeGrp[0].roller);
            clock.min = lv_roller_get_selected(rollerTimeGrp[1].roller);
            clock.sec = lv_roller_get_selected(rollerTimeGrp[2].roller);
            
            Clock_SetValue(&clock);
        }
        else if (obj == switchBtn)
        {
            static lv_coord_t timesetCont_x_target = lv_obj_get_x(timesetCont);
            static lv_coord_t datesetCont_x_target = lv_obj_get_x(datesetCont);

            if (!isDateset)
            {
                LV_OBJ_ADD_ANIM(timesetCont, x, timesetCont_x_target - APP_WIN_WIDTH, 100);
                LV_OBJ_ADD_ANIM(datesetCont, x, datesetCont_x_target - APP_WIN_WIDTH, 100);
                lv_label_set_text(switchBtnLabel, ">");
            }
            else
            {
                LV_OBJ_ADD_ANIM(timesetCont, x, timesetCont_x_target, 100);
                LV_OBJ_ADD_ANIM(datesetCont, x, datesetCont_x_target, 100);
                lv_label_set_text(switchBtnLabel, "<");
            }
            isDateset = !isDateset;
        }
    }
}



static void RollersTimeset_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par,NULL);
    lv_obj_set_size(cont, 180, 150);
    lv_obj_align(cont, NULL, LV_ALIGN_CENTER, 0, -10);
    lv_obj_set_style_local_border_width(cont,LV_CONT_PART_MAIN,LV_STATE_DEFAULT,0);
    for (int i = 0; i < __Sizeof(rollerTimeGrp); i++)
    {
        lv_obj_t* roller = lv_roller_create(cont, NULL);

        char buf[200];
        strcpy(buf, rollersTimeset_Str);

        RollersTimeset_SetEnd(buf, rollerTimeGrp[i].pos);

        lv_roller_set_options(roller, buf, LV_ROLLER_MODE_INIFINITE);
        lv_roller_set_visible_row_count(roller, 2);

        lv_obj_set_style_local_text_font(roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
        lv_obj_set_style_local_border_width(roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_radius(roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 8);
        lv_obj_set_style_local_bg_color(roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_BLACK);
        lv_obj_set_style_local_text_color(roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_obj_set_style_local_border_width(roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, 2);
        lv_obj_set_style_local_border_color(roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, lv_color_make(0x00, 0x89, 0xff));

        lv_obj_align(roller, cont, LV_ALIGN_IN_LEFT_MID, i * (lv_obj_get_width(roller)+2), 0);

        lv_obj_t* label = lv_label_create(cont, NULL);
        lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_16);
        lv_label_set_text(label, rollerTimeGrp[i].text);
        lv_obj_align(label, roller, LV_ALIGN_OUT_TOP_MID, 0, 0);
        rollerTimeGrp[i].label = label;
        rollerTimeGrp[i].roller = roller;
    }

    timesetCont = cont;
}

static void RollersDateset_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_size(cont, 180, 150);
    lv_obj_align(cont, NULL, LV_ALIGN_CENTER, 240, -10);
    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);

    for (int i = 0; i < __Sizeof(rollerDateGrp); i++)
    {

        lv_obj_t* roller = lv_roller_create(cont, NULL);

        char buf[200];
        strcpy(buf, rollersDateset_Str);

        RollersTimeset_SetEnd(buf, rollerDateGrp[i].pos);

        lv_roller_set_options(roller, buf, LV_ROLLER_MODE_INIFINITE);
        lv_roller_set_visible_row_count(roller, 2);
        

        lv_obj_set_style_local_text_font(roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
        lv_obj_set_style_local_border_width(roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_radius(roller, LV_ROLLER_PART_BG, LV_STATE_DEFAULT, 8);
        lv_obj_set_style_local_bg_color(roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_BLACK);
        lv_obj_set_style_local_text_color(roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_obj_set_style_local_border_width(roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, 2);
        lv_obj_set_style_local_border_color(roller, LV_ROLLER_PART_SELECTED, LV_STATE_DEFAULT, lv_color_make(0x00, 0x89, 0xff));

        lv_obj_align(roller, cont, LV_ALIGN_IN_LEFT_MID, i * (lv_obj_get_width(roller) + 2), 0);

        lv_obj_t* label = lv_label_create(cont, NULL);
        lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_16);
        lv_label_set_text(label, rollerDateGrp[i].text);
        lv_obj_align(label, roller, LV_ALIGN_OUT_TOP_MID, 0, 0);
        rollerDateGrp[i].label = label;
        rollerDateGrp[i].roller = roller;
    }
    datesetCont = cont;
}

static void ConfirmButton_Create(lv_obj_t* par)
{
    lv_obj_t* btn = lv_btn_create(par, NULL);
    lv_obj_set_size(btn, 80, 45);
    lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00,0x89,0xff));
    lv_obj_set_style_local_border_width(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);
    lv_obj_align(btn,NULL,LV_ALIGN_IN_BOTTOM_MID,-41,-15);
    lv_obj_t* label = lv_label_create(btn, NULL);
    lv_obj_set_style_local_text_font(label,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&Font_MicrosoftYaHei_28);
    lv_label_set_text(label,"OK");
    lv_obj_set_style_local_text_decor(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_TEXT_DECOR_NONE);

    lv_obj_set_event_cb(btn, BtnGrp_EventHandler);
    confirmBtn = btn;
}

static void SwitchButton_Create(lv_obj_t* par)
{
    lv_obj_t* btn = lv_btn_create(par, NULL);
    lv_obj_set_size(btn, 80, 45);
    lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x89, 0xff));
    lv_obj_set_style_local_border_width(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_radius(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);
    lv_obj_align(btn, confirmBtn, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_obj_t* label = lv_label_create(btn, NULL);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
    lv_label_set_text(label, ">");
    lv_obj_set_style_local_text_decor(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_TEXT_DECOR_NONE);

    lv_obj_set_event_cb(btn, BtnGrp_EventHandler);
    switchBtn = btn;
    switchBtnLabel = label;
}

static void RollerGrp_Update()
{
    Clock_Value_t clock;
    Clock_GetValue(&clock);
    lv_roller_set_selected(rollerDateGrp[0].roller, clock.year - 2001, LV_ANIM_OFF);
    lv_roller_set_selected(rollerDateGrp[1].roller, clock.month-1, LV_ANIM_OFF);
    lv_roller_set_selected(rollerDateGrp[2].roller, clock.date-1, LV_ANIM_OFF);
    lv_roller_set_selected(rollerTimeGrp[0].roller, clock.hour, LV_ANIM_OFF);
    lv_roller_set_selected(rollerTimeGrp[1].roller, clock.min, LV_ANIM_OFF);
    lv_roller_set_selected(rollerTimeGrp[2].roller, clock.sec, LV_ANIM_OFF);
}


static void PagePlayAnim(bool playback = false)
{
#define ANIM_Y_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), -lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
#define ANIM_Y_REV_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), APP_WIN_HEIGHT+lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}

    lv_anim_timeline_t anim_timeline[] = {       
        ANIM_Y_REV_DEF(0, timesetCont),
        ANIM_Y_REV_DEF(20, confirmBtn),
        ANIM_Y_REV_DEF(20, switchBtn),
    };

    uint32_t playtime = lv_anim_timeline_start(anim_timeline, __Sizeof(anim_timeline), playback);
    PageDelay(playtime);
    StatusBar_SetName(Page->GetCurrentName());
    StatusBar_SetEnable(!playback);
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
    RollersTimeset_Create(appWindow);
    RollersDateset_Create(appWindow);
    ConfirmButton_Create(appWindow);
    SwitchButton_Create(appWindow);
    RollerGrp_Update();
    PagePlayAnim(false);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
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
    if (obj == lv_scr_act())
    {
        if (event == LV_GESTURE_DIR_LEFT || event == LV_GESTURE_DIR_RIGHT)
        {
            Page->Pop();
        }
    }
}
