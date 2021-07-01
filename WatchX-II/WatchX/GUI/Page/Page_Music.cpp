#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(Music);
#define ANIM_TIME 300
#define STANDARDGREY LV_COLOR_MAKE(0x80, 0x80, 0x80)
#define STANDARDWHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define DEEPDGREY LV_COLOR_MAKE(0x33, 0x33, 0x33)
#define ALBUM_SIZE 80
#define ALBUM_ANIM_TIME 100

static const lv_coord_t Btn_Width = 80;
static const lv_coord_t Btn_Height = 30;

extern "C" {
    LV_IMG_DECLARE(IMG_Forward);
    LV_IMG_DECLARE(IMG_Backward);
    LV_IMG_DECLARE(IMG_VOLDown);
    LV_IMG_DECLARE(IMG_VOLUP);
    LV_IMG_DECLARE(IMG_AlbumArt);
}

static lv_obj_t* musicCont;
static lv_obj_t* albumBtn;
static lv_obj_t* albumImg;
static lv_obj_t* titleLabel;
static lv_obj_t* authorLabel;
typedef struct BtnGrp_t 
{
    lv_obj_t* btn;
    lv_img_dsc_t img;
    lv_coord_t Btn_Width;
    lv_coord_t Btn_Height;
    lv_coord_t Btn_x;
    lv_coord_t Btn_y;
};

static struct BtnGrp_t btnGrp[] = 
{
    {NULL,  IMG_Backward,  50, 60, 0,    0},
    {NULL,  IMG_Forward,   50, 60, 174,  0},
    {NULL,  IMG_VOLDown,   50, 48, 0,    65},
    {NULL,  IMG_VOLUP,     50, 48, 174,  65},
};
static void AppClickAnim( bool ispress)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, albumImg);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_zoom);
    lv_anim_set_values(&a, lv_img_get_zoom(albumImg), ispress ? 245 : LV_IMG_ZOOM_NONE);
    lv_anim_set_time(&a, ALBUM_ANIM_TIME);

    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, lv_anim_path_ease_in_out);
    lv_anim_set_path(&a, &path);

    lv_anim_start(&a);
}
static void BtnGrp_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_PRESSED)
    {
        if (obj == albumBtn)
        {
            AppClickAnim(true);
        }
        
    }
    else if (event == LV_EVENT_RELEASED || event == LV_EVENT_PRESS_LOST)
    {
        if (obj == albumBtn)
        {
            AppClickAnim(false);
        }
    }
}

static void Title_Create(lv_obj_t* par)
{
    lv_obj_t* label1 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
    lv_obj_set_style_local_text_color(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0xb9, 0x00, 0x5e));
    lv_label_set_text(label1, "Oh Darling");
    lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label1, par, LV_ALIGN_IN_TOP_MID, 0, 20);

    lv_obj_t* label2 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_16);
    lv_obj_set_style_local_text_color(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, STANDARDGREY);
    lv_label_set_text(label2, "The Beatles");
    lv_label_set_align(label2, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label2, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    titleLabel = label1;
    authorLabel = label2;
}

