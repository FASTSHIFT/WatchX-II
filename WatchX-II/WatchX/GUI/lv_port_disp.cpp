#include "GUI/DisplayPrivate.h"
#include "Basic/SysConfig.h"

/*屏幕缓冲区信息*/
static lv_disp_buf_t disp_buf;

/*乒乓缓冲区，缓冲20行像素*/
static lv_color_t lv_disp_buf0[1];
static lv_color_t lv_disp_buf1[LV_HOR_RES_MAX * 20];
static lv_color_t lv_disp_buf2[LV_HOR_RES_MAX * 20];

static lv_disp_drv_t* lv_disp_drv_p = NULL;
static TMR_Type* ParallelOccupiedTMR = NULL;
static DMA_Channel_Type* ParallelDMA = DMA1_Channel2;

extern "C"
{
    void DMA1_Channel2_IRQHandler()
    {
        ParallelOccupiedTMR->CTRL1 &= (uint16_t)(~((uint16_t)TMR_CTRL1_CNTEN)); // close timer
        DMA_ClearITPendingBit(DMA1_INT_TC2);
        
        DMA_ChannelEnable(ParallelDMA, DISABLE);
        
        digitalWrite_HIGH(TFT_CS_Pin);
        digitalWrite_HIGH(TFT_WR_Pin);
        pinMode(TFT_WR_Pin, OUTPUT);
        
        if(lv_disp_drv_p != NULL) 
        {
            lv_disp_flush_ready(lv_disp_drv_p); /* tell lvgl that flushing is done */
        }
    }
}

static void Parallel_Init()
{
    pinMode(TFT_PORT_Pin, OUTPUT);
   
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR2, ENABLE);
    TIM_TypeDef *occupiedTimer = ParallelOccupiedTMR = PIN_MAP[TFT_WR_Pin].TIMx;
    int timChannel = PIN_MAP[TFT_WR_Pin].TimerChannel;
    
    TMR_TimerBaseInitType timConfig;
    TMR_TimeBaseStructInit(&timConfig);
    timConfig.TMR_ClockDivision = TMR_CKD_DIV1;
    timConfig.TMR_CounterMode = TMR_CounterDIR_Up;
    timConfig.TMR_DIV = 10;
    timConfig.TMR_Period = 2;
    TMR_TimeBaseInit(occupiedTimer, &timConfig);
    
    TMR_OCInitType ocConfig;
    TMR_OCStructInit(&ocConfig);
    ocConfig.TMR_OCMode = TMR_OCMode_PWM1;
    ocConfig.TMR_OCPolarity = TMR_OCPolarity_Low;
    ocConfig.TMR_OutputState = TMR_OutputState_Enable;
    ocConfig.TMR_Pulse = 2;
    switch(timChannel)
    {
        case 1:
            TMR_OC1Init(occupiedTimer, &ocConfig);
        break;
        case 2:
            TMR_OC2Init(occupiedTimer, &ocConfig);
        break;
        case 3:
            TMR_OC3Init(occupiedTimer, &ocConfig);
        break;
        case 4:
            TMR_OC4Init(occupiedTimer, &ocConfig);
        break;
    }
    TMR_CtrlPWMOutputs(occupiedTimer, ENABLE);
    TMR_DMACmd(occupiedTimer, TMR_DMA_Update, ENABLE);
//    TMR_Cmd(occupiedTimer, ENABLE);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);
    DMA_InitType dmaConfig;
    dmaConfig.DMA_BufferSize = 0x04;
    dmaConfig.DMA_Direction = DMA_DIR_PERIPHERALDST;
    dmaConfig.DMA_MemoryBaseAddr = (uint32_t)0;
    dmaConfig.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
    dmaConfig.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    dmaConfig.DMA_Mode = DMA_MODE_NORMAL;
    dmaConfig.DMA_MTOM = DISABLE;
    dmaConfig.DMA_PeripheralBaseAddr = (uint32_t)&(PIN_MAP[TFT_PORT_Pin].GPIOx->OPTDT);
    dmaConfig.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
    dmaConfig.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    dmaConfig.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_Init(ParallelDMA, &dmaConfig);
    NVIC_EnableIRQ(DMA1_Channel2_IRQn);
    DMA_INTConfig(ParallelDMA, DMA_INT_TC, ENABLE);
//    DMA_ChannelEnable(ParallelDMA, ENABLE);
}

static void ParallelDMASendBuffer(void *addr, uint32_t length)
{
    noInterrupts();
    
    pinMode(TFT_WR_Pin, OUTPUT_AF_PP);
    
    ParallelDMA->CHCTRL &= (uint16_t)(~DMA_CHCTRL1_CHEN); // disable DMA
    ParallelOccupiedTMR->CTRL1 &= (uint16_t)(~((uint16_t)TMR_CTRL1_CNTEN)); // close timer
    ParallelOccupiedTMR->CNT = 0;
    
    DMA1->ICLR = DMA1_INT_TC2;
    
    ParallelDMA->TCNT = length - 0;
    ParallelDMA->CMBA = ((uint32_t)addr + 0);
//    *(uint8_t*)ParallelDMA->CPBA = *(uint8_t*)addr;
    
    ParallelDMA->CHCTRL |= DMA_CHCTRL1_CHEN;
    
    ParallelOccupiedTMR->CTRL1 |= TMR_CTRL1_CNTEN;
    
    interrupts();
    
//    ParallelOccupiedTMR->EVEG = TMR_EventSource_Update;
    
}

static void ParallelDMADisplay(const lv_area_t *area, uint16_t *color_p)
{
    pinMode(TFT_WR_Pin, OUTPUT);
    lv_coord_t height = (area->y2 - area->y1 + 1);
    lv_coord_t width = (area->x2 - area->x1 + 1);
    screen.setAddrWindow(area->x1, area->y1, area->x2, area->y2);
    digitalWrite_HIGH(TFT_RS_Pin);
    digitalWrite_LOW(TFT_CS_Pin);
    pinMode(TFT_WR_Pin, OUTPUT_AF_PP);
    ParallelDMASendBuffer(color_p, width * height * sizeof(lv_color_t));
}

/**
  * @brief  屏幕刷新回调函数
  * @param  disp:屏幕驱动地址
  * @param  area:刷新区域
  * @param  color_p:刷新缓冲区地址
  * @retval 无
  */
static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    lv_disp_drv_p = disp;
    
    lv_coord_t height = (area->y2 - area->y1 + 1);
    lv_coord_t width = (area->x2 - area->x1 + 1);
    if(height * width <= 10)
    {
        screen.drawFastRGBBitmap(area->x1, area->y1, (uint16_t*)color_p, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1));
        lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
    }
    else
    {
        ParallelDMADisplay(area, (uint16_t*)color_p);
    }
}

/**
  * @brief  屏幕初始化
  * @param  无
  * @retval 无
  */
void lv_port_disp_init()
{
    lv_disp_buf_init(&disp_buf, lv_disp_buf1, lv_disp_buf2, sizeof(lv_disp_buf1) / sizeof(lv_color_t));
    
    Parallel_Init();
    ParallelDMASendBuffer(lv_disp_buf0, 1);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);
}
