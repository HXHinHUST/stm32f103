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
#include "lcd_st7735.h"
#include "stm32f10x_spi.h"
#include "font.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
/*****************************************************************************
 ** Һ�����ƺ궨��, ��ҪΪ�˼��ļ��ڲ���
 *****************************************************************************/
// LCD_CS  
#define	LCD_CS_LOW  	LCD_CS_GPIO->BSRR=LCD_CS_PIN <<16
#define LCD_CS_HIGH     LCD_CS_GPIO->BSRR=LCD_CS_PIN  
// 
#define	LCD_BL_LOW  	LCD_BL_GPIO->BSRR=LCD_BL_PIN  <<16 
#define	LCD_BL_HIGH  	LCD_BL_GPIO->BSRR=LCD_BL_PIN 

#define	LCD_RS_LOW  	LCD_DC_GPIO->BSRR=LCD_DC_PIN <<16
#define	LCD_RS_HIGH  	LCD_DC_GPIO->BSRR=LCD_DC_PIN 
//LCD_RST
#define	LCD_RES_LOW  	LCD_RES_GPIO->BSRR=LCD_RES_PIN <<16
#define	LCD_RES_HIGH  	LCD_RES_GPIO->BSRR=LCD_RES_PIN 
 	


/*****************************************************************************
 ** ���ر���
 *****************************************************************************/
struct _LCD              // �ṹ�壺���ڱ�����Ϣ��״̬
{     
    u8  InitOK;
	u16 width;			 // LCD ���
	u16 height;			 // LCD �߶�
    
    u32 bColor;          // ����ɫ
}xLCD;

/*****************************************************************************
 ** ���غ���
 ****************************************************************************/
void gpioInit(void);                                         // ���ų�ʼ��
void spiInit(void);                                          // SPIͨ��Э���ʼ��
void sendOrder(u16 order);                                   // ��Һ�������ͣ��Ĵ�����ֵַ(ָ��), 8λ
void sendByte (u8  Data);                                    // ��Һ�������ͣ����ݣ�8λ
void sendShort(u16 Data);                                    // ��Һ�������ͣ����ݣ�16λ
void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd);  // ������ʾ����


// ���ų�ʼ��
void gpioInit(void)
{
//#if 1
//    // ʹ��ħŮ�������Ŷӱ�д�ĳ�ʼ����������ֱ������·���׼��
//    GPIOSet (LCD_SCK_GPIO , LCD_SCK_PIN , GPIO_Mode_AF ,  GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
//    GPIOSet (LCD_MOSI_GPIO, LCD_MOSI_PIN, GPIO_Mode_AF ,  GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
//    GPIOSet (LCD_RES_GPIO , LCD_RES_PIN , GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
//    GPIOSet (LCD_DC_GPIO ,  LCD_DC_PIN ,  GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
//		GPIOSet (LCD_CS_GPIO ,  LCD_CS_PIN ,  GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
//		GPIOSet (LCD_BL_GPIO ,  LCD_BL_PIN ,  GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );	
//#else
	// ʹ�ñ�׼��
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );// PORTʱ��ʹ�� 
    // SCK
	GPIO_InitStructure.GPIO_Pin   = LCD_SCK_PIN  ;	  
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;           // ����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_SCK_GPIO, &GPIO_InitStructure);
    // MOSI
	GPIO_InitStructure.GPIO_Pin   = LCD_MOSI_PIN ;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 
    GPIO_Init(LCD_MOSI_GPIO , &GPIO_InitStructure);
    // RES
	GPIO_InitStructure.GPIO_Pin   = LCD_RES_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_RES_GPIO , &GPIO_InitStructure);
    // DC
	GPIO_InitStructure.GPIO_Pin   = LCD_DC_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_DC_GPIO , &GPIO_InitStructure);
    // CS
	GPIO_InitStructure.GPIO_Pin   = LCD_CS_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_CS_GPIO , &GPIO_InitStructure);
	// BL
	GPIO_InitStructure.GPIO_Pin   = LCD_BL_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_BL_GPIO , &GPIO_InitStructure);
//#endif
	LCD_CS_HIGH ;   // ����������ֹ��ƽ����
	LCD_BL_HIGH ;   // ����Ļ��ʾ	
}

