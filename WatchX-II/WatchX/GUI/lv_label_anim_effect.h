#ifndef __LV_LABEL_ANIM_EFFECT_H
#define __LV_LABEL_ANIM_EFFECT_H

#include "lvgl/lvgl.h"

typedef struct{
    lv_obj_t* label_1;
    lv_obj_t* label_2;
    lv_anim_t anim_now;
    lv_anim_t anim_next;
    lv_coord_t y_offset;
    uint8_t value_last;
}lv_label_anim_effect_t;

void lv_label_anim_effect_init(
    lv_label_anim_effect_t* effect, 
    lv_obj_t* cont, 
    lv_obj_t* label_copy
);
void lv_label_anim_effect_check_value(
    lv_label_anim_effect_t* effect, 
    uint8_t value
);

#endif
