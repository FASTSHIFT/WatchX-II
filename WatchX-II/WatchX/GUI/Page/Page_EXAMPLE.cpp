#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

/*����ҳ��*/
PAGE_EXPORT(EXAMPLE);

/**
  * @brief  ҳ���ʼ���¼�
  * @param  ��
  * @retval ��
  */
static void Setup()
{
    /*����ҳ���Ƶ�ǰ̨*/
    lv_obj_move_foreground(appWindow);
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
  * @param  obj:�����¼��Ķ���
  * @param  event:�¼����
  * @retval ��
  */
static void Event(void* obj, uint8_t event)
{
    (void)Page;
}
