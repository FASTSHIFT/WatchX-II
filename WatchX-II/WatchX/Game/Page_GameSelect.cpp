#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "GamePrivate.h"

PAGE_EXPORT(GameSelect);

static lv_obj_t * rollerGame;
static lv_obj_t * btnStart;

static void Obj_EventHandler(lv_obj_t * obj, lv_event_t event)
{
    static char name[32];
    if(obj == rollerGame)
    {
        if(event == LV_EVENT_VALUE_CHANGED)
        {
            lv_roller_get_selected_str(obj, name, sizeof(name));
        }
    }
    if(obj == btnStart)
    {
        if(event == LV_EVENT_CLICKED)
        {
            int index = Game_GetNameID(name);
            if(index > 0)
            {
                GamePage.ChangeTo(index);
                Page->Push(PAGE_Game);
            }
        }
    }
}

static void Creat_GameStartButton()
{
    btnStart = lv_btn_create(appWindow, NULL);

    lv_obj_set_size(btnStart, lv_obj_get_width(rollerGame), 50);
    lv_obj_align(btnStart, rollerGame, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_event_cb(btnStart, Obj_EventHandler);
    
    lv_obj_t * label = lv_label_create(btnStart, NULL);
    lv_label_set_text(label, "GO!");
}

static void Creat_RollerGame(void)
{
    rollerGame = lv_roller_create(appWindow, NULL);
    
    lv_roller_set_options(rollerGame, Game_GetNameList(), LV_ROLLER_MODE_INIFINITE);
    lv_roller_set_visible_row_count(rollerGame, 3);
    lv_obj_align(rollerGame, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(rollerGame, Obj_EventHandler);
}

/**
  * @brief  ҳ���ʼ���¼�
  * @param  ��
  * @retval ��
  */
static void Setup()
{
    /*����ҳ���Ƶ�ǰ̨*/
    lv_obj_move_foreground(appWindow);
    
    Game_Begin();
    
    Creat_RollerGame();
    Creat_GameStartButton();
}

/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit()
{
    lv_obj_clean(appWindow);
}

/**
  * @brief  ҳ���¼�
  * @param  event:�¼����
  * @param  param:�¼�����
  * @retval ��
  */
static void Event(void* obj, uint8_t event)
{

}
