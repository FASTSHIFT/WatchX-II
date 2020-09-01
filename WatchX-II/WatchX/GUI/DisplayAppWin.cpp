#include "DisplayPrivate.h"

typedef struct{
    lv_obj_t * cont;
    lv_style_t style;
}AppWindow_TypeDef;

static AppWindow_TypeDef appWindow_Grp[PAGE_MAX];

lv_obj_t * AppWindow_GetCont(uint8_t pageID)
{
    return (pageID < PAGE_MAX) ? appWindow_Grp[pageID].cont : NULL;
}

lv_coord_t AppWindow_GetHeight()
{
    return (lv_obj_get_height(lv_scr_act()));// - BarStatus_GetHeight() - BarNavigation_GetHeight());
}

lv_coord_t AppWindow_GetWidth()
{
    return (lv_obj_get_width(lv_scr_act()));
}

void AppWindow_Creat()
{
    for(int i = 0; i < PAGE_MAX; i++)
    {
        lv_obj_t * cont = lv_cont_create(lv_scr_act(), NULL);
        
        lv_obj_set_size(cont, AppWindow_GetWidth(), AppWindow_GetHeight());
        lv_obj_align(cont, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_cont_set_fit(cont, LV_FIT_NONE);
        
        appWindow_Grp[i].cont = cont;
        lv_style_init(&appWindow_Grp[i].style);
    }
}
