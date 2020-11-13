#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(BacklightSet);

static lv_obj_t* sliderBacklight;
static lv_obj_t* labelBacklight;

static void SliderBacklight_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        int val = lv_slider_get_value(obj);
        lv_label_set_text_fmt(labelBacklight, "%d%%", val);
        Backlight_SetGradual(val * 10, 200);
    }
}

static void SliderBacklight_Create(lv_obj_t* par)
{
    lv_obj_t* slider = lv_slider_create(par, NULL);
    lv_obj_set_height(slider, 15);
    lv_obj_set_width(slider, 180);
    lv_obj_set_style_local_bg_color(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, lv_color_make(0x00, 0x89, 0xff));
    lv_obj_set_style_local_radius(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, 4);
    lv_obj_set_style_local_border_color(slider, LV_SLIDER_PART_BG, LV_STATE_DEFAULT, lv_color_make(0x00, 0x89, 0xff));
    lv_obj_set_style_local_radius(slider, LV_SLIDER_PART_BG, LV_STATE_DEFAULT, 4);
    lv_obj_set_style_local_bg_color(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, lv_color_make(0x00, 0x89, 0xff));
    lv_obj_set_style_local_border_color(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, lv_color_make(0x00, 0x89, 0xff));
    lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(slider, SliderBacklight_EventHandler);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, Backlight_GetValue() / 10, LV_ANIM_OFF);

    lv_obj_t* label = lv_label_create(par, NULL);
    //lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
    //lv_obj_set_style_local_text_color(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x89, 0xff));
    lv_label_set_text(label, "0%");
    lv_obj_set_auto_realign(label, true);
    lv_obj_align(label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_label_set_text_fmt(label, "%d%%", lv_slider_get_value(slider));

    sliderBacklight = slider;
    labelBacklight = label;
}

static void PagePlayAnim(bool playback = false)
{
#define ANIM_Y_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), -lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
#define ANIM_Y_REV_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), APP_WIN_HEIGHT+lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}

    lv_anim_timeline_t anim_timeline[] =
    {
        ANIM_Y_REV_DEF(0, sliderBacklight),
        ANIM_Y_REV_DEF(0, labelBacklight),
    };

    uint32_t playtime = lv_anim_timeline_start(anim_timeline, __Sizeof(anim_timeline), playback);
    PageDelay(playtime);
    StatusBar_SetName(Page->GetCurrentName());
    StatusBar_SetEnable(!playback);
}

static void Setup()
{
    lv_obj_move_foreground(appWindow);
    SliderBacklight_Create(appWindow);
    PagePlayAnim(false);
}

static void Exit()
{
    PagePlayAnim(true);
    lv_obj_clean(appWindow);
}

static void Event(void* obj, uint8_t event)
{
    if (obj == lv_scr_act())
    {
        if (event == LV_GESTURE_DIR_TOP || event == LV_GESTURE_DIR_BOTTOM)
        {
            Page->Pop();
        }
    }
}
