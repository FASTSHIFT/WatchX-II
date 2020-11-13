#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "GamePrivate.h"
#include "GameDisp.h"

PAGE_EXPORT(Game);

typedef struct {
    lv_color_t color;
    lv_coord_t x_ofs;
    lv_obj_t* led;
}LED_TypeDef;

static LED_TypeDef ledGrp[GAME_LED_MAX] = {
    {LV_COLOR_RED,   -20},
    {LV_COLOR_GREEN, -0},
    {LV_COLOR_BLUE,   20},
};

static lv_obj_t* contGameDisp;
static lv_task_t* taskGame;
static lv_obj_t* canvasGame;
static lv_color_t canvasBuf[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(GAME_DISP_WIDTH, GAME_DISP_HEIGHT)];

static lv_obj_t* contBtnGrp;
static lv_obj_t* btnGrp[GAME_BUTTON_MAX];
typedef struct {
    const char* sym;
    lv_align_t align;
    lv_coord_t x_mod;
    lv_coord_t y_mod;
}BtnCfg_TypeDef;
#define BTN_SIZE 40
static const BtnCfg_TypeDef btnCfg[GAME_BUTTON_MAX] =
{
    {"A",  LV_ALIGN_IN_BOTTOM_RIGHT, -30, -100},
    {"B",  LV_ALIGN_IN_BOTTOM_RIGHT, -30, -40},
    {"U",  LV_ALIGN_IN_TOP_MID,        0,  5},
    {"D",  LV_ALIGN_IN_BOTTOM_MID,     0,  -5},
    {"L",  LV_ALIGN_IN_LEFT_MID,       5,  0},
    {"R",  LV_ALIGN_IN_RIGHT_MID,     -5,  0},
};

static void ButtonGrp_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    for(int i = 0; i < GAME_BUTTON_MAX; i++)
    {
        if(obj == btnGrp[i])
        {
            if(event == LV_EVENT_PRESSED)
            {
                Game_SetButtonState(i, true);
            }
            else if(event == LV_EVENT_RELEASED || event == LV_EVENT_PRESS_LOST)
            {
                Game_SetButtonState(i, false);
            }
        }
    }
}

static void ButtonGrp_Create(lv_obj_t* par)
{
    lv_obj_t* contBtn = lv_obj_create(par, NULL);
    lv_obj_set_size(contBtn, BTN_SIZE * 3 + 10, BTN_SIZE * 3 + 10);
    lv_obj_set_style_local_radius(contBtn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_align(contBtn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 20, -10);

    contBtnGrp = contBtn;

    for (int i = 0; i < GAME_BUTTON_MAX; i++)
    {
        lv_obj_t* btn = lv_btn_create((i >= GAME_BUTTON_UP) ? contBtn : par, NULL);

        lv_obj_set_size(btn, BTN_SIZE, BTN_SIZE);
        lv_obj_align(btn, NULL, btnCfg[i].align, btnCfg[i].x_mod, btnCfg[i].y_mod);
        lv_obj_set_style_local_radius(btn, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 3);
        lv_obj_set_event_cb(btn, ButtonGrp_EventHandler);

        lv_obj_t* label = lv_label_create(btn, NULL);
        lv_label_set_text(label, btnCfg[i].sym);

        lv_obj_set_opa_scale(btn, LV_OPA_TRANSP);
        
        btnGrp[i] = btn;
    }
}

static void CanvasGame_Create(lv_obj_t* par)
{
    lv_obj_t* canvas = lv_canvas_create(par, NULL);
    lv_canvas_set_buffer(canvas, canvasBuf, GAME_DISP_WIDTH, GAME_DISP_HEIGHT, LV_IMG_CF_INDEXED_1BIT);
    lv_canvas_set_palette(canvas, 0, LV_COLOR_BLACK);
    lv_canvas_set_palette(canvas, 1, LV_COLOR_WHITE);
    lv_obj_align(canvas, NULL, LV_ALIGN_CENTER, 0, 0);

    canvasGame = canvas;
}

void CanvasGame_DrawBuf(uint8_t* buf, int16_t w, int16_t h)
{
#   define IsWhite ((buf[(row*w) + x] & (1 << bit_position)) >> bit_position)
    lv_color_t c;
    lv_obj_t* canvas = canvasGame;
    lv_canvas_ext_t* ext = (lv_canvas_ext_t*)lv_obj_get_ext_attr(canvas);
    lv_img_dsc_t* dsc = &ext->dsc;

    for (int16_t y = 0; y < h; y++)
    {
        for (int16_t x = 0; x < w; x++)
        {
            uint8_t row = y / 8;
            uint8_t bit_position = y % 8;
            c.full = IsWhite;
            lv_img_buf_set_px_color(dsc, x, y, c);
        }
    }

    lv_obj_invalidate(canvas);
}

static void CanvasGame_Update(lv_task_t * task)
{
    GamePage.Running();
    Game_DispTaskUpdate();
}

static void ContGameDisp_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_size(cont, GAME_DISP_WIDTH + 4, GAME_DISP_HEIGHT + 4);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);

    contGameDisp = cont;
}