static void BtnGrp_Create(lv_obj_t* par)
{

    for (int i = 0; i < __Sizeof(btnGrp); i++)
    {
        lv_obj_t* button = lv_btn_create(par, NULL);
        lv_obj_set_size(button, btnGrp[i].Btn_Width, btnGrp[i].Btn_Height);
        lv_obj_set_event_cb(button, BtnGrp_EventHandler);
        lv_obj_set_style_local_bg_color(button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, STANDARDGREY);
        lv_obj_set_style_local_bg_color(button, LV_BTN_PART_MAIN, LV_STATE_PRESSED, DEEPDGREY);
        lv_obj_set_style_local_radius(button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 8);
        lv_obj_set_style_local_border_width(button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_align(button, par, LV_ALIGN_IN_TOP_LEFT, btnGrp[i].Btn_x, btnGrp[i].Btn_y);

        lv_obj_t* btnimg = lv_img_create(button, NULL);
        lv_img_set_src(btnimg, &btnGrp[i].img);
        lv_obj_align(btnimg, button, LV_ALIGN_CENTER, 0, 0);

        btnGrp[i].btn = button;
    }
    lv_obj_t* midbutton = lv_btn_create(par, NULL);
    lv_obj_set_size(midbutton, 100,100);
    lv_obj_set_event_cb(midbutton, BtnGrp_EventHandler);
    lv_obj_set_style_local_bg_color(midbutton, LV_BTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_MAKE(0x00,0x00,0x00));
    lv_obj_set_style_local_border_width(midbutton, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_align(midbutton, par, LV_ALIGN_IN_TOP_LEFT, 62,6);

    lv_obj_t* albumimg = lv_img_create(midbutton, NULL);
    lv_img_set_src(albumimg, &IMG_AlbumArt);
    lv_obj_align(albumimg, midbutton, LV_ALIGN_CENTER, 0, 0);

    albumImg = albumimg;
    albumBtn = midbutton;

}

static void ProgressBar_Create(lv_obj_t* par)
{

    static lv_point_t BGline_points[] = { {0, 0}, {200, 0} };
    lv_obj_t* BGline = lv_line_create(par, NULL);
    lv_line_set_points(BGline, BGline_points, 2);
    lv_obj_set_style_local_line_width(BGline, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_line_color(BGline, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x36, 0x00, 0x26));
    lv_obj_align(BGline, par, LV_ALIGN_IN_TOP_LEFT , 12, 123);

    static lv_point_t line_points[] = { {0, 0}, {145, 0} };
    lv_obj_t* line = lv_line_create(par, NULL);
    lv_line_set_points(line, line_points, 2);
    lv_obj_set_style_local_line_width(line, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, 3);
    lv_obj_set_style_local_line_color(line, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0xb9, 0x00, 0x5e));
    lv_obj_align(line, par, LV_ALIGN_IN_TOP_LEFT, 12, 123);

    lv_obj_t* label1 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
    lv_obj_set_style_local_text_color(label1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0xb9, 0x00, 0x5e));
    lv_label_set_text(label1, "2:29");
    lv_label_set_align(label1, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label1, BGline, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    lv_obj_t* label2 = lv_label_create(par, NULL);
    lv_obj_set_style_local_text_font(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &Font_MicrosoftYaHei_20);
    lv_obj_set_style_local_text_color(label2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, STANDARDGREY);
    lv_label_set_text(label2, "3:29");
    lv_label_set_align(label2, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(label2, BGline, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);
    
}

static void Cont_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par,NULL);
    lv_obj_set_width(cont, 224);
    lv_obj_set_height(cont, 230);
    lv_obj_align(cont, NULL, LV_ALIGN_IN_TOP_MID, 0, 72);

    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);

    musicCont = cont;
}


static void PagePlayAnim(bool playback = false)
{
#define ANIM_X_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(x), -lv_obj_get_width(obj),  lv_obj_get_x(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}
#define ANIM_X_REV_DEF(start_time, obj) {start_time, obj, LV_ANIM_EXEC(x), APP_WIN_HEIGHT+lv_obj_get_width(obj),  lv_obj_get_x(obj), LV_ANIM_TIME_DEFAULT, lv_anim_path_ease_out}

    lv_anim_timeline_t anim_timeline[] =
    {
        ANIM_X_REV_DEF(0, titleLabel),
        ANIM_X_REV_DEF(50, authorLabel),
        ANIM_X_REV_DEF(100, musicCont),
    };

    uint32_t playtime = lv_anim_timeline_start(anim_timeline, __Sizeof(anim_timeline), playback);
    PageDelay(playtime);
    StatusBar_SetName(Page->GetCurrentName());
    StatusBar_SetEnable(!playback);
}

static void Setup()
{
    lv_obj_move_foreground(appWindow);
    Title_Create(appWindow);
    Cont_Create(appWindow);
    BtnGrp_Create(musicCont);
    ProgressBar_Create(musicCont);
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
}

