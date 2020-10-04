/**
* @file main
*
*/

/*********************
*      INCLUDES
*********************/
#include <stdlib.h>
#include <Windows.h>
#include <SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/keyboard.h"
#include "GUI/DisplayPrivate.h"

/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/

/**********************
*  STATIC PROTOTYPES
**********************/
static void hal_init(void);
static int tick_thread(void *data);

/**********************
*  STATIC VARIABLES
**********************/
static lv_indev_t * kb_indev;

/**********************
*      MACROS
**********************/

/**********************
*   GLOBAL FUNCTIONS
**********************/

int main(int argc, char** argv)
{
    /*Initialize LittlevGL*/
    lv_init();

    /*Initialize the HAL for LittlevGL*/
    hal_init();

    Display_Init();

    while (1)
    {
        Display_Update();
        Sleep(10);       /*Just to let the system breathe */
    }

    return 0;
}

/**********************
*   STATIC FUNCTIONS
**********************/


/**
* Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
*/
static void hal_init(void)
{
    /* Add a display
    * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    monitor_init();

    static lv_disp_buf_t disp_buf1;
    static lv_color_t buf1_1[LV_HOR_RES_MAX * 120];
    lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * 120);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.buffer = &disp_buf1;
    disp_drv.flush_cb = monitor_flush;
    lv_disp_drv_register(&disp_drv);

    /* Add the mouse (or touchpad) as input device
    * Use the 'mouse' driver which reads the PC's mouse*/
    mouse_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_drv_register(&indev_drv);

    /* If the PC keyboard driver is enabled in`lv_drv_conf.h`
    * add this as an input device. It might be used in some examples. */
#if USE_KEYBOARD
    lv_indev_drv_t kb_drv;
    lv_indev_drv_init(&kb_drv);
    kb_drv.type = LV_INDEV_TYPE_KEYPAD;
    kb_drv.read_cb = keyboard_read;
    kb_indev = lv_indev_drv_register(&kb_drv);
#endif

    /* Tick init.
    * You have to call 'lv_tick_inc()' in every milliseconds
    * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);
}

/**
* A task to measure the elapsed time for LittlevGL
* @param data unused
* @return never return
*/
static int tick_thread(void *data)
{
    while (1) {
        lv_tick_inc(5);
        SDL_Delay(5);   /*Sleep for 1 millisecond*/
    }

    return 0;
}