// SPIͨ��Э���ʼ��
// ��Ҫ����ΪSPI���߿��ܹ��ض��Ҫ��ͬͨ�Ų������豸�������ÿ��ͨ��ǰ������һ�����ú��� 
void spiInit(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	
 

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	
	//SPI2_ReadWriteByte(0xff);//��������		 
 

}   

// SPI���ʹ���1�ֽ�
u8 spiSendByte(u8 data)
{
    static u16 retry;
    
	retry=0;	
    while((LCD_SPI_PORT -> SR & 2) == 0) {  // �ȴ�������Ϊ��	
		retry++;
		if(retry>500) return 0;
	}
    LCD_SPI_PORT ->DR = data;
	
	retry =0;    
    while((LCD_SPI_PORT -> SR & 1) == 0){   // �ȴ�����������	
		retry++;
		if(retry>250) return 0;
	}
    return LCD_SPI_PORT -> DR ;       
}		   



/*****************************************************************************
*��  ����LCD_Init
*��  �ܣ���ʼ��lcd
*��  ����Ϊ�˼���⡢������ֲ������(��Ļ���ء���ʾ����)��h�ļ�������޸�
*����ֵ���� 
*��  ע��
*****************************************************************************/
void LCD_Init(void)  
{ 	 
    xLCD.InitOK = 0; 
    xLCD.bColor  = BLACK;
    
	// ��ʾ��������ƥ��
    if( (LCD_DIR ==1) || (LCD_DIR ==3))
    {
        xLCD.width  = LCD_WIDTH ;       // ��������أ�������ֵ����оƬ���Զ����У�ע�⣺�����Ļ�ұ��л������ͼӴ����ֵ
        xLCD.height = LCD_HIGH ;        // ���߶����أ� ע�⣺�����Ļ�����л������ͼӴ����ֵ
    }
    else
    {
        xLCD.width  = LCD_HIGH;
        xLCD.height = LCD_WIDTH;
    }      
	
	// ��ʼ��GPIO
	gpioInit();
	// ��ʼ��SPI���衢Э��
    spiInit();
	
	// ��Ļ��λ	
	LCD_RES_LOW ;	       // LCD_RST=0	  //SPI�ӿڸ�λ
	Delay_ms(20);          // delay 20 ms 
    LCD_RES_HIGH  ;        // LCD_RST=1		
	Delay_ms(20);

	sendOrder(0x11);       // �˳�˯��ģʽ
	Delay_ms(120);         // Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	sendOrder(0xB1);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
    
	sendOrder(0xB2);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
    
	sendOrder(0xB3);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	sendOrder(0xB4); // Dot inversion
	sendByte(0x03);
    
	sendOrder(0xC0); // ST7735R Power Sequence
	sendByte(0x28);
	sendByte(0x08);
	sendByte(0x04);
    
	sendOrder(0xC1);
	sendByte(0XC0);
    
	sendOrder(0xC2);
	sendByte(0x0D);
	sendByte(0x00);
	sendOrder(0xC3);
    
	sendByte(0x8D);
	sendByte(0x2A);
    
	sendOrder(0xC4);
	sendByte(0x8D);
	sendByte(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	sendOrder(0xC5); //VCOM
	sendByte(0x1A);
	sendOrder(0x36); //MX, MY, RGB mode
	if(LCD_DIR == 1)  sendByte(0xC0);  // ��Ҫ����ʾ������ƣ�C0/00/A0/60,  C8/08/A8/68
	if(LCD_DIR == 2)  sendByte(0x00);      
    if(LCD_DIR == 3)  sendByte(0xA0);
    if(LCD_DIR == 4)  sendByte(0x60);
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	sendOrder(0xE0);
	sendByte(0x04);
	sendByte(0x22);
	sendByte(0x07);
	sendByte(0x0A);
	sendByte(0x2E);
	sendByte(0x30);
	sendByte(0x25);
	sendByte(0x2A);
	sendByte(0x28);
	sendByte(0x26);
	sendByte(0x2E);
	sendByte(0x3A);
	sendByte(0x00);
	sendByte(0x01);
	sendByte(0x03);
	sendByte(0x13);
    
	sendOrder(0xE1);
	sendByte(0x04);
	sendByte(0x16);
	sendByte(0x06);
	sendByte(0x0D);
	sendByte(0x2D);
	sendByte(0x26);
	sendByte(0x23);
	sendByte(0x27);
	sendByte(0x27);
	sendByte(0x25);
	sendByte(0x2D);
	sendByte(0x3B);
	sendByte(0x00);
	sendByte(0x01);
	sendByte(0x04);
	sendByte(0x13);
    
    // ���ڸ������ӵģ�������
    sendOrder(0x2a);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x7f);
    // ���ڸ������ӵģ�������
	sendOrder(0x2b);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x9f);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	sendOrder(0x3A); // 65k mode
	sendByte(0x05);
	
    sendOrder(0x29); // Display on

	LCD_Fill(1, 1, xLCD.width , xLCD.height , BLACK );
    xLCD.InitOK = 1; 	
}  
// ��LCD���ͼĴ�����ַ(ָ��)
void sendOrder(u16 order)
{ 
	LCD_CS_LOW;            // SPI�豸Ƭѡ���ͣ���ʼͨ��
    LCD_RS_LOW;            // RS��: ����ֵ�� RS��: �Ĵ�����ֵַ
	spiSendByte(order);    // ���� �Ĵ�����ֵַ
	LCD_CS_HIGH;           // SPI�豸Ƭѡ���ߣ�����ͨ�� 
}
// ��LCD������ֵ
void sendByte(u8 data) 
{
 	LCD_CS_LOW;            // SPI�豸Ƭѡ���ͣ���ʼͨ��
	LCD_RS_HIGH;	       // RS��: ����ֵ�� RS��: �Ĵ�����ֵַ
	spiSendByte(data);     // ����1�ֽ�
	LCD_CS_HIGH;           // SPI�豸Ƭѡ���ߣ�����ͨ�� 			 
}	
// ��LCD����2���ֽ�
void sendShort(u16 data)
{
 	LCD_CS_LOW;            // SPI�豸Ƭѡ���ͣ���ʼͨ��
	LCD_RS_HIGH;	       // RS��: ���ݣ� RS��: ָ��
	spiSendByte(data>>8);  // ����1�ֽ�
	spiSendByte(data);     // ����1�ֽ�
	LCD_CS_HIGH;           // SPI�豸Ƭѡ���ߣ�����ͨ�� 	
}
				   
