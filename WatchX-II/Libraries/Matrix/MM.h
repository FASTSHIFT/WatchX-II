#ifndef MM_H
#define MM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/*Pool_Size ÄÚ´æ³Ø´óÐ¡*/
#define Pool_Size ( 4 * 1024 )

void MM_MemPoolInit(void);
uint16_t MM_GetTableIndex(void* Block);
void* MM_Alloc(size_t s);
uint8_t MM_Free(void* Block);
uint8_t MM_Set(void *Block, int val, size_t s);
int MM_Occupation(void);
    
#ifdef __cplusplus
}// extern "C"
#endif

#endif //!MM_H


