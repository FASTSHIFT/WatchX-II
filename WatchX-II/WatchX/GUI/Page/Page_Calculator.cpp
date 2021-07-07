#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Evaluate/evaluate.h"

PAGE_EXPORT(Calculator);

static lv_obj_t* contBtnGrp;
static lv_obj_t* textareaClac;

static const lv_coord_t Btn_Height = 35;
static const lv_coord_t Btn_Width  = 60;
static const lv_color_t Btn_ColorBlue   = LV_COLOR_MAKE(0x66, 0x99, 0xff);
static const lv_color_t Btn_ColorOrange = LV_COLOR_MAKE(0xff, 0xcc, 0x33);
static const lv_color_t Btn_ColorYellow = LV_COLOR_MAKE(0xff, 0x99, 0x00);
static const lv_color_t Btn_ColorWhite  = LV_COLOR_MAKE(0xff, 0xff, 0xff);

typedef struct
{
    const char* text;
    const lv_color_t color;
} BtnGrp_TypeDef;

static const BtnGrp_TypeDef BtnGrp[] =
{
    {"C",   Btn_ColorYellow},
    {"*",   Btn_ColorOrange  },
    {"/",   Btn_ColorOrange  },
    {"Del", Btn_ColorYellow},

    {"7",   Btn_ColorWhite },
    {"8",   Btn_ColorWhite },
    {"9",   Btn_ColorWhite },
    {"-",   Btn_ColorOrange  },

    {"4",   Btn_ColorWhite },
    {"5",   Btn_ColorWhite },
    {"6",   Btn_ColorWhite },
    {"+",   Btn_ColorOrange  },

    {"1",   Btn_ColorWhite },
    {"2",   Btn_ColorWhite },
    {"3",   Btn_ColorWhite },
    {"%",   Btn_ColorOrange  },

    {"Ans", Btn_ColorOrange  },
    {"0",   Btn_ColorWhite },
    {".",   Btn_ColorWhite },
    {"=",   Btn_ColorYellow},
};

static void BtnGrp_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    static char last_ans[128] = "\0";
#   define IS_STR(txt, str) (strcmp((txt), (str)) == 0)

    if(event == LV_EVENT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        const char* btn_text = lv_obj_get_style_value_str(obj, LV_BTN_PART_MAIN);
        const char* ta_text = lv_textarea_get_text(textareaClac);

        if (IS_STR(ta_text, "Error Format"))
        {
            lv_textarea_set_text(textareaClac, "");
        }

        if (IS_STR(btn_text, "C"))
        {
            lv_textarea_set_text(textareaClac, "");
        }
        else if(IS_STR(btn_text, "Del"))
        {
            lv_textarea_del_char(textareaClac);
        }
        else if (IS_STR(btn_text, "Ans"))
        {
            lv_textarea_add_text(textareaClac, last_ans);
        }
        else if (IS_STR(btn_text, "=") || IS_STR(btn_text, "%"))
        {   
            if (IS_STR(ta_text, ""))
            {
                return;
            }

            bool is_persent = IS_STR(btn_text, "%");

            evaluate_run_expression(ta_text, is_persent);
            char result[128];
            evaluate_get_result(result, sizeof(result));

            
            if (result[0] >= '0' && result[0] <= '9')
            {
                strncpy(last_ans, result, sizeof(last_ans));
            }

            lv_textarea_set_text(textareaClac, result);
        }
        else
        {
            uint32_t len = strlen(ta_text);
            if (len >= 1)
            {
                if (ta_text[len - 1] == '/' && IS_STR(btn_text, "0"))
                {
                    return;
                }
            }
            lv_textarea_add_text(textareaClac, btn_text);
        }
    }
}

static void BtnGrp_Create(lv_obj_t* par)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, LV_STATE_DEFAULT, 5);
    lv_style_set_border_color(&style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&style, LV_STATE_DEFAULT, 2);
    lv_style_set_value_align(&style, LV_STATE_DEFAULT, LV_ALIGN_CENTER);
    lv_style_set_value_color(&style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_value_font(&style, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);

    for(int i = 0; i < __Sizeof(BtnGrp); i++)
    {
        lv_color_t color = BtnGrp[i].color;
        const char* text = BtnGrp[i].text;

        lv_obj_t* btn = lv_btn_create(par, NULL);
        lv_obj_set_size(btn, Btn_Width, Btn_Height);
        lv_obj_set_x(btn, (i % 4) * Btn_Width);
        lv_obj_set_y(btn, (i / 4) * Btn_Height);
        lv_obj_set_event_cb(btn, BtnGrp_EventHandler);

        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style);
        lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, color);
        lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_GRAY);
        lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, text);
    }
}

static void ContBtnGrp_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_width(cont, APP_WIN_WIDTH);
    lv_obj_set_height(cont, Btn_Height * 5);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);

    contBtnGrp = cont;
}

static void TextareaCalc_Create(lv_obj_t* par)
{
    lv_obj_t* textarea = lv_textarea_create(par, NULL);
    lv_obj_set_width(textarea, APP_WIN_WIDTH);
    lv_obj_set_height(textarea, APP_WIN_HEIGHT - lv_obj_get_height(contBtnGrp));
    lv_obj_align(textarea, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_obj_set_style_local_text_font(textarea, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
    lv_obj_set_style_local_border_width(textarea, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, 0);

    lv_textarea_set_text(textarea, "");

    textareaClac = textarea;
}

static void PagePlayAnim(bool playback = false)
{
    lv_anim_timeline_t anim_timeline[] = {
        {0, contBtnGrp,   LV_ANIM_EXEC(y),         APP_WIN_HEIGHT, lv_obj_get_y(contBtnGrp), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out},
        {0, textareaClac, LV_ANIM_EXEC(opa_scale), LV_OPA_TRANSP,  LV_OPA_COVER,             LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_in_out},
    };

    if(playback)
    {
        lv_textarea_set_cursor_hidden(textareaClac, true);
    }
    
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
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);

    ContBtnGrp_Create(appWindow);
    BtnGrp_Create(contBtnGrp);
    TextareaCalc_Create(appWindow);
    PagePlayAnim();
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
    if(obj == lv_scr_act())
    {
        if(event == LV_GESTURE_DIR_BOTTOM)
        {
            Page->Pop();
        }
    }
    if (event == LV_EVENT_LEAVE)
    {
        Page->Pop();
    }
}