//д�Ĵ���: �Ĵ�����ַ������
void sendRegisterValue(u8 reg, u16 val)
{
	sendOrder(reg);
	sendShort(val);
}	 
	
/*****************************************************************
 * ��  ����LCD_Display
 * ��  �ܣ���ʾ������
 * ��  ����0:Ϩ����0����
 * ����ֵ����
 *
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
******************************************************************/
void LCD_Display(u8 sw)
{
	if(sw==0)
	    LCD_BL_LOW ;
	else
		LCD_BL_HIGH ;	
}
	
/*****************************************************************
 * ��  ����setCursor
 * ��  �ܣ�������ʾ�����ڴ�����д�������Զ�����
 * ��  ����xStart���������, yStart���������
 *         xEnd�����������㣬yEnd������������
 * ����ֵ����
 *
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
******************************************************************/
void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
{		
	sendOrder(0x2A);
	sendByte (0x00);   // ��ʼλ��x��λ����Ϊ1.8������128*160, ������255, ֱ��д0ʡ��
	sendByte (xStart); // ��ʼλ��x��λ��ֵ����ʱ�Զ������˸�8λ��Ҳʡ��������
	sendByte (0x00);   // ��ʼλ��y��λ
	sendByte (xEnd);   // ��ʼλ��xλλ

	sendOrder(0x2B);
	sendByte (0x00);
	sendByte (yStart);
	sendByte (0x00);
	sendByte (yEnd);
	
	sendOrder(0x2c);  // ����д��������
}

/*****************************************************************
 * ��  ����drawPoint
 * ��  �ܣ���һ����
 * ��  ����x���꣬y����, 16λ��ɫֵ
 * ����ֵ����
 * 
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
******************************************************************/
void drawPoint(u16 x, u16 y, u16 color)
{
    setCursor(x, y, x, y);      //���ù��λ�� 
    sendShort(color);    
}

