#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
PAGE_EXPORT(Sleep);
#define ANIM_TIME 300
#define STANDARDGREY LV_COLOR_MAKE(0x80, 0x80, 0x80)
#define STANDARDWHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)

static lv_obj_t* imgSleep;
static lv_obj_t* labelText_Score;
static lv_obj_t* labelScore;
static lv_obj_t* labelOverview1;
static lv_obj_t* labelOverview2;
static lv_obj_t* labelOverview3;
static lv_obj_t* imgDown;
static lv_obj_t* overviewCont;
static lv_obj_t* detailPage;
static lv_obj_t* imgSleepRect;
static lv_obj_t* DetailCont;
static lv_obj_t* buttonUP;
static lv_obj_t* buttonDown;

static const lv_coord_t Btn_Width = 80;
static const lv_coord_t Btn_Height = 30;

typedef struct detailLabel {
    const char* labelTitle;
    lv_obj_t* timeLabel;
    const char* timeText;
    lv_obj_t* percentageLabel;
    const char* percentageText;
    lv_color_t color;

};

static detailLabel sleepDetail[]=
{
    {"Shallow sleep",   NULL,   "4h48min", NULL,"54%",   LV_COLOR_MAKE(0x3f, 0x71, 0xf5)},
    {"Deep sleep",      NULL,   "1h30min", NULL,"17%",   LV_COLOR_MAKE(0x2e, 0x31, 0x92)},
    {"REM",             NULL,   "2h34min", NULL,"29%",   LV_COLOR_MAKE(0x7a, 0xc9, 0x43)},
    {"Awake",           NULL,   "27min",   NULL,"27%",   LV_COLOR_MAKE(0xff, 0x93, 0x1e)}
};

extern "C" {
    LV_IMG_DECLARE(IMG_Sleep);
    LV_IMG_DECLARE(IMG_SleepRect);
    LV_IMG_DECLARE(IMG_Down);
    LV_IMG_DECLARE(IMG_UP);
}

static void Btn_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        static lv_coord_t overviewCont_y_target = lv_obj_get_y(overviewCont);
        static lv_coord_t detailCont_y_target = lv_obj_get_y(DetailCont);
        if (obj == buttonDown)
        {
            LV_OBJ_ADD_ANIM(overviewCont, y, overviewCont_y_target - APP_WIN_HEIGHT, 100);
            LV_OBJ_ADD_ANIM(DetailCont, y, detailCont_y_target - APP_WIN_HEIGHT, 100);
        }
        else if (obj == buttonUP)
        {
            LV_OBJ_ADD_ANIM(overviewCont, y, overviewCont_y_target , 100);
            LV_OBJ_ADD_ANIM(DetailCont, y, detailCont_y_target, 100);
        }
    }
}

static void Overview_Create(lv_obj_t* par)
{
    lv_obj_t* img = lv_img_create(par, NULL);
    lv_img_set_src(img, &IMG_Sleep);
    lv_obj_align(img, NULL, LV_ALIGN_IN_TOP_LEFT, 47, 0);

    lv_obj_t* label1 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
    lv_obj_set_style_local_text_color(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, STANDARDGREY);
    lv_label_set_text(label1, "score");
    lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label1, img, LV_ALIGN_OUT_RIGHT_TOP, 15, -5);

    lv_obj_t* label2 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_50);
    lv_obj_set_style_local_text_color(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, STANDARDWHITE);
    lv_label_set_text(label2, "78");
    lv_label_set_align(label2, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label2, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, -15);

    lv_obj_t* label3 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label3, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
    lv_obj_set_style_local_text_color(label3, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, STANDARDGREY);
    lv_label_set_text(label3, "Total time");
    lv_label_set_align(label3, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label3, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -100);

    lv_obj_t* label4 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_28);
    lv_obj_set_style_local_text_color(label4, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3F, 0xA9, 0xF5));
    lv_label_set_text(label4, "8h 52min");
    lv_label_set_align(label4, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label4, label3, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_t* label5 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label5, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
    lv_obj_set_style_local_text_color(label5, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3F, 0xA9, 0xF5));
    lv_label_set_text(label5, "00:29-09:48");
    lv_label_set_align(label5, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label5, label4, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_t* button = lv_btn_create(par, NULL);
    lv_obj_set_size(button, Btn_Width, Btn_Height);
    lv_obj_set_event_cb(button, Btn_EventHandler);
    lv_obj_set_style_local_bg_color(button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0x00, 0x00));
    lv_obj_set_style_local_bg_color(button, LV_BTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_MAKE(0xff, 0xff, 0xff));
    lv_obj_set_style_local_radius(button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);
    lv_obj_set_style_local_border_color(button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_style_local_border_width(button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_align(button, par, LV_ALIGN_IN_BOTTOM_MID, 0, -10);

    lv_obj_t* img2 = lv_img_create(par, NULL);
    lv_img_set_src(img2, &IMG_Down);
    lv_obj_align(img2, button, LV_ALIGN_CENTER, 0, 0);

    imgSleep = img;
    labelText_Score = label1;
    labelScore = label2;
    labelOverview1 = label3;
    labelOverview2 = label4;
    labelOverview3 = label5;
    buttonDown = button;
}

static void OverviewCont_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_width(cont, APP_WIN_WIDTH);
    lv_obj_set_height(cont, 200);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_BOTTOM_MID ,0, 0);

    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);

    overviewCont = cont;
}

