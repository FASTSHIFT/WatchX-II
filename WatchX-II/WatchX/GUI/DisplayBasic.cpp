#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

#ifdef ARDUINO

#define USE_FPS_TEST 0

/*实例化屏幕对象*/
SCREEN_CLASS screen(
    TFT_PORT_Pin, TFT_RES_Pin, TFT_CS_Pin,
    TFT_RS_Pin, TFT_WR_Pin, TFT_RD_Pin
);

#if (USE_FPS_TEST == 1) 
static void Display_FPSTest();
#endif



extern "C" {
    void lv_demo_benchmark(void);
    void lv_demo_widgets(void);
    void lv_demo_stress(void);
}

/**
  * @brief  显示初始化
  * @param  无
  * @retval 无
  */
void Display_Init()
{
    /*背光关闭*/
    Backlight_Init();
    Backlight_SetValue(0);
    
    /*屏幕初始化*/
    screen.begin();
    screen.setRotation(0);
    screen.fillScreen(screen.Black);
    
#if (USE_FPS_TEST == 1)
    Display_FPSTest();
#endif
    
    /*自动报错初始化*/
    DisplayError_Init();
    
    /*lvgl初始化*/
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    lv_port_log_init();
    
    /*APP窗口初始化*/
    AppWindow_Create();
    
    /*页面初始化*/
    DisplayPage_Init();
    
    /*背光渐亮*/
    Backlight_SetGradual(Backlight_GetBKP(), 100);
    
    //lv_demo_benchmark();
    //lv_demo_stress();
    //lv_demo_widgets();
}



#if (USE_FPS_TEST == 1) 
static void Display_FPSTest()
{
    float Ftime, Ltime;
    float FPS;
    Ftime = millis();
    int i;
    for(i = 100; i > 0; i--)
    {
        screen.fillScreen(screen.Black);
        screen.fillScreen(screen.White);
    }
    Ltime = millis() - Ftime;
    FPS = 100.0f / (Ltime / 1000.0f) * 2.0f;
    screen.fillScreen(screen.Blue);
    screen.setTextSize(1);
    screen.setTextColor(screen.White);
    screen.setCursor(0, 0);
    screen.print(Ltime);
    screen.setCursor(45, 0);
    screen.print("ms");
    screen.setCursor(0, 9);
    screen.print("FPS:");
    screen.setCursor(25, 9);
    screen.print(FPS);
    while(1);
}
#endif

#else

void Display_Init()
{
    /*APP窗口初始化*/
    AppWindow_Create();

    /*页面初始化*/
    DisplayPage_Init();
}

#endif
