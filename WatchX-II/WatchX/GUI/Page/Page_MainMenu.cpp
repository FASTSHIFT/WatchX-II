#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(MainMenu);

static lv_obj_t* contApps;

extern "C" {
    LV_IMG_DECLARE(IMG_Bluetooth);
    LV_IMG_DECLARE(IMG_Calculator);
    LV_IMG_DECLARE(IMG_FileExplorer);
    LV_IMG_DECLARE(IMG_Game);
    LV_IMG_DECLARE(IMG_HeartRate);
    LV_IMG_DECLARE(IMG_Music);
    LV_IMG_DECLARE(IMG_Settings);
    LV_IMG_DECLARE(IMG_Sleep);
    LV_IMG_DECLARE(IMG_Sport);
    LV_IMG_DECLARE(IMG_Stopwatch);
    LV_IMG_DECLARE(IMG_AppShadow);
}

typedef struct
{
    const void* src_img;
    const char* name;
    const uint8_t pageID;
    const lv_color_t bg_color;
    lv_obj_t* img;
    lv_obj_t* cont;
} AppICON_TypeDef;

#define APP_DEF(name, color) {&IMG_##name, #name, PAGE_##name, color, NULL}
#define APP_ICON_SIZE 80

static AppICON_TypeDef AppICON_Grp[] =
{
    APP_DEF(Bluetooth,    LV_COLOR_MAKE(0, 40, 255)),
    APP_DEF(Calculator,   LV_COLOR_MAKE(248, 119, 0)),
    APP_DEF(FileExplorer, LV_COLOR_MAKE(255, 184, 78)),
    APP_DEF(Game,         LV_COLOR_MAKE(15, 255, 186)),
    APP_DEF(HeartRate,    LV_COLOR_MAKE(255, 84, 42)),
    APP_DEF(Music,        LV_COLOR_MAKE(238, 76, 132)),
    APP_DEF(Settings,     LV_COLOR_MAKE(0, 137, 255)),
    APP_DEF(Sleep,        LV_COLOR_MAKE(0, 27, 200)),
    APP_DEF(Sport,        LV_COLOR_MAKE(155, 93, 255)),
    APP_DEF(Stopwatch,    LV_COLOR_MAKE(60, 255, 167)),
};

static void AppClickAnim(lv_obj_t* cont, lv_obj_t* img, bool ispress)
{
    LV_OBJ_ADD_ANIM(cont, width, ispress ? 70 : 80, 100);
    LV_OBJ_ADD_ANIM(cont, height, ispress ? 70 : 80, 100);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, img);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_zoom);
    lv_anim_set_values(&a, lv_img_get_zoom(img), ispress ? 190 : LV_IMG_ZOOM_NONE);
    lv_anim_set_time(&a, 100);

    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, lv_anim_path_ease_in_out);
    lv_anim_set_path(&a, &path);

    lv_anim_start(&a);
}

static uint8_t AppICON_GetPageID(lv_obj_t* obj)
{
    uint8_t pageID = PAGE_NONE;
    for (int i = 0; i < __Sizeof(AppICON_Grp); i++)
    {
        if (obj == AppICON_Grp[i].cont)
        {
            pageID = AppICON_Grp[i].pageID;
            break;
        }
    }
    return pageID;
}

static void AppICON_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    if(event == LV_EVENT_PRESSED)
    {
        AppClickAnim(obj, lv_obj_get_child(obj, NULL), true);
    }
    else if(event == LV_EVENT_RELEASED || event == LV_EVENT_PRESS_LOST)
    {
        AppClickAnim(obj, lv_obj_get_child(obj, NULL), false);
    }

    if(event == LV_EVENT_CLICKED)
    {
        uint8_t pageID = AppICON_GetPageID(obj);

        if (pageID == PAGE_NONE)
            return;

        if (pageID == PAGE_Calculator)
        {
            Page->PagePush(PAGE_Calculator);
        }
        else if (pageID == PAGE_Stopwatch)
        {
            Page->PagePush(PAGE_Stopwatch);
        }
    }
}

