#ifndef __DISPLAYPRIVATE_H
#define __DISPLAYPRIVATE_H

#ifdef ARDUINO

/*Basic*/
#include "Adafruit_ST7789/Adafruit_ST7789.h"

typedef Adafruit_ST7789 SCREEN_CLASS;

extern SCREEN_CLASS screen;

void DisplayError_Init();

void lv_port_disp_init();
void lv_port_indev_init();
void lv_port_log_init();

#endif

void Display_Init();
void Display_Update();

/*Page*/
#include "PageManager/PageManager.h"
typedef enum
{
    /*保留*/
    PAGE_NONE,
    /*用户页面*/
    PAGE_MainMenu,
    PAGE_DialPlate,
    PAGE_Bluetooth,
    PAGE_Calculator,
    PAGE_FileExplorer,
    PAGE_Game,
    PAGE_HeartRate,
    PAGE_Music,
    PAGE_Settings,
    PAGE_Sleep,
    PAGE_Sport,
    PAGE_Stopwatch,
    PAGE_TimeSet,
    PAGE_BacklightSet,
    /*保留*/
    PAGE_MAX
} Page_Type;

void PageDelay(uint32_t ms);
void DisplayPage_Init();

#define PAGE_EXPORT(name)\
static PageManager* Page;\
static lv_obj_t* appWindow;\
static void Setup();\
static void Exit();\
static void Event(void* obj, uint8_t event);\
static void PageEvent_##name(void* obj, uint8_t event)\
{\
    if(obj == Page)\
    {\
        switch (event)\
        {\
        case PageManager::MSG_Setup: Setup(); break; \
        case PageManager::MSG_Exit:  Exit();  break; \
        case PageManager::MSG_Loop:/*Loop();*/break; \
        default: break; \
        }\
    }\
    else\
    {\
        Event(obj, event);\
    }\
}\
void PageRegister_##name(PageManager* page, uint8_t pageID)\
{\
    appWindow = AppWindow_GetObj(pageID);\
    lv_obj_set_event_cb(appWindow, (lv_event_cb_t)Event);\
    page->Register(pageID, PageEvent_##name, #name);\
    Page = page; \
}

/*LVGL*/
#include "lvgl/lvgl.h"
#include "lv_ext/lv_label_anim_effect.h"
#include "lv_ext/lv_obj_ext_func.h"
#include "lv_ext/lv_anim_timeline.h"

/*AppWindow*/
void AppWindow_Create(lv_obj_t* par);
lv_obj_t* AppWindow_GetObj(uint8_t pageID);
#define APP_WIN_HEIGHT lv_obj_get_height(appWindow)
#define APP_WIN_WIDTH  lv_obj_get_width(appWindow)

/*StatusBar*/
void StatusBar_Create(lv_obj_t* par);
void StatusBar_SetName(const char* name);
void StatusBar_SetEnable(bool en, lv_anim_enable_t anim_en = LV_ANIM_ON);
const char* StatusBar_GetName();
bool StatusBar_GetEnable();

#endif
