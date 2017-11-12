#include "main.h"
#include "System_Init.h"
#define ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)
extern u8 USART_Tx[BUF_SIZE];
extern u8 USART_Tx2[BUF_SIZE];
extern u16 ADCValue;
void Delay(int i)
{
    while(i--)
    {
        
    }
}
void Sys_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                           RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF |
                           RCC_AHB1Periph_GPIOG , ENABLE);

	/*
		1��NOTE:PA2-->USART2_TX; PA3-->USART2_RX   ����Ϊ
		   NOTE:PA9-->USART1_TX; PA10-->USART1_RX
		
		2��NOTE:PB10-->USART3_TX; PB11-->USART3_RX	 ����Ϊ
		   NOTE:PD5-->USART2_TX; PD6-->USART2_RX
	*/

    // Configure PA ************************************************//
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADC:PA0/PA4/PA5
	//5V��Ƿѹ�������Ԫ��ԭ����PA0����ΪPF3,����ADC�е�ͨ��0��Ϊͨ��9 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN ;      //����Ϊģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9| GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);     //���Ÿ�����ӳ��   PA6-->SPI1-->MISO
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);     //���Ÿ�����ӳ��   PA7-->SPI1-->MOSI
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_OTG_FS);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_OTG_FS);

		//����ͬʱ��PA2/PA3�� PD5/PD6����ΪUSART2�� ����USART2�Ľ����ж��޷������ж� 
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  
		//GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);    
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); 


    //PA2-->USART2_TX; PA3-->USART2_RX ����Ϊ  PA9-->USART1_TX; PA10-->USART1_RX
    //USART1��Ҫ�������ʹ���������ͨѶ
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); 

	
    // Configure PB ************************************************//  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;//| GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);     //���Ÿ�����ӳ��   PB3-->SPI1-->SCK
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);     //���Ÿ�����ӳ��   PB6-->I2C1_SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);     //���Ÿ�����ӳ��   PB7-->I2C1_SDA
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);   
    // Configure PC ************************************************//  
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_SDIO);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SDIO);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO);  

	
    // Configure PD ************************************************//  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_SDIO);

		//PB10-->USART3_TX; PB11-->USART3_RX����ΪPD5-->USART2_TX; PD6-->USART2_RX
		//USART2��Ҫ��������PC����ͨѶ
		//����ͬʱ��PA2/PA3�� PD5/PD6����ΪUSART2�� ���ͬʱ���ã�������USART2�Ľ����ж��޷�����
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  
		//GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);    
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); 
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);    
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); 
 
    // Configure PE ************************************************//  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    // Configure PF ************************************************//  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

	//5V��Ƿѹ�������Ԫ��ԭ����PA0����ΪPF3,����ADC�е�ͨ��0��Ϊͨ��9 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN ;      //����Ϊģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOF, &GPIO_InitStructure); 

	
    // Configure PG ************************************************//  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOG, &GPIO_InitStructure); 
}