static void ImageSleepRect_create(lv_obj_t* par)
{
    lv_obj_t* button1 = lv_btn_create(par, NULL);
    lv_obj_set_size(button1, Btn_Width, Btn_Height);
    lv_obj_set_event_cb(button1, Btn_EventHandler);
    lv_obj_set_style_local_bg_color(button1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0x00, 0x00));
    lv_obj_set_style_local_bg_color(button1, LV_BTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_MAKE(0x80, 0x80, 0x80));
    lv_obj_set_style_local_radius(button1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);
    lv_obj_set_style_local_border_color(button1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_style_local_border_width(button1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_align(button1, par, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_obj_t* btnimg = lv_img_create(button1, NULL);
    lv_img_set_src(btnimg, &IMG_UP);
    lv_obj_align(btnimg, button1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* img = lv_img_create(par, NULL);
    lv_img_set_src(img, &IMG_SleepRect);
    lv_obj_align(img, par, LV_ALIGN_IN_TOP_MID, 0, Btn_Height);
    imgSleepRect = img;

    lv_obj_t* label1 = lv_label_create(par,NULL);
    lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
    lv_obj_set_style_local_text_color(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, STANDARDGREY);
    lv_label_set_text(label1, "00:29");
    lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label1, img, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    lv_obj_t* label2 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
    lv_obj_set_style_local_text_color(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, STANDARDGREY);
    lv_label_set_text(label2, "09:48");
    lv_label_set_align(label2, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label2, img, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);

    buttonUP = button1;
}

static void DeltailLabels_Create(lv_obj_t* par)
{
    for (int i = 0; i < __Sizeof(sleepDetail); i++)
    {
        const char* text = sleepDetail[i].labelTitle;
        lv_color_t color = sleepDetail[i].color;
        lv_obj_t* title = lv_label_create(par, NULL);

        lv_obj_set_style_local_text_font(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
        lv_obj_set_style_local_text_color(title, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, STANDARDGREY);
        lv_label_set_text(title, text);
        lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
        lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_LEFT, 0, i*50);

        sleepDetail[i].timeLabel = lv_label_create(par, NULL);
        lv_obj_set_style_local_text_font(sleepDetail[i].timeLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
        lv_obj_set_style_local_text_color(sleepDetail[i].timeLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, STANDARDWHITE);
        lv_label_set_text(sleepDetail[i].timeLabel,sleepDetail[i].timeText);
        lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
        lv_obj_align(sleepDetail[i].timeLabel, title, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
        

        sleepDetail[i].percentageLabel = lv_label_create(par, NULL);
        lv_obj_set_style_local_text_font(sleepDetail[i].percentageLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
        lv_obj_set_style_local_text_color(sleepDetail[i].percentageLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, color);
        lv_label_set_text(sleepDetail[i].percentageLabel, sleepDetail[i].percentageText);
        lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
        lv_obj_align(sleepDetail[i].percentageLabel, sleepDetail[i].timeLabel, LV_ALIGN_OUT_RIGHT_MID, 15, 0);
    }
}

static void DetailPage_Create(lv_obj_t* par)
{
    lv_obj_t* page = lv_page_create(par,NULL);
    lv_obj_set_size(page,180, 130);
    lv_obj_align(page, par, LV_ALIGN_IN_BOTTOM_LEFT, 5, 0);  
    lv_page_set_scrollbar_mode(page, LV_SCRLBAR_MODE_HIDE);
    lv_obj_set_style_local_border_width(page, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    detailPage = page;
}

static void DetailCont_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par, NULL);
    lv_obj_set_width(cont, APP_WIN_WIDTH);
    lv_obj_set_height(cont, 230);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, APP_WIN_HEIGHT+10);

    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);

    DetailCont = cont;
}

static void PagePlayAnim(bool playback = false)
{
#define ANIM_Y_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), -lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
#define ANIM_Y_REV_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(y), APP_WIN_HEIGHT+lv_obj_get_height(obj),  lv_obj_get_y(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
    lv_anim_timeline_t anim_timeline[] = {
        ANIM_Y_REV_DEF(0,overviewCont),

    };

    uint32_t playtime = lv_anim_timeline_start(anim_timeline, __Sizeof(anim_timeline), playback);
    PageDelay(playtime);
    StatusBar_SetName(Page->GetCurrentName());
    StatusBar_SetEnable(!playback);
}

static void Setup()
{
    lv_obj_move_foreground(appWindow);
    OverviewCont_Create(appWindow);
    Overview_Create(overviewCont);
    DetailCont_Create(appWindow);
    ImageSleepRect_create(DetailCont);
    DetailPage_Create(DetailCont);
    DeltailLabels_Create(detailPage);
    PagePlayAnim();
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
        if (event == LV_GESTURE_DIR_RIGHT)
        {
            Page->Pop();
        }
    }
}