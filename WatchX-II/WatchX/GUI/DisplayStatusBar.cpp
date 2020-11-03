#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

static lv_obj_t* labelTitle;
static lv_obj_t* labelTitleTime;
static lv_obj_t* contTitle;

static void StatusBar_Update(lv_task_t* task)
{
    RTC_Get();
    lv_label_set_text_fmt(labelTitleTime, "%02d:%02d", calendar.hour, calendar.min);
}

void StatusBar_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par,NULL);
    lv_obj_set_size(cont ,lv_obj_get_width(par),20);
    lv_obj_set_y(cont, -lv_obj_get_height(cont));
    lv_obj_set_style_local_border_width(cont,LV_CONT_PART_MAIN,LV_STATE_DEFAULT,0);

    lv_obj_t* label1 = lv_label_create(cont, NULL);

    lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_16);
    lv_obj_set_style_local_text_color(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_label_set_text(label1, "");
    lv_obj_align(label1, cont, LV_ALIGN_IN_LEFT_MID, 15, 0);

    lv_obj_t* label2 = lv_label_create(cont, NULL);

    lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_16);
    lv_obj_set_style_local_text_color(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_label_set_text(label2, "00:00");
    lv_obj_align(label2, cont, LV_ALIGN_IN_RIGHT_MID, -15, 0);

    labelTitle = label1;
    labelTitleTime = label2;
    contTitle = cont;
    lv_task_create(StatusBar_Update, 1000, LV_TASK_PRIO_LOW, NULL);
}

void StatusBar_SetName(const char* name)
{
    lv_label_set_text(labelTitle, name);
}

const char* StatusBar_GetName()
{
    return lv_label_get_text(labelTitle);
}

void StatusBar_SetEnable(bool en, lv_anim_enable_t anim_en)
{
    lv_coord_t y_target = en ? 0 : -lv_obj_get_height(contTitle);

    if (anim_en)
    {
        LV_OBJ_ADD_ANIM(contTitle, y, y_target, LV_ANIM_TIME_DEFAULT);
    }
    else
    {
        lv_obj_set_y(contTitle, y_target);
    }
}

bool StatusBar_GetEnable()
{
    return (lv_obj_get_y(contTitle) != -lv_obj_get_height(contTitle)) ? true : false;
}