void Sys_TIMx_Init(void)
{
   /* TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Prescaler = (120000000/TIM_CLK_FREQ)-1;//��ʱ��4��Ƶ	
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 0xffff;//BOOT_FREQ - 1;//
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);
    TIM_ARRPreloadConfig(TIM8, ENABLE);//������Ԥװ��ʹ��
    
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low;//
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//OC = 1 WHEN MOE = 0//TIM_OCIdleState_Reset;//
    TIM_OCInitStructure.TIM_Pulse = 20;//ռ�ձ�
    
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);
    TIM_OC2Init(TIM8, &TIM_OCInitStructure);
    TIM_OC3Init(TIM8, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);//�Ƚ�ֵԤװ��ʹ��
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);//�Ƚ�ֵԤװ��ʹ��
    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);//�Ƚ�ֵԤװ��ʹ��
    
    TIM_CCxCmd(TIM8, TIM_Channel_2, TIM_CCx_Disable);
    TIM_CCxCmd(TIM8, TIM_Channel_1, TIM_CCx_Disable);
    TIM_CCxCmd(TIM8, TIM_Channel_3, TIM_CCx_Disable);
    
    TIM_CtrlPWMOutputs(TIM8, ENABLE);
    
    DMA_InitTypeDef DMA_InitStructure;
    
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_Channel = DMA_Channel_7;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(TIM8->ARR));
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)addr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold	= DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream2, &DMA_InitStructure);
    
    TIM_DMACmd(TIM8, TIM_DMA_CC1, ENABLE);//UPDATE ��Ϊ��	Դ
    DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);	
    */
}
void TIM3_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    TIM_OCInitTypeDef TIM_OCInitStructure;

	// TIM3�ǹ�����APB1֮�£���������ͨ��APB1�����µ�ʹ�ܺ�����ʹ��TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    /* Time base configuration */
	//TIM_TimeBaseStructInit():Fills each TIM_TimeBaseInitStruct member with its default value.
	//��ʹ�����������ǰ��TIM3����λĬ��ֵ����������Ϊ��ʵ�ֳ���Ľ�׳��
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	//50KHz/1000 = 50Hz
    TIM_TimeBaseStructure.TIM_Period = 1000-1;//100hz  //�ܴ��ڴ����ٶ�Ӱ�첨������115200ʱ��߷���Ƶ����50Hzÿ�룬�ʲ���Ƶ�����50Hz��
    TIM_TimeBaseStructure.TIM_Prescaler = 600 -1;//APB1��PLCK1��ʱ��Ƶ��Ϊ30MHz��30MHz/600 = 50KHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //��APB1����ʱ��PLCK1����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���������0��ʼ������ֵ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	// TIM_ARRPreloadConfig():Enables or disables TIMx peripheral Preload register on ARR
    TIM_ARRPreloadConfig(TIM3, ENABLE);//������Ԥװ��ʹ��

	//Clears the TIMx's interrupt pending bits.
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	//Enables or disables the specified TIM interrupts.
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    
}
void Sys_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  

//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
//    NVIC_Init(&NVIC_InitStructure); 
    
    /* Enable the USARTx Interrupt */
	
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//		
//		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}
//******************************************USART*****************************//

void USART1_Init(void)       //USART1��Ҫ�Ǻʹ��������н���ͨ��
{
    DMA_InitTypeDef DMA_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);      //DMAʱ��ʹ��
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
	//�������ڵ�DMA���͹���
    USART_DMACmd(USART1, USART_DMAReq_Tx , ENABLE);     //USART_DMAReq_Tx:USART DMA transmit request
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);      //USART1ʹ�ܵ��ǽ��պͷ����ж�
        
    DMA_StructInit(&DMA_InitStructure);   //Fills each DMA_InitStruct member with its default value
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;   //ͨ��ѡ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART1->DR));  //�������ַ����������DMA������������ַ
    //�˴�����Ҫ���д��ڵ�DMA���䣬���������ַΪ���ڽ��շ��͵����ݴ洢��USART2->DR�ĵ�ַ����ʾΪ&(USART2->DR)
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)USART_Tx2;  //�ڴ����ַ��Ҳ�������Ǵ��DMA���ݵ��ڴ��ַ        
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //���ݴ��䷽�򣬴˴���������λ���ڴ��ȡ������Ȼ���͵����ڣ���������Ȼ����Ŀ�ĵ���
    DMA_InitStructure.DMA_BufferSize = 15;  //����һ�δ����������Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// ���ô������ݵ�ʱ�������ַ�ǲ��仹�ǵ������˴�����һֱ���̶������ַ&(USART2->DR)�������ݣ��ʵ�ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //��������ʱ���ڴ��ַ�Ƿ�����������ǽ��ڴ��������洢��Ԫ�����ݷ��͵����ڣ������ǵ��ڴ��ַ����Ҫ������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//��������ݳ������ֽڴ���(8bits),���ִ���(16bits)�����ִ���(32bits)
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //�����ڴ�����ݳ��ȣ����Ǵ˴��϶�����������ݳ���һ����������8λ�ֽڴ��䣬��ֵ����ΪDMA_PeripheralDataSize_Byte
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;   //DMAģʽ�Ƿ�ѭ���ɼ����������ô˲����ɼ����֮��ѭ����������ΪDMA_Mode_Normal
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  // ����DMAͨ�������ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  //�Ƿ���FIFOģʽ���˴�����ѡ�񲻿���
    DMA_InitStructure.DMA_FIFOThreshold	= DMA_FIFOThreshold_Full;  //FIFO��ֵ����
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;   //���ô洢��ͻ����������
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //
    DMA_Init(DMA2_Stream7, &DMA_InitStructure); // ��ʼ��DMA Stream
    
    
    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);   //ʹ��DMA2_Stream7 interrupts.
    DMA_Cmd(DMA2_Stream7, ENABLE);    //ʹ��DMA1_Stream6���������� 
 //   DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
    USART_Cmd(USART1, ENABLE);   
}