static void AppICON_Create(lv_obj_t* par)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, LV_STATE_DEFAULT, 10);
    lv_style_set_border_width(&style, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&style, LV_STATE_PRESSED, LV_COLOR_GRAY);

    for(int i = 0; i < __Sizeof(AppICON_Grp); i++)
    {
        lv_obj_t* cont = lv_cont_create(par, NULL);
        lv_obj_add_style(cont, LV_CONT_PART_MAIN, &style);
        lv_obj_set_size(cont, APP_ICON_SIZE, APP_ICON_SIZE);
        lv_obj_set_style_local_bg_color(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, AppICON_Grp[i].bg_color);

        lv_obj_set_event_cb(cont, AppICON_EventHandler);
        lv_obj_set_drag_parent(cont, true);

        lv_coord_t interval_pixel_0 = (lv_obj_get_width(par) - lv_obj_get_width(cont) * 2) / 3;
        lv_coord_t interval_pixel_1 = interval_pixel_0 + lv_obj_get_width(cont);
        lv_coord_t interval_pixel_2 = lv_obj_get_width(cont) / 2 + interval_pixel_0 / 2;

        lv_obj_set_auto_realign(cont, true);
        lv_obj_align(
            cont,
            NULL,
            LV_ALIGN_IN_TOP_MID,
            ((i % 2) == 0) ? -interval_pixel_2 : interval_pixel_2,
            interval_pixel_0 + (i / 2) * interval_pixel_1
        );

        lv_obj_t* img = lv_img_create(cont, NULL);
        lv_img_set_src(img, AppICON_Grp[i].src_img);
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_auto_realign(img, true);

        AppICON_Grp[i].cont = cont;
        AppICON_Grp[i].img = img;
    }
}

static void ImgAppShadow_Create(lv_obj_t* par)
{
    lv_obj_t* img1 = lv_img_create(par, NULL);
    lv_img_set_src(img1, &IMG_AppShadow);
    lv_obj_align(img1, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_obj_t* img2 = lv_img_create(par, img1);
    lv_img_set_angle(img2, 1800);
    lv_obj_align(img2, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
}

static void ContApps_ReleaseAnim(lv_coord_t manual_target = 0)
{
    lv_coord_t y_pos = lv_obj_get_y(contApps);
    lv_coord_t y_min = -(lv_obj_get_height(contApps) - APP_WIN_HEIGHT);
    lv_coord_t y_target = 0;

    if(!manual_target)
    {
        if(y_pos > 0)
        {
            y_target = 0;
        }
        else if(y_pos < y_min)
        {
            y_target = y_min;
        }
        else
        {
            return;
        }
    }
    else
    {
        y_target = manual_target;
    }

    LV_OBJ_ADD_ANIM(contApps, y, y_target, LV_ANIM_TIME_DEFAULT);
}

static void ContApps_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    static bool is_draging = false;

    if (event == LV_EVENT_DRAG_BEGIN)
    {
        is_draging = true;
    }
    else if (event == LV_EVENT_DRAG_END)
    {
        is_draging = false;
    }

    if((!is_draging && event == LV_EVENT_RELEASED)
            || event == LV_EVENT_PRESS_LOST
            || event == LV_EVENT_DRAG_END
      )
    {
        if(lv_anim_get(contApps, (lv_anim_exec_xcb_t)lv_obj_set_y) == NULL)
        {
            ContApps_ReleaseAnim();
        }
    }

}

static void ContApps_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);

    lv_obj_set_width(cont, APP_WIN_WIDTH);

    lv_coord_t interval_pixel_h = (lv_obj_get_width(par) - APP_ICON_SIZE * 2) / 3;
    lv_obj_set_height(
        cont,
        (interval_pixel_h + APP_ICON_SIZE)
        * (__Sizeof(AppICON_Grp) / 2)
        + interval_pixel_h
    );
    lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_style_local_border_width(cont, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_event_cb(cont, ContApps_EventHandler);
    lv_obj_set_drag(cont, true);
    lv_obj_set_drag_throw(cont, true);
    lv_obj_set_drag_dir(cont, LV_DRAG_DIR_VER);

    contApps = cont;
}

static void PagePlayAnim(bool open)
{
    if(open)
    {
        lv_obj_set_y(contApps, APP_WIN_HEIGHT);
        ContApps_ReleaseAnim();
    }
    else
    {
        ContApps_ReleaseAnim(APP_WIN_HEIGHT);
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

    ContApps_Create(appWindow);
    ImgAppShadow_Create(appWindow);
    AppICON_Create(contApps);
    PagePlayAnim(true);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    PagePlayAnim(false);
    PageDelay(LV_ANIM_TIME_DEFAULT);
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
        if(event == LV_GESTURE_DIR_LEFT || event == LV_GESTURE_DIR_RIGHT)
        {
            Page->PagePop();
        }
    }
}
