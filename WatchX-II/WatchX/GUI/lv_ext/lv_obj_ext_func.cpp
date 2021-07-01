#include "lv_obj_ext_func.h"

/**
  * @brief  ��ȫɾ�����󣬵�����ΪNULL��ʱ��ִ��ɾ��
  * @param  obj:���������ַ
  * @retval true �ɹ�; false ʧ��
  */
bool lv_obj_del_safe(lv_obj_t** obj)
{
    bool ret = false;
    if(*obj != NULL)
    {
        lv_obj_del(*obj);
        *obj = NULL;
        ret = true;
    }
    return ret;
}

void lv_obj_set_opa_scale(lv_obj_t* obj, lv_opa_t opa)
{
    lv_obj_set_style_local_opa_scale(obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, opa);
}

lv_opa_t lv_obj_get_opa_scale(lv_obj_t* obj)
{
    return lv_obj_get_style_opa_scale(obj, LV_OBJ_PART_MAIN);
}

/**
  * @brief  ��label��׷���ַ���
  * @param  label:��׷�ӵĶ���
  * @param  text:׷�ӵ��ַ���
  * @retval ��
  */
void lv_label_set_text_add(lv_obj_t * label, const char * text)
{
    if(!label)
        return;

    lv_label_ins_text(label, strlen(lv_label_get_text(label)), text);
}

/**
  * @brief  ��ȡ�����X��������
  * @param  obj:�����ַ
  * @retval X��������
  */
lv_coord_t lv_obj_get_x_center(lv_obj_t * obj)
{
    return (obj->coords.x2 + obj->coords.x1) / 2;
}

/**
  * @brief  ��ȡ�����Y��������
  * @param  obj:�����ַ
  * @retval Y��������
  */
lv_coord_t lv_obj_get_y_center(lv_obj_t * obj)
{
    return (obj->coords.y2 + obj->coords.y1) / 2;
}

/**
  * @brief  ����table��������г�Ա�Ķ��뷽ʽ
  * @param  table:�����ַ
  * @param  align:���䷽ʽ
  * @retval ��
  */
void lv_table_set_align(lv_obj_t * table, lv_label_align_t align)
{
    uint16_t col = lv_table_get_col_cnt(table);
    uint16_t row = lv_table_get_row_cnt(table);
    for(uint16_t i = 0; i < col; i++)
    {
        for(uint16_t j = 0; j < row; j++)
        {
            lv_table_set_cell_align(table, j, i, align);
        }
    }
}

/**
  * @brief  Ϊ������Ӷ���
  * @param  obj:�����ַ
  * @param  a:������������ַ
  * @param  exec_cb:���ƶ������Եĺ�����ַ
  * @param  start:�����Ŀ�ʼֵ
  * @param  end:�����Ľ���ֵ
  * @param  time:������ִ��ʱ��
  * @param  delay:������ʼǰ����ʱʱ��
  * @param  ready_cb:���������¼��ص�
  * @param  path_cb:��������
  * @retval ��
  */
void lv_obj_add_anim(
    lv_obj_t * obj, lv_anim_t * a,
    lv_anim_exec_xcb_t exec_cb,
    int32_t start, int32_t end,
    uint16_t time,
    uint32_t delay,
    lv_anim_ready_cb_t ready_cb,
    lv_anim_path_cb_t path_cb
)
{
    lv_anim_t anim_temp;
    
    if (a == NULL)
    {
        a = &anim_temp;

        /* INITIALIZE AN ANIMATION
        *-----------------------*/
        lv_anim_init(a);
    }
        
    /* MANDATORY SETTINGS
     *------------------*/

    /*Set the "animator" function*/
    lv_anim_set_exec_cb(a, exec_cb);

    /*Set the "animator" function*/
    lv_anim_set_var(a, obj);

    /*Length of the animation [ms]*/
    lv_anim_set_time(a, time);

    /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_values(a, start, end);
    

    /* OPTIONAL SETTINGS
     *------------------*/

    /*Time to wait before starting the animation [ms]*/
    lv_anim_set_delay(a, delay);

    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, path_cb);

    /*Set the path in an animation*/
    lv_anim_set_path(a, &path);

    /*Set a callback to call when animation is ready.*/
    lv_anim_set_ready_cb(a, ready_cb);

    /*Set a callback to call when animation is started (after delay).*/
    lv_anim_set_start_cb(a, ready_cb);

    /* START THE ANIMATION
     *------------------*/
    lv_anim_start(a);                             /*Start the animation*/
}
