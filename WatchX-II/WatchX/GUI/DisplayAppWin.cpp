#include "DisplayPrivate.h"

static lv_obj_t* appWindow_Grp[PAGE_MAX];

lv_obj_t* AppWindow_GetObj(uint8_t pageID)
{
    return (pageID < PAGE_MAX) ? appWindow_Grp[pageID] : NULL;
}

void AppWindow_Create(lv_obj_t* par)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&style, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_grad_color(&style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_color(&style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    
    for(int i = 0; i < PAGE_MAX; i++)
    {
        lv_obj_t * cont = lv_cont_create(par, NULL);
        lv_obj_add_style(cont, LV_CONT_PART_MAIN, &style);
        lv_obj_set_size(cont, lv_obj_get_width(par), lv_obj_get_height(par));
        lv_obj_align(cont, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_cont_set_fit(cont, LV_FIT_NONE);
        
        appWindow_Grp[i] = cont;
    }
}
