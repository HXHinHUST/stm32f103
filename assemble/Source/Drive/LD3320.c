#include "LD3320.h"
#include "dht11.h"

uint8 nAsrStatus = 0;	
uint8 nLD_Mode = LD_MODE_IDLE;//用来记录当前是在进行ASR识别还是在播放MP3
uint8 ucRegVal;


void LD3320_main(void)
{
	uint8 nAsrRes=0;
	u8 temp = 0,humi = 0,temp_ = 0,humi_ = 0;
	LD3320_init();
	char TEMP[20],HUMI[20];
	if(!DHT11_Init()){
		 printf("\r\n EEROR! THE DHT11 HAS NO RESPOND...");
	}
	printf("\r\n THE DHT11 HAS RESPOND\r\n");
	
 	printf("1. da kai tai deng\r\n"); 
	printf("2. guan bi tai deng\r\n"); 				
	printf("3. qi dong feng shan\r\n"); 		
	printf("4. guan diao feng shan\r\n"); 			
	printf("5. zhuang tai \r\n"); 		
	nAsrStatus = LD_ASR_NONE;//初始状态：没有在作ASR     nAsrStatus   上面有定义         #define LD_ASR_NONE					0x00	//表示没有在作ASR识别

	while(1)
	{
		if(DHT11_Read_Data(&temp,&humi,&temp_,&humi_))
		{
//			printf("\r\n temp:%d.%d,humi:%d.%d \r\n",temp,temp_,humi,humi_);
			sprintf(TEMP,"temp: %d.%d degrees",temp,temp_);
			sprintf(HUMI,"humi: %d.%d %%",humi,humi_);
			LCD_String(6, 10,TEMP,12, YELLOW , BLACK);	
			LCD_String(6, 30,HUMI,12, YELLOW , BLACK);
		}
		
		
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:               //#define LD_ASR_RUNING				0x01	//表示LD3320正在作ASR识别中										
			case LD_ASR_ERROR:  break;        //#define LD_ASR_ERROR	 			0x31	//表示一次识别流程中LD3320芯片内部出现不正确的状态			
			case LD_ASR_NONE:                 //#define LD_ASR_NONE					0x00	//表示没有在作ASR识别
					nAsrStatus=LD_ASR_RUNING;     //初始状态：没有在作ASR     nAsrStatus           #define LD_ASR_NONE					0x00	//表示没有在作ASR识别
					if (RunASR()==0)//启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
					{		
						nAsrStatus = LD_ASR_ERROR;
						printf("erro");
					}
					break;	
			case LD_ASR_FOUNDOK:
					 nAsrRes = LD_GetResult();//一次ASR识别流程结束，去取ASR识别结果										 
					 printf("\r\n识别码:%d", nAsrRes);		
					 switch(nAsrRes)		   			//对结果执行相关操作,客户修改
						{
							case CODE_LSD:					//command "turn on fan"
								printf(" qi dong feng  shan\r\n"); 
								LED1_OFF();
								break;
							case CODE_SS:	 					//command "turn off fan"
								printf("guan diao feng  shan\r\n"); 
								LED1_ON();
								break;
							case CODE_AJCF:					//command "turn on lamp"
								printf("guan bi tai deng\r\n"); 
								LED2_ON();
								break;
							case CODE_QM:						//command "turn off lamp"
								printf("da kai tai deng\r\n");
								LED2_OFF();
								break;
							case CODE_JT:						//command "status"
								printf("zhuang tai\r\n");
								break;
							default:break;
						}	
						nAsrStatus = LD_ASR_NONE;
						break;
			case LD_ASR_FOUNDZERO:
			default:
					nAsrStatus = LD_ASR_NONE;
					break;
			}//switch
	}
}

static uint8 LD_AsrAddFixed(void)
{
	uint8 k, flag;
	uint8 nAsrAddLength;
	#define DATE_A 5    //数组二维数值
	#define DATE_B 30		//数组一维数值
	//添加关键词，用户修改
	uint8  sRecog[DATE_A][DATE_B] = {
	 			"qi dong feng shan",\
				"guan diao feng shan",\
				"guan bi tai deng",\
				"da kai tai deng",\
				"zhuang tai"\
		
																	};	
	uint8  pCode[DATE_A] = {
	 															CODE_LSD,\
																CODE_SS, \
																CODE_AJCF,\
																CODE_QM,\
																CODE_JT\
															};	//添加识别码，用户修改
	flag = 1;
	for (k=0; k<DATE_A; k++)
	{			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}

		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0);
		LD_WriteReg(0x08, 0x04);
		LD3320_delay(1);
		LD_WriteReg(0x08, 0x00);
		LD3320_delay(1);

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
	}	 
	return flag;
}
/*

*/
///相关初始化
static void LD3320_init(void)          //初始化
{
	LD3320_GPIO_Cfg();	                 //初始化GPIO
	LD3320_EXTI_Cfg();                   //初始化中断
	LD3320_SPI_cfg();                    //初始化SPI通讯
	LED_GPIO_cfg();                      //初始化LED灯
	LD_reset();                          //I/O口高低在加延时，这里的延时是死延时，特别要注意要改
}

