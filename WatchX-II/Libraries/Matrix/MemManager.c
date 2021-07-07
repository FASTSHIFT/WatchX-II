/*
    MemManager内存管理器
    轻量化的内存管理器
    ！！本管理器用于操作限定的连续的内存区域
    ！！本内存管理器的所有操作都基于本管理器内存池，无法操作本内存池以外的内存
    
    作者：Trigger
    日期：2020.12.25
*/

#include "MM.h"

#define USE_DEBUG 0

#if ( USE_DEBUG != 0 )
#include <stdio.h>
#define DEBUG_LOG(format, ...)\
do{\
    printf("File:%s Line:%d Function:%s >>", __FILE__, __LINE__, __FUNCTION__);\
    printf(format, ##__VA_ARGS__);\
    printf("\r\n");\
    while(1);\
}while(0)
#else
#define DEBUG_LOG(...)
#endif

/*内存池大小在MM.h中修改*/
/*AllocTabel_Size 内存分配表大小*/
#define AllocTabel_Size (Pool_Size/8)

/*memPool 内存池*/
static uint8_t memPool[Pool_Size];
/*memAllocTabel 内存分配表*/
static uint8_t memAllocTabel[AllocTabel_Size];

/**
  * @brief  MemPoolInit 初始化内存池和内存分配表
  * @param  无
  * @retval 无
  */
void MM_MemPoolInit()
{
    for (int i = 0; i < Pool_Size; i++)
    {
        memPool[i] = 0;
        if(memPool[i])
            memAllocTabel[i / 8] = memAllocTabel[i / 8] |= (1 << i % 8);
        else
            memAllocTabel[i / 8] = memAllocTabel[i / 8] &= (~(1 << i % 8));
    }
}


/**
  * @brief  SpaceSearch 内存空间查找（按照内存分配表查找）
  * @param  s：(size_t)空间大小
  * @retval (int16_t)合适空间首元素下标(未找到空间返回-1)
  */
static int16_t MM_SpaceSearch(size_t s)
{
    size_t spaceSize = 0;
    for (int i = 0; i < Pool_Size; i++)
    {
        uint8_t tmp = memAllocTabel[i / 8];
        if (!(tmp & (0x80 >> (i % 8))))
            spaceSize++;
        else
            spaceSize = 0;
        if (spaceSize == s + 2)
        {
            return (int16_t)(i - s);
        }
    }
    return -1;
}

/**
  * @brief  GetTableIndex 获取首元素的分配表下标
  * @param  Block (void*)目标地址
  * @retval (uint16_t)分配表下标
  */
uint16_t MM_GetTableIndex(void* Block)
{
    uint8_t* poolP = memPool;
    return (uint16_t)(((uint8_t*)Block - poolP) / 8);
}

/**
  * @brief  Alloc 申请内存
  * @param  s：(size_t)空间大小
  * @retval (void*)空间地址
  */
void* _MM_Alloc(size_t s)
{
    int spaceIndex = MM_SpaceSearch(s);
    if (spaceIndex == -1)
    {
        DEBUG_LOG("Space Search Faild");
        return NULL;
    }    
    for (size_t i = spaceIndex; i < spaceIndex + s; i++)
    {
        memAllocTabel[i / 8] = memAllocTabel[i / 8] |= (0x80 >> i % 8);
    }
    return &memPool[spaceIndex];
}

/**
  * @brief  Free 释放内存
  * @param  Block (void*)目标地址
  * @retval (uint8_t)释放结果
  */
uint8_t _MM_Free(void* Block)
{
    if (Block==NULL)
    {
        DEBUG_LOG("Block is NULL!");
        return 0;
    }
    uint8_t* poolP = memPool;
    uint16_t blockIndex = (uint16_t)((uint8_t*)Block - poolP);
    uint16_t pDev = blockIndex;
    while ((memAllocTabel[pDev / 8] & (0x80 >> (pDev % 8))))
    {
        memAllocTabel[pDev / 8] = memAllocTabel[pDev / 8] &= (~(0x80 >> pDev % 8));
        pDev++;
    }
    if(!(memAllocTabel[blockIndex / 8] & (0x80 >> (blockIndex % 8))))
        return 1;
    else
        return 0;
}

/**
  * @brief  Set 设置内存值
  * @param  Block (void*)目标地址
  * @param  val (int)目标值
  * @param  s (size_t)目标大小
  * @retval (uint8_t)设置结果
  */
uint8_t _MM_Set(void *Block, int val, size_t s)
{
    uint8_t *pBlock = (uint8_t*)Block;
    size_t valBlock = (size_t)Block;
    if(pBlock >= &memPool[0] && pBlock <= &memPool[Pool_Size-1])
    {
        size_t blockIndex = valBlock - (size_t)memPool;
        for(size_t i = blockIndex;i<blockIndex+s;i++)
            memPool[i] = val;
    }
    else
        return 0;
    return 1;
}

/**
  * @brief  Occupation 查看占用情况
  * @param  无
  * @retval (int)已占用字节数
  */
int MM_Occupation()
{
    int count=0;
    for (int i = 0; i < Pool_Size; i++)
    {
        uint8_t tmp = memAllocTabel[i / 8];
        if ((tmp & (0x80 >> (i % 8))))
            count++;
    }
    return count;
}
