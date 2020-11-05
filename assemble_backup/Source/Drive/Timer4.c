//##############################################ͷ�ļ�
#include "Timer4.h"

//##############################################ͷ�ļ�


//##############################################ȫ�ֱ�������

uint32_t Timer4_Count = 0;  //��¼Timer3�жϴ���
uint16_t Timer4_Frequency;//Timer3�ж�Ƶ��


//##############################################ȫ�ֱ�������
//******************************************************TIMER4��ʼ������
void Timer4_Init(uint16_t Handler_Frequency)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	Timer4_Frequency = Handler_Frequency;
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = 1000*1000/Handler_Frequency ;//װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	//��Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���ָ�ʱ��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);//����жϱ�־
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);//ʹ�ܶ�ʱ��3
	
}
//******************************************************TIMER4��ʼ������


//******************************************************TIMER4�ж����ȼ��趨
void Nvic_Init(void)
{
  	NVIC_InitTypeDef NVIC_InitStructure;
	
	
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ�����

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

//******************************************************TIMER4�ж����ȼ��趨





//******************************************************�жϷ�����TIMER4

//##############################################ȫ�ֱ�������
uint8_t Count_1ms,Count_2ms,Count_5ms,Count_250ms;

//##############################################ȫ�ֱ�������
void TIM4_IRQHandler(void)//Timer4�ж�
{	
	if(TIM4->SR & TIM_IT_Update)
	{     
		TIM4->SR = ~TIM_FLAG_Update;//����жϱ�־
		
		//if( Bsp_Int_Ok == 0 )	return;//Ӳ��δ��ʼ����ɣ��򷵻�
		Timer4_Count++;
		Count_1ms++;
		Count_2ms++;
		Count_5ms++;
		Count_250ms++;
	}
}
//******************************************************�жϷ�����TIMER4
