#include "Adafruit_ST7789.h"


#if defined(__STM32__)
#define TFT_RST_SET     GPIO_HIGH(rstport,rstpinmask)
#define TFT_CS_SET      GPIO_HIGH(csport,cspinmask)
#define TFT_RS_SET      GPIO_HIGH(rsport,rspinmask)
#define TFT_RW_SET      GPIO_HIGH(rwport,rwpinmask)
#define TFT_RD_SET      GPIO_HIGH(rdport,rdpinmask)

#define TFT_RST_CLR     GPIO_LOW(rstport,rstpinmask)
#define TFT_CS_CLR      GPIO_LOW(csport,cspinmask)
#define TFT_RS_CLR      GPIO_LOW(rsport,rspinmask)
#define TFT_RW_CLR      GPIO_LOW(rwport,rwpinmask)
#define TFT_RD_CLR      GPIO_LOW(rdport,rdpinmask)
#else
#define TFT_RST_SET     digitalWrite(RST,HIGH)
#define TFT_CS_SET      digitalWrite(CS,HIGH)
#define TFT_RS_SET      digitalWrite(RS,HIGH)
#define TFT_RW_SET      digitalWrite(RW,HIGH)
#define TFT_RD_SET      digitalWrite(RD,HIGH)

#define TFT_RST_CLR     digitalWrite(RST,LOW)
#define TFT_CS_CLR      digitalWrite(CS,LOW)
#define TFT_RS_CLR      digitalWrite(RS,LOW)
#define TFT_RW_CLR      digitalWrite(RW,LOW)
#define TFT_RD_CLR      digitalWrite(RD,LOW)
#endif

Adafruit_ST7789::Adafruit_ST7789(
    uint8_t port_start, uint8_t rst, uint8_t cs, uint8_t rs, uint8_t rw, uint8_t rd)
    : Adafruit_GFX(ST7789_TFTWIDTH, ST7789_TFTHEIGHT)
{
    PORT_START = port_start;
    TFT_Port = portOutputRegister(digitalPinToPort(PORT_START));
    
    RST = rst;
    CS = cs;
    RS = rs;
    RW = rw;
    RD = rd;

#if defined(__STM32__)
    rstport = digitalPinToPort(RST);
    rstpinmask = digitalPinToBitMask(RST);
    csport = digitalPinToPort(CS);
    cspinmask = digitalPinToBitMask(CS);
    rsport = digitalPinToPort(RS);
    rspinmask = digitalPinToBitMask(RS);
    rwport = digitalPinToPort(RW);
    rwpinmask = digitalPinToBitMask(RW);
    rdport = digitalPinToPort(RD);
    rdpinmask = digitalPinToBitMask(RD);
#endif
}