/******************************************************************
 * �������� LCD_Fill
 * ��  �ܣ� ��ָ����������䵥����ɫ
 *          ��Ļ�����1��ʼ����Ļ���Һ��·�ʵ�����ж�������
 * ��  ���� u16 xStart, yStart  ���Ͻ���ʼ����
 *          u16 xEnd,   yEnd    ���½���ʼ����
 *          u32 Color           ������ɫ
 *
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
 *****************************************************************/
void LCD_Fill(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd, u16 color)
{     
	spiInit();                                      // ��ֹSPI�����������豸�޸���
	u32 pixel = (xEnd-xStart+1) * (yEnd-yStart+1);	// ������������
								   
	setCursor(xStart, yStart, xEnd, yEnd);      	// �趨��䷶Χ
    while(pixel-->0)                                // ������ɫֵ
       sendShort(color);                            
}  







/******************************************************************
 * �������� drawAscii
 * ��  �ܣ� ��ָ��λ����ʾһ���ַ�	
 * ��  ���� u16 x,y     ��ʼ����
 *          u8  num     Ҫ��ʾ���ַ�:" "--->"~"
 *          u8  size    �����С 12/16/24/32
 *          u32 fColor  ������ɫ
 *          u32 bColor  ������ɫ
 * ��  ע�� �ο�ԭ�Ӹ��Ұ�����Ĵ�����޸�  ���ϴ��QȺ��262901124 
 *****************************************************************/
void drawAscii(u16 x,u16 y,u8 num,u8 size,u32 fColor, u32 bColor)
{  			
	spiInit();                                        // ��ֹSPI�����������豸�޸���
	
    if(xLCD.InitOK ==0) return;
    
    u8 temp;
	u8 t,t1;
	u16 y0=y;
    
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		   // �õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';                                       // �õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)         temp=asc2_1206[num][t];   // ����1206����
		else if(size==16)    temp=asc2_1608[num][t];   // ����1608����
		else if(size==24)    temp=asc2_2412[num][t];   // ����2412����
		else if(size==32)    temp=asc2_3216[num][t];   // ����3216����
		else return;								   // û�е��ֿ�		
		for(t1=0; t1<8; t1++)
		{			    
            if(temp&0x80)   drawPoint (x, y, fColor);  // ���� ���� 
            else            drawPoint (x, y, bColor);  // ���� ����
            temp<<=1;
			y++;
			if(y>=xLCD.height)    return;		       // ������Ļ�߶�(��)
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=xLCD.width) return;              // ������Ļ���(��)
				break;
			}
		}  	 
	}  	    	 	  
} 







/******************************************************************************
 * ��  ���� LCD_String
 * ��  �ܣ� ����Ļ����ʾ�ַ�����֧��Ӣ�ġ�����
            ��Ҫ��ʾ��ע���ֿ�ĸ�ʽ��λ�ã� �������ֿ���W25Q128��
 *         
 * ��  ���� 
 * ����ֵ����
 * ��  ע�� ħŮ�������Ŷ�  ���ϴ��QȺ��262901124        ����޸�_2020��05��1����
 ******************************************************************************/  
void LCD_String(u16 x, u16 y, char* pFont, u8 size, u32 fColor, u32 bColor)
{
    if(xLCD .InitOK ==0 ) return;
    
    u16 xStart = x;
    
    // �����С����
    if( size!=12 && size!=16 && size!=24 && size!=32 )
        size=24;    
    
    while(*pFont != 0)
    {
        // λ�ÿ���        
        if(x>(xLCD.width-size))       // �����һ�в���λ�ã�����һ��
        {
            x=xStart;
            y=y+size;
        }
        if(y>(xLCD.height - size))    // ���������Ļ�ײ����ͷ��أ��������
            return;        
        
		// �ж�������ASCII���Ǻ���
        if(*pFont < 127)              // ASCII�ַ�
        {
            drawAscii (x, y, *pFont, size, fColor, bColor);            
            pFont++;
            x+=size/2;            
        }
    }      
}





