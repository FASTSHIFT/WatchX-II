#ifndef MM_H
#define MM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MM_USE_SYS_ALLOC 1

#if MM_USE_SYS_ALLOC
#define MM_Alloc(s)         malloc(s)
#define MM_Free(p)          free(p)
#define MM_Set(p, val, s)   memset(p, val, s)
#else
#define MM_Alloc(s)         _MM_Alloc(s)
#define MM_Free(p)          _MM_Free(p)
#define MM_Set(p, val, s)   _MM_Set(p, val, s)
#endif

/*Pool_Size ÄÚ´æ³Ø´óÐ¡*/
#define Pool_Size ( 4 * 1024 )

void MM_MemPoolInit(void);
uint16_t MM_GetTableIndex(void* Block);
void* _MM_Alloc(size_t s);
uint8_t _MM_Free(void* Block);
uint8_t _MM_Set(void *Block, int val, size_t s);
int MM_Occupation(void);
    
#ifdef __cplusplus
}// extern "C"
#endif

#endif //!MM_H


