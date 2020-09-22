#ifndef __LV_OBJ_EXT_FUNC_H
#define __LV_OBJ_EXT_FUNC_H

#include "lvgl/lvgl.h"

#define LV_ANIM_TIME_DEFAULT    400
#define LV_SYMBOL_DEGREE_SIGN   "\xC2\xB0"

bool lv_obj_del_safe(lv_obj_t** obj);
void lv_obj_set_opa_scale(lv_obj_t* obj, lv_opa_t opa);
lv_opa_t lv_obj_get_opa_scale(lv_obj_t* obj);
void lv_label_set_text_add(lv_obj_t * label, const char * text);
lv_coord_t lv_obj_get_x_center(lv_obj_t * obj);
lv_coord_t lv_obj_get_y_center(lv_obj_t * obj);
void lv_table_set_align(lv_obj_t * table, lv_label_align_t align);
lv_anim_value_t lv_anim_path_strong_ease_in_out(const lv_anim_path_t * path, const lv_anim_t * a);
void lv_obj_add_anim(
    lv_obj_t * obj, lv_anim_t * a,
    lv_anim_exec_xcb_t exec_cb, 
    int32_t start, int32_t end,
    uint16_t time = LV_ANIM_TIME_DEFAULT,
    lv_anim_ready_cb_t ready_cb = NULL,
    lv_anim_path_cb_t path_cb = lv_anim_path_strong_ease_in_out
);
#define LV_OBJ_ADD_ANIM(obj,attr,target,time)\
do{\
    lv_obj_add_anim(\
        (obj), NULL,\
        (lv_anim_exec_xcb_t)lv_obj_set_##attr,\
        lv_obj_get_##attr(obj),\
        (target),\
        (time)\
    );\
}while(0)

#endif
