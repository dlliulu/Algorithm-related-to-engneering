
#include "led.h"
#include "12864.h"
#include "time_init.h"
#include "max6675.h"
#include "pid.h"
#include "pidout.h"

u16 Kms10;
void delay(u32 x)
{
  while(x--);
}

void delay_us(u16 us)
{
	 u8 i;
  while(us--)
	{
	  for(i=0;i<6;i++)
		{
		
		}
	}
}

void Isr_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;// TIM4_IRQChannel; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
NVIC_Init (&NVIC_InitStructure); 


NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;// TIM3_IRQChannel; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
NVIC_Init (&NVIC_InitStructure);
	
}

 void TIM4_IRQHandler() //10ms 1��
{					  
  
	static u8 tsec; u8 st;
	st=TIM_GetFlagStatus(TIM4, TIM_IT_Update);
	if(st!=0)
	{  TIM_ClearFlag(TIM4, TIM_IT_Update); 
		Kms10++;
		if(tsec++>=100)
		{
		  tsec=0;
		  pid.times++;     //ÿ1s�У�pid.times++,Ϊ���崦�����ÿ���
		}
	}
}

 void TIM3_IRQHandler() //1ms 1��
{					  
   u8 st;
	st=TIM_GetFlagStatus(TIM3, TIM_IT_Update);
	if(st!=0)
	{  
		pid.C10ms++;  // ÿ1ms�Լ�һ
		TIM_ClearFlag(TIM3, TIM_IT_Update); 
     	PID_out();    //���PID������������
	}
}

void PID_Init()
{
  	pid.Sv=120;//�û��趨�¶�
	pid.Kp=30;
	pid.T=500;//PID��������
  	pid.Ti=5000000;//����ʱ��
	pid.Td=1000;//΢��ʱ��
	pid.pwmcycle=200;//pwm����1000
	pid.OUT0=1;
}


void Display()
{
	Show_string1(0x90,"�趨ֵ:");
	Show_number(0X94,pid.Sv);	
	
	Show_string1(0x88,"��ǰֵ:");
	Show_number(0X8c,pid.Pv);
	
	Show_string1(0x98,"PIDOUT:");
	Show_number(0X9c,pid.OUT);	
}


int main()
{
   	 LCD_Init();
	 Show_string1(0x80,"λ��ʽPID");
	 Timer4_init();	//T4 10msʱ��	
	 Isr_Init();
	 Max6675_Init();
	 PID_Init();
	 PIDOUT_init();
	 Timer3_init();	//T3 1msʱ��
   while(1)
   {
    	read_temper();//��ȡ��ǰ�¶�
		PID_Calc(); //pid���� 
		Display(); 
   }
}