static void LD3320_GPIO_Cfg(void)                              //初始化化IO
{	
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(LD3320RST_GPIO_CLK | LD3320CS_GPIO_CLK,ENABLE);
		//LD_CS	/RSET
		GPIO_InitStructure.GPIO_Pin =LD3320CS_PIN;                 //GPIO_Pin_4
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(LD3320CS_GPIO_PORT,&GPIO_InitStructure);         //GPIOA
	
		GPIO_InitStructure.GPIO_Pin =LD3320RST_PIN;                //GPIO_Pin_15	
		GPIO_Init(LD3320RST_GPIO_PORT,&GPIO_InitStructure);        //GPIOB
}

static void LD3320_EXTI_Cfg(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	

	RCC_APB2PeriphClockCmd(LD3320IRQ_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =LD3320IRQ_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LD3320IRQ_GPIO_PORT, &GPIO_InitStructure);
	//外部中断线配置
  GPIO_EXTILineConfig(LD3320IRQEXIT_PORTSOURCE, LD3320IRQPINSOURCE);
  EXTI_InitStructure.EXTI_Line = LD3320IRQEXITLINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	//中断嵌套配置
  NVIC_InitStructure.NVIC_IRQChannel = LD3320IRQN;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

static void LD3320_SPI_cfg(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  //spi端口配置
	RCC_APB2PeriphClockCmd(LD3320SPI_CLK,ENABLE);		
  RCC_APB2PeriphClockCmd(LD3320WR_GPIO_CLK | LD3320SPIMISO_GPIO_CLK | LD3320SPIMOSI_GPIO_CLK | LD3320SPISCK_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LD3320SPIMISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(LD3320SPIMISO_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LD3320SPIMOSI_PIN;
	GPIO_Init(LD3320SPIMOSI_GPIO_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LD3320SPISCK_PIN;
	GPIO_Init(LD3320SPISCK_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LD3320WR_PIN;				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LD3320WR_GPIO_PORT, &GPIO_InitStructure);
	
	LD_CS_H();
	
	SPI_Cmd(LD3320SPI, DISABLE);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   	//全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   						//主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   					//8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   							//时钟极性 空闲状态时，SCK保持低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   						//时钟相位 数据采样从第一个时钟边沿开始
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   							//软件产生NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;   //波特率控制 SYSCLK/128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   					//数据高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;							   							//CRC多项式寄存器初始值为7
	SPI_Init(LD3320SPI, &SPI_InitStructure);

	SPI_Cmd(LD3320SPI, ENABLE);
}

static void LED_GPIO_cfg(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK | LED4_GPIO_CLK,ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = LED3_PIN;
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = LED4_PIN;
		GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);	
	
		LED1_OFF();
		LED2_OFF();
		LED3_OFF();
		LED4_OFF();
}
///相关初始化 end 

///中间层
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(LD3320IRQEXITLINE)!= RESET ) 
	{
		ProcessInt(); 
 		printf("进入中断\r\n");	
		EXTI_ClearFlag(LD3320IRQEXITLINE);
		EXTI_ClearITPendingBit(LD3320IRQEXITLINE);//清除LINE上的中断标志位  
	} 
}

static void LD3320_delay(unsigned long uldata)
{
	unsigned int i  =  0;
	unsigned int j  =  0;
	unsigned int k  =  0;
	for (i=0;i<5;i++)
	{
		for (j=0;j<uldata;j++)
		{
			k = 200;
			while(k--);
		}
	}
}

static uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	for (i=0; i<5; i++)		//防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_AsrStart();			//初始化ASR
		LD3320_delay(100);
		if (LD_AsrAddFixed()==0)	//添加关键词语到LD3320芯片中
		{
			LD_reset();				//LD3320芯片内部出现不正常，立即重启LD3320芯片
			LD3320_delay(50);	//并从初始化开始重新ASR识别流程
			continue;
		}
		LD3320_delay(10);
		if (LD_AsrRun() == 0)
		{
			LD_reset();			 //LD3320芯片内部出现不正常，立即重启LD3320芯片
			LD3320_delay(50);//并从初始化开始重新ASR识别流程
			continue;
		}
		asrflag=1;
		break;						//ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}	
	return asrflag;
}

