
 #include "time_init.h"

#include"stm32f10x_conf.h"

  
void Timer4_init()	//T4 10msʱ��
{	 //72000000/7200=100us
//
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4,ENABLE);

 

TIM_TimeBaseStructure.TIM_Period = 10000-1; //��������     //100us*10=1000us=10ms
TIM_TimeBaseStructure.TIM_Prescaler =72-1;//��Ƶֵ   	    
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 	//�ָ�ʱ��			
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
 TIM_DeInit(TIM4);
TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure); 
TIM_Cmd(TIM4, ENABLE); 	 //ʹ�ܶ�ʱ��2

 /*���¶�ʱ��4�жϳ�ʼ��*/
TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //���ϼ�����������ж�


}
  
void Timer3_init()	//T3 1msʱ��
{	 //72000000/7200=100us
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3,ENABLE);

 

TIM_TimeBaseStructure.TIM_Period = 1000-1; //��������     //1ms
TIM_TimeBaseStructure.TIM_Prescaler =72-1;//��Ƶֵ   	    
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 	//�ָ�ʱ��			
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
 TIM_DeInit(TIM3);
TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure); 
TIM_Cmd(TIM3, ENABLE); 	 //ʹ�ܶ�ʱ��2

 /*���¶�ʱ��4�жϳ�ʼ��*/
TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //���ϼ�����������ж�


}

