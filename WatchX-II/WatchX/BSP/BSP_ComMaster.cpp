#include "BSP.h"

#define CM_SERIAL Serial3

typedef enum
{
    CM_State_Idle,
    CM_State_GetLen,
    CM_State_RxData,
    CM_State_CheckData,
    CM_State_ExecuteData,
    CM_State_Error,
} CM_State_Type;

static ComMaster_UserCallback_TypeDef* CM_UserCallback_Head = NULL;
static ComMaster_UserCallback_TypeDef* CM_UserCallback_Tail = NULL;

static const uint8_t CM_DataHead = 0xA0;
static uint8_t CM_State = CM_State_Idle;
static uint8_t CM_RxID = 0;
static uint8_t CM_RxBuffer[64];
static uint8_t CM_RxLen = 0;
static uint8_t CM_RxCnt = 0;

static uint8_t CM_GetCheckSum(void* pData, uint32_t size)
{
    uint8_t* pdata = (uint8_t*)pData;
    uint8_t sum = 0;
    
    for(uint32_t i = 0; i < size; i++)
    {
        sum += pdata[i];
    }
    return sum;
}

static bool CM_ExecuteUserCallback(uint8_t id, void* buf, uint8_t size)
{
    if(id == 0)
        return false;
    
    bool retval = false;
    
    ComMaster_UserCallback_TypeDef* now = CM_UserCallback_Head;
    while(now != NULL)
    {
        if(id == (now->ID & 0x0F))
        {
            if(now->Func != NULL)
            {
                retval = now->Func(buf, size);
            }
            break;
        }

        now = now->Next;
    }
    return retval;
}

#define CM_STATE_JMP_EXE(state)\
do{\
    CM_State = CM_State_##state;\
    CM_StateUpdate(data);\
}while(0)

static void CM_StateUpdate(uint8_t data = 0)
{
    switch(CM_State)
    {
    case CM_State_Idle:
        if((data & 0xF0) == CM_DataHead)
        {
            CM_RxID = data & 0x0F;
            CM_State = CM_State_GetLen;
        }
        break;

    case CM_State_GetLen:
        if(data >= sizeof(CM_RxBuffer))
        {
            CM_STATE_JMP_EXE(Error);
        }
        else
        {
            CM_RxLen = data;
            CM_RxCnt = 0;
            memset(CM_RxBuffer, 0, sizeof(CM_RxBuffer));

            CM_State = CM_State_RxData;
        }
        break;

    case CM_State_RxData:
        if(CM_RxCnt < CM_RxLen)
        {
            CM_RxBuffer[CM_RxCnt] = data;
            CM_RxCnt++;
        }
        else
        {
            CM_STATE_JMP_EXE(CheckData);
        }
        break;

    case CM_State_CheckData:
        if(data == CM_GetCheckSum(CM_RxBuffer, CM_RxLen))
        {
            CM_STATE_JMP_EXE(ExecuteData);
        }
        else
        {
            CM_STATE_JMP_EXE(Error);
        }
        break;

    case CM_State_ExecuteData:
        if(CM_ExecuteUserCallback(CM_RxID, CM_RxBuffer, CM_RxLen) == true)
        {
            CM_State = CM_State_Error;
        }
        else
        {
            CM_State = CM_State_Idle;
        }
        break;

    case CM_State_Error:
        CM_STATE_JMP_EXE(Idle);
        break;

    default:
        break;
    }
}

void ComMaster_Init()
{
    CM_SERIAL.begin(115200);
}

void ComMaster_Update()
{
    while(CM_SERIAL.available())
    {
        CM_StateUpdate(CM_SERIAL.read());
    }
}

void ComMaster_AddUserCallbackNode(ComMaster_UserCallback_TypeDef* node)
{
    if(node == NULL)
        return;
    
    node->Next = NULL;
    
    if(CM_UserCallback_Head == NULL)
    {
        CM_UserCallback_Head = node;
    }
    else
    {
        CM_UserCallback_Tail->Next = node;
    }
    CM_UserCallback_Tail = node;
}