void Adafruit_ST7789::begin()
{
    for(uint8_t pin = PORT_START; pin < PORT_START + 8; pin++)
    {
        pinMode(pin, OUTPUT);
    }

    TFT_RST_SET;
    TFT_CS_SET;
    TFT_RS_SET;
    TFT_RW_SET;
    TFT_RD_SET;
    
    pinMode(RST, OUTPUT);
    pinMode(CS, OUTPUT);
    pinMode(RS, OUTPUT);
    pinMode(RW, OUTPUT);
    pinMode(RD, OUTPUT);
    
    
    delay(200);
    TFT_RST_SET;
    delayMicroseconds(100);
    TFT_RST_CLR;
    delayMicroseconds(5);
    TFT_RST_SET;
    delay(200);
    
////    writeCommand( 0x01);
//    delay(500);
    

    //************* Start Initial Sequence **********//
    writeCommand( 0x36);   //Memory data access control  
    writeData( 0x00);   

    writeCommand( 0x3A);  //Interface pixel format    
    writeData( 0x55);//65K RGB,16bit/pixel  

    writeCommand( 0xB2); //P260 Porch Setting  
    writeData( 0x0C); //BPA[6:0]: Back porch setting in normal mode. The minimum setting is 0x01.   
    writeData( 0x0C); //FPA[6:0]: Front porch setting in normal mode. The minimum setting is 0x01.  
    writeData( 0x00); //PSEN: Enable separate porch control. 0 : Disable separate porch control , 1:Enable separate porch control 
    writeData( 0x33); //BPB[3:0]: Back porch setting in idle mode. The minimum setting is 0x01. FPB[3:0]: Front porch setting in idle mode. The minimum setting is 0x01
    writeData( 0x33); //BPC[3:0]: Back porch setting in partial mode. The minimum setting is 0x01. FPC[3:0]: Front porch setting in partial mode. The minimum setting is 0x01  
    
    writeCommand( 0xB7);   //Gate Control
    writeData( 0x35);
    
    writeCommand( 0xBB);   //VCOM  Setting 
    writeData( 0x1A);      // 0.75V

    writeCommand( 0xC0);   //LCM Control   
    writeData( 0x2C);   

    writeCommand( 0xC2);   //VDV and VRH Command Enable   
    writeData( 0x01);      //CMDEN=¡±1¡±, VDV and VRH register value comes from command write. 

    writeCommand( 0xC3);   //GVDD VRH Set  
    writeData( 0x0F);      //4.3V 4.3+( vcom+vcom offset+0.5vdv)  

    writeCommand( 0xC4);   //VDV Set   
    writeData( 0x20);      //Default Value :0V

    writeCommand( 0xC6);   //Frame Rate Control in Normal Mode   
    writeData( 0x0F);      //Dot INV, 60Hz  

    writeCommand( 0xD0);   //Power Control 1  
    writeData( 0xA4);   
    writeData( 0xA1);      //AVDD 6.8V,AVCL -4.8V,VDS 2.19V

    writeCommand( 0xE0);   //Positive Voltage Gamma Control  
    writeData( 0xD0);   
    writeData( 0x13);   
    writeData( 0x1A);   
    writeData( 0x0A);   
    writeData( 0x0A);   
    writeData( 0x26);   
    writeData( 0x3F);   
    writeData( 0x54);   
    writeData( 0x54);   
    writeData( 0x18);   
    writeData( 0x14);   
    writeData( 0x14);   
    writeData( 0x30);   
    writeData( 0x33);   

    writeCommand( 0xE1);   //Negative Voltage Gamma Control 
    writeData( 0xD0);   
    writeData( 0x13);   
    writeData( 0x1A);   
    writeData( 0x0A);   
    writeData( 0x0A);   
    writeData( 0x26);   
    writeData( 0x3F);   
    writeData( 0x54);   
    writeData( 0x54);   
    writeData( 0x1A);   
    writeData( 0x16);   
    writeData( 0x16);   
    writeData( 0x32);   
    writeData( 0x35);    

//    writeCommand( 0xE7);     //SPI2 Enable 
//    writeData( 0x00);   //10-2DATA   00-1DATA

    writeCommand( 0x29);     //Display on 
    delay(120);
    writeCommand( 0x11);
    delay(120); //ms

    writeCommand( 0x21);     //Display inversion on 
}

//void Adafruit_ST7789::PORT_WRITE(uint8_t dat)
//{
//    uint32_t *port = (uint32_t *)TFT_Port;
//    
//    *port = (*port &~ 0xff) | dat;
//    TFT_RW_CLR;
//    TFT_RW_SET;
//}

//#define PORT_WRITE(data) (TFT_RW_CLR,*(uint8_t*)TFT_Port = (uint8_t)(data),TFT_RW_SET)
//((*(uint8_t*)TFT_Port) = (uint8_t)data);\
//__IO uint32_t *port = (uint32_t *)TFT_Port;\
//*port = (*port &~ 0xff) | (uint8_t)data;\

#define PORT_WRITE(data) \
do{\
    TFT_RW_CLR;\
    ((*TFT_Port) = (uint8_t)data);\
    TFT_RW_SET;\
}while(0)

uint8_t Adafruit_ST7789::PORT_READ()
{
    delayMicroseconds(10); 
    TFT_RD_CLR;
    delayMicroseconds(10); 
    uint8_t dat = (GPIOB->IPTDT) & 0xff;
    delayMicroseconds(10); 
    TFT_RD_SET;
    return dat;
}

void Adafruit_ST7789::read(uint8_t cmd, void *dat, int len)
{
    TFT_CS_CLR;
    TFT_RS_CLR;

    PORT_WRITE(cmd);
    
    TFT_RS_SET;
    
    delayMicroseconds(10); 
    
    for(uint8_t pin = PORT_START; pin < PORT_START + 8; pin++)
    {
        pinMode(pin, INPUT_PULLDOWN);
    }
    PORT_READ();
    for(int i = 0; i < len; i++)
    {
        ((uint8_t*)dat)[i] = PORT_READ();
    }

    TFT_CS_SET;
    
    for(uint8_t pin = PORT_START; pin < PORT_START + 8; pin++)
    {
        pinMode(pin, OUTPUT);
    }
}

void Adafruit_ST7789::writeCommand(uint8_t cmd)
{
    TFT_CS_CLR;
    TFT_RS_CLR;

    PORT_WRITE(cmd);

    TFT_CS_SET;
}

void Adafruit_ST7789::writeData(uint8_t data)
{
    TFT_CS_CLR;
    TFT_RS_SET;

    PORT_WRITE(data);

    TFT_CS_SET;
}

void Adafruit_ST7789::writeData16(uint16_t data)
{
    TFT_CS_CLR;
    TFT_RS_SET;

    PORT_WRITE((data >> 8));
    PORT_WRITE(data & 0XFF);

    TFT_CS_SET;
}