void USART2_Init(void)   //USART2��Ҫ�Ǻ�PC���н���ͨ��
{
		DMA_InitTypeDef DMA_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);         //USART2ʹ�ܵ��ǽ����ж�
        
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART2->DR));
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)USART_Tx;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold	= DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);

    //DMA_IT_TC:  Transfer complete interrupt mask
    DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);   //ʹ��DMA1 Stream3 interrupts.
    DMA_Cmd(DMA1_Stream6, ENABLE);
        
    USART_Cmd(USART2, ENABLE);


	
//    DMA_InitTypeDef DMA_InitStructure;
//    USART_InitTypeDef USART_InitStructure;
//
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);      //DMAʱ��ʹ��
//    
//    USART_InitStructure.USART_BaudRate = 115200;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    USART_Init(USART2, &USART_InitStructure);
//	//�������ڵ�DMA���͹���
//    USART_DMACmd(USART2, USART_DMAReq_Tx , ENABLE);     //USART_DMAReq_Tx:USART DMA transmit request
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);      //USART2ʹ�ܵ��ǽ��պͷ����ж�
//        
//    DMA_StructInit(&DMA_InitStructure);   //Fills each DMA_InitStruct member with its default value
//    DMA_InitStructure.DMA_Channel = DMA_Channel_4;   //ͨ��ѡ��
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART2->DR));  //�������ַ����������DMA������������ַ
//    //�˴�����Ҫ���д��ڵ�DMA���䣬���������ַΪ���ڽ��շ��͵����ݴ洢��USART2->DR�ĵ�ַ����ʾΪ&(USART2->DR)
//    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)USART_Tx2;  //�ڴ����ַ��Ҳ�������Ǵ��DMA���ݵ��ڴ��ַ        
//    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //���ݴ��䷽�򣬴˴���������λ���ڴ��ȡ������Ȼ���͵����ڣ���������Ȼ����Ŀ�ĵ���
//    DMA_InitStructure.DMA_BufferSize = 15;  //����һ�δ����������Ĵ�С
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// ���ô������ݵ�ʱ�������ַ�ǲ��仹�ǵ������˴�����һֱ���̶������ַ&(USART2->DR)�������ݣ��ʵ�ַ������
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //��������ʱ���ڴ��ַ�Ƿ�����������ǽ��ڴ��������洢��Ԫ�����ݷ��͵����ڣ������ǵ��ڴ��ַ����Ҫ������
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//��������ݳ������ֽڴ���(8bits),���ִ���(16bits)�����ִ���(32bits)
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //�����ڴ�����ݳ��ȣ����Ǵ˴��϶�����������ݳ���һ����������8λ�ֽڴ��䣬��ֵ����ΪDMA_PeripheralDataSize_Byte
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;   //DMAģʽ�Ƿ�ѭ���ɼ����������ô˲����ɼ����֮��ѭ����������ΪDMA_Mode_Normal
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  // ����DMAͨ�������ȼ�
//    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  //�Ƿ���FIFOģʽ���˴�����ѡ�񲻿���
//    DMA_InitStructure.DMA_FIFOThreshold	= DMA_FIFOThreshold_Full;  //FIFO��ֵ����
//    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;   //���ô洢��ͻ����������
//    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //
//    DMA_Init(DMA1_Stream6, &DMA_InitStructure); // ��ʼ��DMA Stream
//    
//    
// //   DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
//    DMA_Cmd(DMA1_Stream6, ENABLE);    //ʹ��DMA1_Stream6���������� 
// //   DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
//    USART_Cmd(USART2, ENABLE);
  
}


