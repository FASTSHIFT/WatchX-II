#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(Settings);
typedef struct
{
    const char* text;
    const uint8_t pageID;
    lv_obj_t* btn;
    lv_obj_t* line;
} ListBtn_Typedef;

static ListBtn_Typedef btnGrp[] =
{
    {"Time", PAGE_TimeSet},
    {"Backlight", PAGE_BacklightSet},
    {"Wuhu", PAGE_NONE},
    {"Lala", PAGE_NONE},
    {"Yuele", PAGE_NONE},
    {"Rio", PAGE_NONE},
    {"Wotule", PAGE_NONE}
};

static lv_obj_t* settingsPage;
static void BtnGrp_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (obj == btnGrp[0].btn)
        {
            Page->Push(PAGE_TimeSet);
        }
        else if (obj == btnGrp[1].btn)
        {
            Page->Push(PAGE_BacklightSet);
        }
    }
}

static void SettingsList_Create(lv_obj_t* par)
{
    lv_obj_t* page = lv_page_create(par, NULL);
    lv_obj_set_size(page, APP_WIN_WIDTH, APP_WIN_HEIGHT);
    lv_obj_align(page, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_page_set_scrollbar_mode(page, LV_SCRLBAR_MODE_AUTO);
    lv_page_set_edge_flash(page, true);
    lv_obj_set_style_local_border_width(page, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);

    for (int i = 0; i < __Sizeof(btnGrp); i++)
    {
        static lv_point_t line_points[] = { {0, 0}, {200, 0}};
        lv_obj_t* list_btn = lv_btn_create(page, NULL);
        lv_page_glue_obj(list_btn, page);
        lv_obj_set_size(list_btn, 200, 60);
        lv_obj_set_style_local_border_width(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_bg_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
        lv_obj_set_style_local_radius(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_event_cb(list_btn, BtnGrp_EventHandler);
        lv_btn_set_layout(list_btn, LV_LAYOUT_COLUMN_LEFT);

        if (i == 0)
        {
            lv_obj_align(list_btn, page, LV_ALIGN_IN_TOP_MID, 0, 0);
        }
        else
        {
            lv_obj_align(list_btn, btnGrp[i - 1].btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 1);
        }
        btnGrp[i].btn = list_btn;

        lv_obj_t* label = lv_label_create(list_btn, NULL);
        lv_label_set_text(label, btnGrp[i].text);
        lv_obj_set_style_local_text_decor(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_TEXT_DECOR_NONE);

        lv_obj_t* line = lv_line_create(page, NULL);
        lv_page_glue_obj(list_btn, page);
        lv_line_set_points(line, line_points, 2);
        lv_obj_set_style_local_line_width(line, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, 2);
        lv_obj_set_style_local_line_color(line, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x89, 0xff));
        lv_obj_align(line, list_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        btnGrp[i].line = line;
    }
    settingsPage = page;
}


static void PagePlayAnim(bool playback = false)
{
#define ANIM_X_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(x), -lv_obj_get_width(obj),  lv_obj_get_x(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
#define ANIM_X_REV_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(x), APP_WIN_HEIGHT+lv_obj_get_width(obj),  lv_obj_get_x(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}

    lv_anim_timeline_t anim_timeline[] =
    {
        ANIM_X_REV_DEF(0, settingsPage),
    };

    uint32_t playtime = lv_anim_timeline_start(anim_timeline, __Sizeof(anim_timeline), playback);
    PageDelay(playtime);
    StatusBar_SetName(Page->GetCurrentName());
    StatusBar_SetEnable(!playback);
}

static void Setup()
{
    lv_obj_move_foreground(appWindow);
    SettingsList_Create(appWindow);
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
