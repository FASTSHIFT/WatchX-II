#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

#define WX_NAME             "WatchX-II"  //�ֱ�����
#define WX_HARDWARE_VERSION "v1.0"       //Ӳ���汾
#define WX_SOFTWARE_VERSION "v0.8"       //����汾

/*ʹ������I2C��ȡ������*/
#define WX_USE_BUILT_IN_I2C 1

/*������*/
#define NULL_PIN            PD0

/*��Ļ*/
#define TFT_PORT_Pin        PB0
#define TFT_RES_Pin         PA4
#define TFT_CS_Pin          PA5
#define TFT_RS_Pin          PA8
#define TFT_WR_Pin          PA3
#define TFT_RD_Pin          NULL_PIN
#define TFT_IM_Pin          NULL_PIN
#define TFT_BLK_Pin         PA11

/*����*/
#define KEY_Pin             PA0

/*��Դʹ�ܱ���*/
#define POWER_EN_Pin        PA12

/*��ؼ��*/
#define BAT_DET_Pin         PA1
#define BAT_DET_EN_Pin      PA2
#define BAT_CHG_DET_Pin     PC13

/*�񶯵��*/
#define MotorLRA_IN1_Pin    PA7
#define MotorLRA_IN2_Pin    PA6

/*������I2C*/
#define SENSOR_SCL_Pin      PB10
#define SENSOR_SDA_Pin      PB11
#define SENSOR_I2C          I2C2

/*�������ݾ�����*/
#define TP_EINT_Pin         PB12

/*SD�� SPI�ӿ�*/
#define SD_CS_Pin           PA15
#define SD_SCLK_Pin         PB13
#define SD_DO_Pin           PB14
#define SD_DI_Pin           PB15

/*���ٶȼ��ж�*/
#define LIS_INT1_Pin        PB8
#define LIS_INT2_Pin        PB9

#endif