void USART1_Tx(u16 Data_count)
{    
    DMA_Cmd(DMA2_Stream7, DISABLE);       //����DMA2_Stream7���书��

	//ǰ���Ѿ�����DMA���ܣ��ʺ���Ϊ��ȷ��ȷʵ�ǽ����ˣ����ܽ�����һ���Ķ�������֤����Ľ�׳��
    if (DMA_GetCmdStatus(DMA2_Stream7) == DISABLE)   //Returns the status of EN bit for the specified DMAy Streamx.
    {
		//DMA_GetFlagStatus()�ú����Ĺ�����:��ѯDMA����ͨ����״̬
        if (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == SET)  //DMA_FLAG_TCIFx:  Streamx transfer complete flag
        {
            DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
        }
		//DMA_SetCurrDataCounter�ú�������:Enables or disables the specified DMAy Streamx.
		//DMA_SetCurrDataCounter(�����趨ÿ��DMA���������)
		//���ö�Ӧ��DMA�������������������С
        DMA_SetCurrDataCounter(DMA2_Stream7, Data_count);     //Writes the number of data units to be transferred on the DMAy Streamx.
        DMA_Cmd(DMA2_Stream7, ENABLE);   //ʹ��DMA1_Stream6���书��
    } 
}

void USART2_Tx(u16 Data_count)
{
    if (DMA_GetCmdStatus(DMA1_Stream6) == DISABLE)
    {
        if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) == SET)
        {
            DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
        }
        DMA_SetCurrDataCounter(DMA1_Stream6, Data_count);
        DMA_Cmd(DMA1_Stream6, ENABLE);
    } 
}

void USART3_Init(void)   //USART3��Ҫ�Ǻ�PC���н���ͨ��
{
    DMA_InitTypeDef DMA_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_DMACmd(USART3, USART_DMAReq_Tx , ENABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //USART3ʹ�ܵ��ǽ����ж�
        
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART3->DR));
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)USART_Tx;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold	= DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream3, &DMA_InitStructure);

    //DMA_IT_TC:  Transfer complete interrupt mask
    DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);   //ʹ��DMA1 Stream3 interrupts.
    DMA_Cmd(DMA1_Stream3, ENABLE);
        
    USART_Cmd(USART3, ENABLE);
  
}

//void USART3_Tx(u16 Data_count)
//{
//    if (DMA_GetCmdStatus(DMA1_Stream3) == DISABLE)
//    {
//        if (DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) == SET)
//        {
//            DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
//        }
//        DMA_SetCurrDataCounter(DMA1_Stream3, Data_count);
//        DMA_Cmd(DMA1_Stream3, ENABLE);
//    } 
//}

void Sys_SPI1_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	/* ʹ��gpioʱ�Ӻ�SPI2 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//SPI_SSOutputCmd(SPI2,ENABLE); 
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫���������ȫ˫��ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //����ΪSPI������ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; //SPI���ݴ�С������16λ֡���ݽṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;   //�豸����״̬ʱͬ��ʱ��SCK��״̬��High��ʾ�ߵ�ƽ��Low��ʾ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  //ʱ����λ��1��ʾ��ͬ��ʱ��SCK�������ر߲�����2��ʾż���ر߲���
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;     //NSS������ؼ�Ƭѡ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//ʱ�ӵ�Ԥ��Ƶֵ,15Mb
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //MSB��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;     //CRC����͵Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);   //��ʼ��SPI2��������
	SPI_Cmd(SPI1, ENABLE);      //ʹ��SPI2
}
void Sys_ADC_Init(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;


  //PA3��PF3��ͬ��
//  DMA_InitTypeDef       DMA_InitStructure;

  //PF3-->ADC3-->DMA2_Stream0     PA3-->ADC2-->DMA2_Stream3
  /* Enable peripheral clocks *************************************************/
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //����DMA2��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3 | RCC_APB2Periph_ADC2, ENABLE); //����ADC2��ADC3��ʱ��

