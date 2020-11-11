#ifndef __LCD_H
#define __LCD_H		
/***********************************************************************************************************************************
    *	@file  	  w25qxx.c
	*   @date     2020-08-17		
	*	@author   ħŮ������   ����
    *   @brief    �������     QȺ�ļ��У�262901124  ��������
    *   @taobao   �Ա�Ӳ��     ttps://demoboard.taobao.com/
    ********************************************************************************************************************************
    ** 1.8���ɫ��ʾ��ģ������˵��(ST7735)
    ** GND         ��Դ��
    ** VCC         3.3V����Ҫ��5V, ģ�鲻��LDO��5V��������
    ** SCL/SCK     SPI��SCK����  
    ** SDA/DIN     SPI��MOSI���� 
    ** RES/RST     ��λ���ţ���ú͵�Ƭ���ĸ�λ�������
    ** DC /RS      �������ѡ������
    ** CS /CE      SPI��CSƬѡ����
    ** BL /LED     ���Ʊ���LED�� �ߵ�ƽ���������Ѵ�1K���裬����10K������
    **  
    ** ����Ӳ��    ħŮ������_STM32F103RCT6 + 1.8�����ģ��(ST7735)
************************************************************************************************************************************/
#include <stm32f10x.h>    // ����ʹ���û�Ŀ¼���ļ��������޸��Ż�������ļ��Ǳ���ģ� ���ֵ�ַ�Ͳ����ĺ궨��
#include "stdio.h"        // C��׼��ͷ�ļ�: �������������getchar()��putchar()��scanf()��printf()��gets()��puts()��sprintf()
#include "stdlib.h"       // C��׼��ͷ�ļ�: ͨ�ù��ߺ�����malloc()��calloc()��realloc()��free()��system()��atoi()��atol()��rand()��srand()��exit()
#include "stdbool.h"      // C��׼��ͷ�ļ�: ���岼������: bool��true��false
#include "string.h"       // C��׼��ͷ�ļ�: �ַ����鳣�ã�strcpy()��strncpy()��strcmp()��strlen()��strnset()




/*****************************************************************************
 ** ��ֲʱ��ֻ���޸���������š�SPI�˿ڡ���Ļ����
*****************************************************************************/
// ģ������
// 3-SCL
#define LCD_SCK_GPIO      GPIOB             // ʱ�ӣ�SCL/SCK
#define LCD_SCK_PIN       GPIO_Pin_13
// 4-SDA
#define LCD_MOSI_GPIO     GPIOB             // �����SDA/DIN
#define LCD_MOSI_PIN      GPIO_Pin_15
// 5-RES
#define LCD_RES_GPIO      GPIOC             // ��λ, RST
#define LCD_RES_PIN       GPIO_Pin_4
// 6-DC
#define LCD_DC_GPIO       GPIOB             // ����, RS/DC  
#define LCD_DC_PIN        GPIO_Pin_11
// 7-CS
#define LCD_CS_GPIO       GPIOB             // Ƭѡ, CS/CE
#define LCD_CS_PIN        GPIO_Pin_0
// 8-BL
#define LCD_BL_GPIO       GPIOA             // ���⣬BL/LED
#define LCD_BL_PIN        GPIO_Pin_1
// SPI
#define LCD_SPI_PORT      SPI2
#define LCD_SPI_SCK       RCC_APB1Periph_SPI2
// ��Ļ����
#define LCD_WIDTH         130               // ������Ļ�����ش�С��
#define LCD_HIGH          130              // ע�⣺ʵ�����غ���Ҫ�ȱ�ʾֵ��1~2���أ�
#define LCD_DIR           1                 // ������ʾ���򣬲�����1��2��3��4
//  ���� ��ֲ�޸� ************************************************************




  
/*****************************************************************************
 ** ��ɫ���壬 ��ֲʱ�����޸�
****************************************************************************/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	  0xF800
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	0x001F
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    			


/*****************************************************************************
 ** ����  ȫ�ֺ����� ��ֲʱ�����޸�
****************************************************************************/
void LCD_Init(void);                                                        // LCD����ST7735��ʼ������Ļ�����ڱ��ļ������޸�(��ֲ������)����Ҫ�����سߴ硢��ʾ����
void LCD_Display(u8 sw);                                                    // ��ʾ�����أ� 1������Ļ�� 0��Ϩ��
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);		            // ��䵥ɫ
//void LCD_Line(u16 sX, u16 sY, u16 eX, u16 eY, u16 color);                   // ����
//void LCD_Circle(u16 x0,u16 y0,u8 r, u16 color);				                // ��Բ
//void LCD_Image(u16 x, u16 y, u16 width, u16 height, const u8 *image);       // ��ʾͼƬ
// �����ַ���������ʾ���ַ�������font.h�ļ��У�����������W25Q128�ļ��У��ʺ����⺺����������壺12��16��24��32��
void LCD_String(u16 x, u16 y, char* pFont, u8 size, u32 fColor, u32 bColor);  // ��ʾ��Ӣ�ַ��������Ͻ�����x, ���Ͻ�����y, �ֿ⻺���ַ���ֺŴ�С����ɫ����ɫ
// �ض�������ʾ:�ֿ�������font�ļ���.ֻ�ܱ��������ֿ⣬�ʺϹ̶����
//void LCD_Chinese16ForFile(u32 x, u32 y, u8 index, u16 fColor, u16 bColor);  // ��ʾ1��16�ź��֣����Ͻ�����x, ���Ͻ�����y, �ֿ������ڻ����е�λ�ã���ɫ����ɫ
//void LCD_Chinese32ForFile(u32 x, u32 y, u8 index, u16 fColor, u16 bColor);  // ��ʾ1��32�ź��֣����Ͻ�����x, ���Ͻ�����y, �ֿ������ڻ����е�λ�ã���ɫ����ɫ
//void LCD_GUI(void);
#endif  
	 
	 



