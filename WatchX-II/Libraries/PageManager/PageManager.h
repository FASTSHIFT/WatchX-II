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
 
/**
  ******************************************************************************
  * @file    PageManager.h
  * @author  _VIFEXTech
  * @version v1.4
  * @date    2020-7-28
  * @brief   基于状态机的异步页面调度器，不依赖任何GUI框架，支持操作系统
  * @finished: 2018.10.6  - v1.0 拥有简单的页面切换管理，事件传递调度
  * @upgrade:  2019.4.1   - v1.1 增加注释，重写事件传递调度，减少重复代码
  * @upgrade:  2019.10.5  - v1.2 使用栈结构管理页面嵌套，默认栈深度10层(可调)
  * @upgrade:  2019.12.4  - v1.3 修复在Setup和Exit阻塞情况下页面切换的BUG，添加析构
  * @upgrade:  2020.7.28  - v1.4 修改EventFunction_t形参顺序，判断指针使用 "ptr != NULL" 的形式，添加MIT开源协议
  * @upgrade:  2020.11.3  - v1.5 将事件回调改成消息机制，统一使用一个回调函数处理；
  *                              修改函数命名，去除Page前缀；
  *                              为每个页面添加Name，可使用GetCurrentName()访问当前页面的名字
  * @upgrade:  2020.11.6  - v1.6 优化Running函数，去除其中的IS_PAGE判断
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PAGEMANAGER_H
#define __PAGEMANAGER_H

#include <stdint.h>

class PageManager {
    typedef void(*CallbackFunction_t)(void*,uint8_t);
    typedef struct PageList{
        CallbackFunction_t Callback;
        const char* Name;
    } PageList_t;
public:
    PageManager(uint8_t pageMax, uint8_t pageStackSize = 10);
    ~PageManager();
    uint8_t NowPage, LastPage, NextPage;
    uint8_t NewPage, OldPage;
    typedef enum {
        MSG_NONE,
        MSG_Setup,
        MSG_Exit,
        MSG_Loop,
        MSG_MAX
    }MSG_t;

    bool Register(uint8_t pageID, CallbackFunction_t callback, const char* name);
    bool Clear(uint8_t pageID);
    bool Push(uint8_t pageID);
    bool Pop();
    void ChangeTo(uint8_t pageID);
    void EventTransmit(void* obj, uint8_t event);
    void StackClear();
    const char* GetCurrentName();
    void Running();
private:
    PageList_t* PageList;
    uint8_t *PageStack;
    uint8_t PageStackSize;
    uint8_t PageStackTop;
    uint8_t MaxPage;
    bool IsPageBusy;
};

#endif
