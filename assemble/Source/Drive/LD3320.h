#ifndef __LD3320_H
#define	__LD3320_H

#include "Timer4.h"
#include "lcd_st7735.h"

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long

///��������״̬����������¼������������ASRʶ����������MP3����
#define LD_MODE_IDLE			0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 		0x40

///�������״̬����������¼������������ASRʶ������е��ĸ�״̬
#define LD_ASR_NONE					0x00	//��ʾû������ASRʶ��
#define LD_ASR_RUNING				0x01	//��ʾLD3320������ASRʶ����
#define LD_ASR_FOUNDOK			0x10	//��ʾһ��ʶ�����̽�������һ��ʶ����
#define LD_ASR_FOUNDZERO 		0x11	//��ʾһ��ʶ�����̽�����û��ʶ����
#define LD_ASR_ERROR	 			0x31	//	��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬

///�û��޸ĺ���
void  LD3320_main(void);
static uint8 LD_AsrAddFixed(void);
static void Board_text(uint8 Code_Val);
static void Delayms(uint16 i);


///��س�ʼ��
static void LD3320_init(void);
static void LD3320_GPIO_Cfg(void);
static void LD3320_EXTI_Cfg(void);
static void LD3320_SPI_cfg(void);
static void LED_GPIO_cfg(void);

///�м��
static void LD3320_delay(unsigned long uldata);
static uint8 RunASR(void);
static void LD_reset(void);
static void LD_AsrStart(void);
static uint8 LD_Check_ASRBusyFlag_b2(void);

///�Ĵ�������
static uint8 spi_send_byte(uint8 byte);
static void LD_WriteReg(uint8 data1,uint8 data2);
static uint8 LD_ReadReg(uint8 reg_add);
static uint8 LD_GetResult(void);
static uint8 LD_AsrRun(void);
static void ProcessInt(void);
static void LD_Init_Common(void);
static void LD_Init_ASR(void);

#endif /*__LD3320_H */