//  /* DMA2_Stream3 channel0 configuration **************************************/
//  DMA_DeInit(DMA2_Stream0);
//  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCValue;
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//  DMA_InitStructure.DMA_BufferSize = 1;
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
//  /* DMA2_Stream0 enable */
//  DMA_Cmd(DMA2_Stream0, ENABLE);

  //ͨ�ÿ��Ʋ������ã����������ADC����ͨ������
  //ADC_CommonInitTypeDefͨ�ó�ʼ���ṹ�塣ADC_CommonInitTypeDef�ṹ�����ݾ�������ADC���õĹ�������������ģʽѡ��ADCʱ�ӵȵȡ�
  //ת��ʱ�� = ����ʱ�� + �̶����ڵ�ʱ��
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ����һ��ADC����ADת����ת��������
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; //Ԥ��Ƶ,ADCʱ������PCLK2��Ƶ��������Ƶϵ������ADCʱ��Ƶ�ʣ���ѡ�ķ�Ƶϵ��Ϊ2��4��6��8��ADC���ʱ������Ϊ36MHz
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;  //DMA��ֹ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //���������׶ε��ӳ�������
  ADC_CommonInit(&ADC_CommonInitStructure);


//  //���������ADC1��������
//  /* ADC1 Init ****************************************************************/
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //����ADCת���ֱ���
//  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //�����ڴ˴�ʹ�õ���ת��������ɨ��ģʽ
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;// ENABLE;//����ת�����˴�Ӧ����DISABLE
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //����ʹ���������,���Խ����ⲿͨ���Ĵ���
//  ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC1; //��Ϊ����ѡ�����������������Դ˴����ǲ��ý�������
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //�������ݶ��뷽ʽ
//  //��������ADC����Ϊ12λ�������������Ϊ����룬��ADת��������ݴ����ADC_DR�Ĵ�����[4:15]λ�ڣ����Ϊ�Ҷ��룬������ADC_DR�Ĵ�����[0:11]λ�ڡ�
//  
//  ADC_InitStructure.ADC_NbrOfConversion = 1; //�������ù������еĳ��ȣ����������ǵ���ת�����������ǵ�ֵ����Ϊ1 ����
//  ADC_Init(ADC1, &ADC_InitStructure);
//
//  /* Enable ADC1 DMA */
//  ADC_DMACmd(ADC1, ENABLE);


  //���������ADC2��������
  /* ADC2 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //����ADCת���ֱ���
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //�����ڴ˴�ʹ�õ���ת��������ɨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;// ENABLE;//����ת�����˴�Ӧ����DISABLE
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //����ʹ���������,���Խ����ⲿͨ���Ĵ���
  ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC1; //��Ϊ����ѡ�����������������Դ˴����ǲ��ý�������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //�������ݶ��뷽ʽ
  //��������ADC����Ϊ12λ�������������Ϊ����룬��ADת��������ݴ����ADC_DR�Ĵ�����[4:15]λ�ڣ����Ϊ�Ҷ��룬������ADC_DR�Ĵ�����[0:11]λ�ڡ� 
  ADC_InitStructure.ADC_NbrOfConversion = 1; //�������ù������еĳ��ȣ����������ǵ���ת�����������ǵ�ֵ����Ϊ1 ����
  ADC_Init(ADC2, &ADC_InitStructure);

//  /* Enable ADC1 DMA */
//  ADC_DMACmd(ADC2, ENABLE);



  //���������ADC3��������
  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //����ADCת���ֱ���
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //�����ڴ˴�ʹ�õ���ת��������ɨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;// ENABLE;//����ת�����˴�Ӧ����DISABLE
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //����ʹ���������,���Խ����ⲿͨ���Ĵ���
  ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC1; //��Ϊ����ѡ�����������������Դ˴����ǲ��ý�������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //�������ݶ��뷽ʽ
  //��������ADC����Ϊ12λ�������������Ϊ����룬��ADת��������ݴ����ADC_DR�Ĵ�����[4:15]λ�ڣ����Ϊ�Ҷ��룬������ADC_DR�Ĵ�����[0:11]λ�ڡ�
  
  ADC_InitStructure.ADC_NbrOfConversion = 1; //�������ù������еĳ��ȣ����������ǵ���ת�����������ǵ�ֵ����Ϊ1 ����
  ADC_Init(ADC3, &ADC_InitStructure);