void Adafruit_ST7789::setRotation(uint8_t r)
{
    rotation = r % 4;
    switch(rotation)
    {
    case 0:
        _width = ST7789_TFTWIDTH;
        _height = ST7789_TFTHEIGHT;

        writeCommand(0x36);
        writeData(0x00);
        break;

    case 1:
        _width = ST7789_TFTHEIGHT;
        _height = ST7789_TFTWIDTH;

        writeCommand(0x36);
        writeData(0xA0);
        break;

    case 2:
        _width = ST7789_TFTWIDTH;
        _height = ST7789_TFTHEIGHT;

        writeCommand(0x36);
        writeData(0xC0);
        break;

    case 3:
        _width = ST7789_TFTHEIGHT;
        _height = ST7789_TFTWIDTH;

        writeCommand(0x36);
        writeData(0x70);
        break;
    }
}

void Adafruit_ST7789::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
//    if(x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0)
//        return;
    
    writeCommand(0x2A); //Column Address Set
    writeData16(x0);
    writeData16(x1);

    writeCommand(0x2B); //Row Address Set
    writeData16(y0);
    writeData16(y1);
    
    writeCommand(0x2C);  //Memory Write 
}

void Adafruit_ST7789::setCursor(int16_t x, int16_t y)
{
    cursor_x = x;
    cursor_y = y;
    if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
    setAddrWindow(x, y, x + 1, y + 1);
}

void Adafruit_ST7789::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
    setAddrWindow(x, y, x + 1, y + 1);
    writeData16(color);
}

void Adafruit_ST7789::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    // Rudimentary clipping
    if((x >= _width) || (y >= _height) || h < 1) return;
    if((y + h - 1) >= _height) h = _height - y;
    if(x < 0) x = 0;
    if(y < 0) y = 0;

    setAddrWindow(x, y, x, y + h - 1);

    while (h--)
        writeData16(color);
}

void Adafruit_ST7789::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    // Rudimentary clipping
    if((x >= _width) || (y >= _height) || w < 1) return;
    if((x + w - 1) >= _width)  w = _width - x;
    if(x < 0) x = 0;
    if(y < 0) y = 0;

    setAddrWindow(x, y, x + w - 1, y);

    while (w--)
        writeData16(color);
}

void Adafruit_ST7789::drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
    if((x >= _width) || (y >= _height)) return;

    int16_t actual_cursor_x = x;
    int16_t actual_cursor_y = y;
    int16_t actual_cursor_x1 = x + w - 1;
    int16_t actual_cursor_y1 = y + h - 1;

    if(actual_cursor_x < 0)
    {
        actual_cursor_x = 0;
    }
    else if(actual_cursor_x >= _width)
    {
        actual_cursor_x = _width - 1;
    }

    if(actual_cursor_y < 0)
    {
        actual_cursor_y = 0;
    }
    else if(actual_cursor_y >= _height)
    {
        actual_cursor_y = _height - 1;
    }

    if(actual_cursor_x1 < 0)
    {
        actual_cursor_x1 = 0;
    }
    else if(actual_cursor_x1 >= _width)
    {
        actual_cursor_x1 = _width - 1;
    }

    if(actual_cursor_y1 < 0)
    {
        actual_cursor_y1 = 0;
    }
    else if(actual_cursor_y1 >= _height)
    {
        actual_cursor_y1 = _height - 1;
    }

    setAddrWindow(actual_cursor_x, actual_cursor_y, actual_cursor_x1, actual_cursor_y1);

    for(int16_t Y = 0; Y < h; Y++)
    {
        for(int16_t X = 0; X < w; X++)
        {
            uint32_t index = (uint32_t)X + (uint32_t)Y * w;
            int16_t actualX = x + X;
            int16_t actualY = y + Y;
            if(actualX >= 0 && actualX < _width && actualY >= 0 && actualY < _height)
            {
                writeData16(bitmap[index]);
            }
        }
    }
}

void Adafruit_ST7789::fillScreen(uint16_t color)
{
    setAddrWindow(0, 0, _width - 1, _height - 1);
    uint32_t size = (_width * _height);
    TFT_CS_CLR;
    TFT_RS_SET;
    for(uint32_t i = 0; i < size; i++)
    {
        PORT_WRITE((color >> 8));
        PORT_WRITE(color);
    }
    TFT_CS_SET;
}

void Adafruit_ST7789::drawFastRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    uint32_t size = w * h;
    TFT_CS_CLR;
    TFT_RS_SET;
    for(uint32_t i = 0; i < size; i++)
    {
        PORT_WRITE((bitmap[i] >> 8));
        PORT_WRITE(bitmap[i]);
    }
    TFT_CS_SET;
}

