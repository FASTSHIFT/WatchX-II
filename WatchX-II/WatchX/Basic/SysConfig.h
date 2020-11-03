#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

#define WX_NAME             "WatchX-II"  //手表名称
#define WX_HARDWARE_VERSION "v1.0"       //硬件版本
#define WX_SOFTWARE_VERSION "v0.6"       //软件版本

/*使用内置I2C读取传感器*/
#define WX_USE_BUILT_IN_I2C 0

/*空引脚*/
#define NULL_PIN            PD0

/*屏幕*/
#define TFT_PORT_Pin        PB0
#define TFT_RES_Pin         PA4
#define TFT_CS_Pin          PA5
#define TFT_RS_Pin          PA8
#define TFT_WR_Pin          PA3
#define TFT_RD_Pin          NULL_PIN
#define TFT_IM_Pin          NULL_PIN
#define TFT_BLK_Pin         PA11

/*按键*/
#define KEY_Pin             PA0

/*电源使能保持*/
#define POWER_EN_Pin        PA12

/*电池检测*/
#define BAT_DET_Pin         PA1
#define BAT_DET_EN_Pin      PA2
#define BAT_CHG_DET_Pin     PC13

/*振动电机*/
#define MotorLRA_IN1_Pin    PA7
#define MotorLRA_IN2_Pin    PA6

/*传感器I2C*/
#define SENSOR_SCL_Pin      PB10
#define SENSOR_SDA_Pin      PB11
#define SENSOR_I2C          I2C2

/*触摸数据就绪脚*/
#define TP_EINT_Pin         PB12

/*SD卡 SPI接口*/
#define SD_CS_Pin           PA15
#define SD_SCLK_Pin         PB13
#define SD_DO_Pin           PB14
#define SD_DI_Pin           PB15

/*加速度计中断*/
#define LIS_INT1_Pin        PB8
#define LIS_INT2_Pin        PB9

#endif