//  /* Enable ADC1 DMA */
//  ADC_DMACmd(ADC3, ENABLE);
  
  /* ADC1 regular channel18 (VBAT) configuration ******************************/
  //������ADCͨ��ת��˳���ʱ��
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_9, 1, ADC_SampleTime_480Cycles);
  
//  /* Enable DMA request after last transfer (Single-ADC mode) */
//  ADC_DMARequestAfterLastTransferCmd(ADC1 | ADC2 | ADC3, ENABLE);

  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADC2, ENABLE);  //����ADת����
  ADC_Cmd(ADC3, ENABLE);  //����ADת����

}



void Sys_DAC_Init(void)
{

    DAC_InitTypeDef DAC_InitStructure;
    /* DAC channel2 Configuration */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software; 
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);
    
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);
    DAC_Cmd(DAC_Channel_2, ENABLE); 
    
    DAC_Init(DAC_Channel_1, &DAC_InitStructure); 
    DAC_Cmd(DAC_Channel_1, ENABLE); 
}


void SET_DAC_VALUE(float DAC1_VALUE,float DAC2_VALUE)
{
    u16 val1,val2;
    val1 = (u16)((DAC1_VALUE * 4096)/ 3); 
    val2 = (u16)((DAC2_VALUE * 4096)/ 3); 
    
    DAC_SetChannel1Data(DAC_Align_12b_R, val1);    
    DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);	
    
    DAC_SetChannel2Data(DAC_Align_12b_R, val2); 
    DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);	  
}


