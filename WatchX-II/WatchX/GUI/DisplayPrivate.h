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
void PageRegister_##name(PageManager* page_p, uint8_t pageID)\
{\
    appWindow = AppWindow_GetCont(pageID);\
    lv_obj_set_event_cb(appWindow, (lv_event_cb_t)Event);\
    Page = page_p;\
    page_p->PageRegister(pageID, Setup, NULL, Exit, Event);\
}

/*LittleVGL*/
#include "lvgl/lvgl.h"
#include "lv_ext/lv_label_anim_effect.h"
#include "lv_ext/lv_obj_ext_func.h"


/*AppWindow*/
void AppWindow_Create();
lv_obj_t * AppWindow_GetCont(uint8_t pageID);
lv_coord_t AppWindow_GetHeight();
lv_coord_t AppWindow_GetWidth();
#define APP_WIN_HEIGHT AppWindow_GetHeight()
#define APP_WIN_WIDTH  AppWindow_GetWidth()

#endif