static void LED_Create(lv_obj_t* par)
{
    for (int i = 0; i < GAME_LED_MAX; i++)
    {
        lv_obj_t* led = lv_led_create(par, NULL);
        lv_obj_set_style_local_bg_color(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, ledGrp[i].color);
        lv_obj_set_style_local_border_width(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_size(led, 10, 10);
        lv_obj_set_opa_scale(led, LV_OPA_TRANSP);
        lv_obj_align(led, contGameDisp, LV_ALIGN_OUT_BOTTOM_MID, ledGrp[i].x_ofs, 5);
        lv_led_off(led);
        ledGrp[i].led = led;
    }
}

void Game_SetLEDState(uint8_t led_id, bool val)
{
    if(led_id < GAME_LED_MAX && ledGrp[led_id].led != NULL)
    {
        lv_obj_t* led = ledGrp[led_id].led;
        val ? lv_led_on(led) : lv_led_off(led);
    }
}

void Game_SetLEDBright(uint8_t led_id, uint8_t val)
{
    if(led_id < GAME_LED_MAX && ledGrp[led_id].led != NULL)
    {
        lv_obj_t* led = ledGrp[led_id].led;
        lv_led_set_bright(led, val);
    }
}

static void PagePlayAnim(bool playback = false)
{
#define ANIM_Y_DOWN_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), -lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
#define ANIM_Y_UP_DEF(start_time, obj)   {start_time, obj, LV_ANIM_EXEC(y), APP_WIN_HEIGHT+lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
#define ANIM_OPA_DEF(start_time, obj)    {start_time, obj, LV_ANIM_EXEC(opa_scale), LV_OPA_TRANSP, LV_OPA_COVER, 100, lv_anim_path_ease_out} 
    lv_anim_timeline_t anim_timeline[] = {
        ANIM_Y_DOWN_DEF(0, contGameDisp),
        ANIM_Y_UP_DEF(0, contBtnGrp),
        ANIM_OPA_DEF(LV_ANIM_TIME_DEFAULT,       btnGrp[GAME_BUTTON_A]),
        ANIM_OPA_DEF(LV_ANIM_TIME_DEFAULT + 100, btnGrp[GAME_BUTTON_B]),
        ANIM_OPA_DEF(LV_ANIM_TIME_DEFAULT + 200, btnGrp[GAME_BUTTON_RIGHT]),
        ANIM_OPA_DEF(LV_ANIM_TIME_DEFAULT + 300, btnGrp[GAME_BUTTON_DOWN]),
        ANIM_OPA_DEF(LV_ANIM_TIME_DEFAULT + 400, btnGrp[GAME_BUTTON_LEFT]),
        ANIM_OPA_DEF(LV_ANIM_TIME_DEFAULT + 500, btnGrp[GAME_BUTTON_UP]),
        ANIM_OPA_DEF(LV_ANIM_TIME_DEFAULT + 600, ledGrp[GAME_LED_RED].led),
        ANIM_OPA_DEF(LV_ANIM_TIME_DEFAULT + 700, ledGrp[GAME_LED_GREEN].led),
        ANIM_OPA_DEF(LV_ANIM_TIME_DEFAULT + 800, ledGrp[GAME_LED_BLUE].led),
    };

    uint32_t playtime = lv_anim_timeline_start(anim_timeline, __Sizeof(anim_timeline), playback);
    PageDelay(playtime);
}

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup()
{
    lv_obj_move_foreground(appWindow);
    
    Game_Begin();
    
    ContGameDisp_Create(appWindow);
    LED_Create(appWindow);
    CanvasGame_Create(contGameDisp);
    ButtonGrp_Create(appWindow);
    
    PagePlayAnim();
    taskGame = lv_task_create(CanvasGame_Update, 10, LV_TASK_PRIO_LOW, NULL);
}

/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit()
{
    GamePage.ChangeTo(GAME_NONE);
    GamePage.Running();
    PagePlayAnim(true);
    lv_task_del(taskGame);
    lv_obj_clean(appWindow);
}

/**
  * @brief  页面事件
  * @param  event:事件编号
  * @param  param:事件参数
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