void* INT_To_ASCII(u16 data,char* lptr)
{
     u16 Int_Data4,Int_Data3,Int_Data2,Int_Data1;
     u16 Res_Data4,Res_Data3,Res_Data2,Res_Data1;  
  
     Int_Data4 = data/10000;
     Res_Data4 = data%10000;
     
     Int_Data3 = Res_Data4/1000;
     Res_Data3 = Res_Data4%1000;
     
     Int_Data2 = Res_Data3/100;
     Res_Data2 = Res_Data3%100;
     
     Int_Data1 = Res_Data2/10;
     Res_Data1 = Res_Data2%10;
     
     if(Int_Data4>0)
     {
        *lptr++ = Int_Data4+48;
        *lptr++ = Int_Data3+48;
        *lptr++ = Int_Data2+48;
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }
     else if(Int_Data3>0)
     {
        *lptr++ = Int_Data3+48;
        *lptr++ = Int_Data2+48;
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }
       else if(Int_Data2>0)
     {
        *lptr++ = Int_Data2+48;
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }
       else if(Int_Data1>0)
     {
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }   
     else if(Res_Data1>0)
     {
        *lptr++ = Res_Data1+48;  
     }
     else
     {
        *lptr++ = '0';
     }
     *lptr =  '\0';
     return lptr;
}
void* TwoPoint_To_ASCII(float data,char* lptr)
{
     u16 Int_Data4,Int_Data3,Int_Data2,Int_Data1;
     u16 Res_Data4,Res_Data3,Res_Data2,Res_Data1;  
     u16 Temp ;
	 
     Temp = (u16)(data*100);    //������λ��Ч���֣�220.5689*100 = 22056
     Int_Data4 = Temp/10000;    //Int_Data4 = 2
     Res_Data4 = Temp%10000;    //Res_Data4 = 2056
     
     Int_Data3 = Res_Data4/1000;   //Int_Data3 = 2
     Res_Data3 = Res_Data4%1000;   //Res_Data3 = 056
     
     Int_Data2 = Res_Data3/100;    //Int_Data2 = 0
     Res_Data2 = Res_Data3%100;    //Res_Data2 = 56
     
     Int_Data1 = Res_Data2/10;     //Int_Data1 = 5
     Res_Data1 = Res_Data2%10;     //Res_Data1 = 6
     
     if(Int_Data4>0)
     {
        *lptr++ = Int_Data4+48;    //lptr[0] = '2';  arr[0] = '2';
        *lptr++ = Int_Data3+48;    //lptr[1] = '2';  arr[1] = '2';
        *lptr++ = Int_Data2+48;    //lptr[2] = '0';  arr[2] = '0';
        *lptr++ = '.';             //lptr[3] = '.';  arr[3] = '.';
        *lptr++ = Int_Data1+48;    //lptr[4] = '5';  arr[4] = '5';
        *lptr++ = Res_Data1+48;    //lptr[5] = '6';  arr[5] = '6';
     }
     else if(Int_Data3>0)
     {
         *lptr++ = '0';
        *lptr++ = Int_Data3+48;
        *lptr++ = Int_Data2+48;
         *lptr++ = '.';
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }
       else if(Int_Data2>0)
     {
         *lptr++ = '0';
         *lptr++ = '0';
        *lptr++ = Int_Data2+48;
         *lptr++ = '.';
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }
       else if(Int_Data1>0)    
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '.';
         *lptr++ = Int_Data1+48;
         *lptr++ = Res_Data1+48;
     }   
     else if(Res_Data1>0)
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '.';
         *lptr++ = '0';
         *lptr++ = Res_Data1+48;  
     }
     else
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
        *lptr++ = '0';
     }
     *lptr =  '\0';
     return lptr;
}
void* TreePoint_To_ASCII(float data,char* lptr)
{
     u32 Int_Data5,Int_Data4,Int_Data3,Int_Data2,Int_Data1;
     u32 Res_Data5,Res_Data4,Res_Data3,Res_Data2,Res_Data1;  
     u32 Temp ;
	 
     Temp = (u32)(data*1000);       //������λ��Ч����;  199.78654*1000 = 199786 
     
     Int_Data5 = Temp/100000;       //Int_Data5 = 1
     Res_Data5 = Temp%100000;       //Int_Data5 = 99786 
     
     Int_Data4 = Res_Data5/10000;   //Int_Data4 = 9 
     Res_Data4 = Res_Data5%10000;   //Res_Data4 = 9786 
     
     Int_Data3 = Res_Data4/1000;    //Int_Data3 = 9 
     Res_Data3 = Res_Data4%1000;    //Res_Data3 = 786 
     
     Int_Data2 = Res_Data3/100;     //Int_Data2 = 7
     Res_Data2 = Res_Data3%100;     //Res_Data2 = 86 
     
     Int_Data1 = Res_Data2/10;      //Int_Data1 = 8
     Res_Data1 = Res_Data2%10;      //Res_Data1 = 6 
     
     if(Int_Data4>0)
     {
        *lptr++ = Int_Data5+48;     //lptr[0] = '1'
        *lptr++ = Int_Data4+48;     //lptr[1] = '9'
        *lptr++ = Int_Data3+48;     //lptr[2] = '9'
        *lptr++ = '.';              //lptr[3] = '.'
        *lptr++ = Int_Data2+48;     //lptr[4] = '7'
        *lptr++ = Int_Data1+48;     //lptr[5] = '8'
        *lptr++ = Res_Data1+48;     //lptr[6] = '6'
     }
     else if(Int_Data3>0)
     {
         *lptr++ = '0';
         *lptr++ = Int_Data4+48;
         *lptr++ = Int_Data3+48;
         *lptr++ = '.';
         *lptr++ = Int_Data2+48;
         *lptr++ = Int_Data1+48;
         *lptr++ = Res_Data1+48;
     }
       else if(Int_Data2>0)
       {
           *lptr++ = '0';
           *lptr++ = '0';
           *lptr++ = Int_Data3+48;
           *lptr++ = '.';
           *lptr++ = Int_Data2+48;
           *lptr++ = Int_Data1+48;
           *lptr++ = Res_Data1+48;
     }
       else if(Int_Data1>0)    
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '.';
         *lptr++ = Int_Data2+48;
         *lptr++ = Int_Data1+48;
         *lptr++ = Res_Data1+48;
     }   
     else if(Res_Data1>0)
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '.';
         *lptr++ = '0';
         *lptr++ = Int_Data1+48;
         *lptr++ = Res_Data1+48;
     }
     else
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '.';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
     }
     *lptr =  '\0';
     return lptr;
}
