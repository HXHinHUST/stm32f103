
//##############################################ͷ�ļ�
#include "Timer4.h"

//##############################################ͷ�ļ�

//##############################################ȫ�ֱ�������




//##############################################ȫ�ֱ�������


/******************************************************************************
����ԭ�ͣ�	void LED_Init(void)
��    �ܣ�	LED��ʼ��
*******************************************************************************/ 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	//�ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_Disable SWJ ��ȫ���ã�JTAG+SW-DP��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	//�ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_JTAGDisable ��JTAG-DP ���� + SW-DP ʹ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_Structure.GPIO_Pin =  LED2_Pin;	//LED2����Դ�Ե�ָʾ��
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED2_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LED3_Pin;	//LED3��������Χ��ҹ���	
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED3_GPIO, &GPIO_Structure);
	
	
}

/******************************************************************************
����ԭ�ͣ�	void LED_ON_OFF(void)
��    �ܣ�	LED������˸
*******************************************************************************/ 

uint8_t  ii;

void LED_ON_OFF(void)
{
//	uint8_t i ;
//	for(i=0;i<3;i++)
//	{
//		LED2_ON;LED3_OFF;
//		//Delay_led(200);
//		LED3_ON;LED2_OFF;
//		//Delay_led(200);	
//	}
//	for(i=0;i<3;i++)
//	{
//		LED2_ON;LED3_OFF;
//	//	Delay_led(100);
//		LED3_ON;LED2_OFF;
//		//Delay_led(100);	
//	}	
//	LED2_OFF;
//	LED3_OFF;
////	Delay_led(100);

//LED3_OFF;	

//    if   (ii)
//		{
//			ii++;
//			switch(ii)
//			{
//				case 0:	

//				break;
//				case 1:	
//					LED2_ON;
//				break;
//				case 2:	
//					LED3_ON;
//				break;
//				case 3:	
//   ii=0;
//				break;
//			}
//			
//				__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop(); 
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//		}

}

