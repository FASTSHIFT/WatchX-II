/*
 * MIT License
 * Copyright (c) 2018-2020 _VIFEXTech
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the follo18wing conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "PageManager.h"

#define IS_PAGE(page)   ((page)<(MaxPage))

#ifndef NULL
#   define NULL 0
#endif

/**
  * @brief  ��ʼ��ҳ�������
  * @param  pageMax: ҳ���������
  * @param  eventMax: �¼��������
  * @retval ��
  */
PageManager::PageManager(uint8_t pageMax, uint8_t pageStackSize)
{
    MaxPage = pageMax;
    NewPage = 0;
    OldPage = 0;
    IsPageBusy = false;

    /* �����ڴ棬����б� */
    PageList = new PageList_t[MaxPage];
    for(uint8_t page = 0; page < MaxPage; page++)
    {
        Clear(page);
    }
    
    /*ҳ��ջ����*/
    PageStackSize = pageStackSize;
    PageStack = new uint8_t[PageStackSize];
    StackClear();
}

/**
  * @brief  ҳ�����������
  * @param  ��
  * @retval ��
  */
PageManager::~PageManager()
{
    delete[] PageList;
    delete[] PageStack;
}

/**
  * @brief  ���һ��ҳ��
  * @param  pageID: ҳ����
  * @retval true:�ɹ� false:ʧ��
  */
bool PageManager::Clear(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return false;

    PageList[pageID].Callback = NULL;

    return true;
}

/**
  * @brief  ע��ҳ��
  * @param  pageID: ҳ����
  * @param  Callback: ��Ϣ�ص�����
  * @retval true:�ɹ� false:ʧ��
  */
bool PageManager::Register(uint8_t pageID, CallbackFunction_t callback, const char* name)
{
    if(!IS_PAGE(pageID))
        return false;

    PageList[pageID].Callback = callback;
    PageList[pageID].Name = name;
    return true;
}

/**
  * @brief  ҳ���¼�����
  * @param  obj: �����¼��Ķ���
  * @param  event: �¼����
  * @retval ��
  */
void PageManager::EventTransmit(void* obj, uint8_t event)
{
    /*���¼����ݵ���ǰҳ��*/
    if (PageList[NowPage].Callback != NULL)
    {
        PageList[NowPage].Callback(obj, event);
    }
}

/**
  * @brief  �л���ָ��ҳ��
  * @param  pageID: ҳ����
  * @retval ��
  */
void PageManager::ChangeTo(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return;
    
    /*���ҳ���Ƿ�æµ*/
    if(!IsPageBusy)
    {
        /*��ҳ��ID*/
        NextPage = NewPage = pageID;

        /*���Ϊæµ״̬*/
        IsPageBusy = true;
    }
}

/**
  * @brief  ҳ��ѹջ����ת����ҳ��
  * @param  pageID: ҳ����
  * @retval true:�ɹ� false:ʧ��
  */
bool PageManager::Push(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return false;
    
    /*���ҳ���Ƿ�æµ*/
    if(IsPageBusy)
       return false; 
    
    /*��ֹջ���*/
    if(PageStackTop >= PageStackSize - 1)
        return false;
    
    /*��ֹ�ظ�ҳ��ѹջ*/
    if(pageID == PageStack[PageStackTop])
        return false;

    /*ջ��ָ������*/
    PageStackTop++;
    
    /*ҳ��ѹջ*/
    PageStack[PageStackTop] = pageID;
    
    /*ҳ����ת*/
    ChangeTo(PageStack[PageStackTop]);
    
    return true;
}

/**
  * @brief  ҳ�浯ջ����ת����һ��ҳ��
  * @param  ��
  * @retval true:�ɹ� false:ʧ��
  */
bool PageManager::Pop()
{
    /*���ҳ���Ƿ�æµ*/
    if(IsPageBusy)
       return false; 
    
    /*��ֹջ���*/
    if(PageStackTop == 0)
        return false;
    
    /*��յ�ǰҳ��*/
    PageStack[PageStackTop] = 0;
    
    /*��ջ��ջ��ָ������*/
    PageStackTop--;
    
    /*ҳ����ת*/
    ChangeTo(PageStack[PageStackTop]);
    
    return true;
}

/**
  * @brief  ���ҳ��ջ
  * @param  ��
  * @retval ��
  */
void PageManager::StackClear()
{
    /*���ҳ���Ƿ�æµ*/
    if(IsPageBusy)
       return; 
    
    /*���ջ����������*/
    for(uint8_t i = 0; i < PageStackSize; i++)
    {
        PageStack[i] = 0;
    }
    /*ջ��ָ�븴λ*/
    PageStackTop = 0;
}

/**
  * @brief  ��ȡ��ǰҳ������
  * @param  ��
  * @retval ҳ������
  */
const char* PageManager::GetCurrentName()
{
    return PageList[NowPage].Name;
}

/**
  * @brief  ҳ�������״̬��
  * @param  ��
  * @retval ��
  */
void PageManager::Running()
{
    /*ҳ���л��¼�*/
    if(NewPage != OldPage)
    {
        /*���Ϊæµ״̬*/
        IsPageBusy = true;

        /*������ҳ���˳��¼�*/
        if (PageList[OldPage].Callback != NULL)
        {
            PageList[OldPage].Callback(this, MSG_Exit);
        }
        
        /*��Ǿ�ҳ��*/
        LastPage = OldPage;
        
        /*�����ҳ��Ϊ��ǰҳ��*/
        NowPage = NewPage;

        /*������ҳ���ʼ���¼�*/
        if (PageList[NewPage].Callback != NULL)
        {
            PageList[NewPage].Callback(this, MSG_Setup);
        }

        /*��ҳ���ʼ����ɣ����Ϊ��ҳ��*/
        OldPage = NewPage;

        /*���ҳ�治æµ������ѭ��״̬*/
        IsPageBusy = false;
    }
   
    /*ҳ��ѭ���¼�*/
    if (PageList[NowPage].Callback != NULL)
    {
        PageList[NowPage].Callback(this, MSG_Loop);
    }   
}