static void LD_reset(void)
{
	LD_RST_H();               //GPIO_SetBits(GPIOB, GPIO_Pin_15)
	LD3320_delay(100);
	LD_RST_L();               //GPIO_ResetBits(GPIOB, GPIO_Pin_15)
	LD3320_delay(100);
	LD_RST_H();               //GPIO_SetBits(GPIOB, GPIO_Pin_15)
	LD3320_delay(100);
	LD_CS_L();                //GPIO_ResetBits(GPIOA, GPIO_Pin_4)
	LD3320_delay(100);
	LD_CS_H();		            //GPIO_SetBits(GPIOA, GPIO_Pin_4)
	LD3320_delay(100);
}

static void LD_AsrStart(void)
{
	LD_Init_ASR();
}

uint8 LD_Check_ASRBusyFlag_b2(void)
{
	uint8 j;
	uint8 flag = 0;
	for (j=0; j<10; j++)
	{
		if (LD_ReadReg(0xb2) == 0x21)
		{
			flag = 1;
			break;
		}
		LD3320_delay(10);		
	}
	return flag;
}
///中间层end


///寄存器操作
static uint8 spi_send_byte(uint8 byte)
{
	while (SPI_I2S_GetFlagStatus(LD3320SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(LD3320SPI,byte);
	while (SPI_I2S_GetFlagStatus(LD3320SPI,SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(LD3320SPI);
}

static void LD_WriteReg(uint8 data1,uint8 data2)
{
	LD_CS_L();
	LD_SPIS_L();
	spi_send_byte(0x04);
	spi_send_byte(data1);
	spi_send_byte(data2);
	LD_CS_H();
}

static uint8 LD_ReadReg(uint8 reg_add)
{
	uint8 i;
	LD_CS_L();
	LD_SPIS_L();
	spi_send_byte(0x05);
	spi_send_byte(reg_add);
	i=spi_send_byte(0x00);
	LD_CS_H();
	return(i);
}

static uint8 LD_GetResult(void)
{
	return LD_ReadReg(0xc5);
}

static uint8 LD_AsrRun(void)
{
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	LD3320_delay( 5 );
	LD_WriteReg(0x08, 0x00);
	LD3320_delay( 5);

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff);	
	LD_WriteReg(0x37, 0x06);
	LD_WriteReg(0x37, 0x06);
	LD3320_delay(5);
	LD_WriteReg(0x1C, 0x0b);
	LD_WriteReg(0x29, 0x10);
	LD_WriteReg(0xBD, 0x00);   
	return 1;
}

static void ProcessInt(void)
{
	uint8 nAsrResCount=0;

	ucRegVal = LD_ReadReg(0x2B);

// 语音识别产生的中断
//（有声音输入，不论识别成功或失败都有中断）
	LD_WriteReg(0x29,0) ;
	LD_WriteReg(0x02,0) ;

	if((ucRegVal & 0x10) && LD_ReadReg(0xb2)==0x21 && LD_ReadReg(0xbf)==0x35)		
	{	 
			nAsrResCount = LD_ReadReg(0xba);

			if(nAsrResCount>0 && nAsrResCount<=4) 
			{
				nAsrStatus=LD_ASR_FOUNDOK; 				
			}
			else
			{
				nAsrStatus=LD_ASR_FOUNDZERO;
			}	
	}
	else
	{
		nAsrStatus=LD_ASR_FOUNDZERO;//执行没有识别
	}

	LD_WriteReg(0x2b,0);
	LD_WriteReg(0x1C,0);//写0:ADC不可用
	LD_WriteReg(0x29,0);
	LD_WriteReg(0x02,0);
	LD_WriteReg(0x2B,0);
	LD_WriteReg(0xBA,0);	
	LD_WriteReg(0xBC,0);	
	LD_WriteReg(0x08,1);//清除FIFO_DATA
	LD_WriteReg(0x08,0);//清除FIFO_DATA后 再次写0
}

static void LD_Init_Common(void)
{
	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	LD3320_delay(5);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03);  
	LD3320_delay(5);
	LD_WriteReg(0xCF, 0x43);   
	LD3320_delay(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E, 0x00); 
		LD_WriteReg(0x19, LD_PLL_MP3_19);   
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);   
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19); 
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
	  LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}
	LD3320_delay(5);
	
	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c); 
	LD3320_delay(1);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}

static void LD_Init_ASR(void)
{
	nLD_Mode=LD_MODE_ASR_RUN;
	LD_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);	
	LD3320_delay(5);
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
	LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8); 
	LD3320_delay( 1 );
}
///寄存器操作 end
/*********************************************END OF FILE**********************/
