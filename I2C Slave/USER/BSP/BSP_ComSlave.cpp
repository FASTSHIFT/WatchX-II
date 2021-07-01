#include "BSP.h"

#define CS_SERIAL Serial2

typedef enum
{
    CS_State_Idle,
    CS_State_GetUserData,
    CS_State_CheckData,
    CS_State_TxData,
    CS_State_Error,
} CS_State_Type;

static const ComSlave_UserCallback_TypeDef* CS_CurrentUserCallback = NULL;

static const uint8_t CS_DataHead = 0xA0;
static uint8_t CS_State = CS_State_Idle;
static uint8_t CS_TxID = 0;
static uint8_t CS_TxBuffer[64 + 3];
static uint8_t CS_TxLen = 0;

static void CS_WriteBuf(const void* buf, uint32_t len)
{
    CS_SERIAL.write(buf, len);
}

static uint8_t CS_GetCheckSum(void* pData, uint32_t size)
{
    uint8_t* pdata = (uint8_t*)pData;
    uint8_t sum = 0;
    
    for(uint32_t i = 0; i < size; i++)
    {
        sum += pdata[i];
    }
    return sum;
}

#define CS_STATE_JMP_EXE(state)\
do{\
    CS_State = CS_State_##state;\
    CS_StateUpdate();\
}while(0)

static void CS_StateUpdate()
{
    switch(CS_State)
    {
    case CS_State_Idle:
        break;

    case CS_State_GetUserData:
        memset(CS_TxBuffer, 0, sizeof(CS_TxBuffer));
        if( CS_CurrentUserCallback == NULL
        ||  CS_CurrentUserCallback->Func == NULL)
        {
            CS_STATE_JMP_EXE(Error);
        }
        else
        {
            CS_TxLen = CS_CurrentUserCallback->Func(CS_TxBuffer + 2, sizeof(CS_TxBuffer) - 3);
            CS_STATE_JMP_EXE(CheckData);
        }
        break;

    case CS_State_CheckData:
        CS_TxID = (CS_CurrentUserCallback->ID & 0x0F);
        if( CS_TxLen > (sizeof(CS_TxBuffer) - 3)
        ||  CS_TxLen == 0
        ||  CS_TxID == 0x00
        )
        {
            CS_STATE_JMP_EXE(Error);
        }
        else
        {
            CS_TxBuffer[0] = CS_DataHead | CS_TxID;
            CS_TxBuffer[1] = CS_TxLen;
            uint8_t checkSum = CS_GetCheckSum(CS_TxBuffer + 2, CS_TxLen);
            CS_TxBuffer[2 + CS_TxLen] = checkSum;
            CS_STATE_JMP_EXE(TxData);
        }
        break;

    case CS_State_TxData:
        CS_WriteBuf(CS_TxBuffer, 2 + CS_TxLen + 1);
        CS_State = CS_State_Idle;
        CS_CurrentUserCallback = NULL;
        break;

    case CS_State_Error:
        CS_State = CS_State_Idle;
        CS_CurrentUserCallback = NULL;
        break;

    default:
        break;
    }
}

void ComSlave_Init()
{
    CS_SERIAL.begin(115200);
}

void ComSlave_Update()
{
    CS_StateUpdate();
}

bool ComSlave_ReqExecuteUserCallbackNode(const ComSlave_UserCallback_TypeDef* node)
{
    if(CS_CurrentUserCallback != NULL
    || CS_State != CS_State_Idle)
    {
        return false;
    }
    
    CS_CurrentUserCallback = node;
    CS_State = CS_State_GetUserData;
    return true;
}
