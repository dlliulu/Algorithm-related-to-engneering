/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"
#include "main.h"
u16 Delay_Tim3 = 0;
u16 LED_Status_Delay = 0;
/** @addtogroup Template_Project
  * @{
  */
extern uint16_t  sEEAddress ;   
extern uint32_t  sEETimeout ;   
extern uint16_t* sEEDataReadPointer;   
extern uint8_t*  sEEDataWritePointer;  
extern uint8_t   sEEDataNum;
extern u8 RxBuffer_3[50];
extern u8 RxBuffer_2[50];
extern u8 RxCounter_3;
extern u8 Tim3_Flag;
extern u8 RxTim3;
extern u8 RxTim2;
extern u8 NEW_ORDER_FLAG;
extern u8 Usart2_FLAG;
extern u8 Col_Times;
extern u8 Data_Out_Flag;
extern u8 USART2_BUSY ;
extern u8 LCD_Display_Flag ;
u16 Display_Delay;

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/


/*
5A A5 04 83 0000 01 	

5A A5  ��ʾ��֡ͷ				 
04	   ��ʾ�����ݳ���				 
83	   ��ʾ�������ݴ洢��ָ��				 
0000   ��ʾ��������ַ�������ֽڣ�				 
01	   ��ʾ���� 0000 ��ַ��ʼ�� 1 ���ֳ�������				 

*/
void USART1_IRQHandler(void)   //�ʹ��������н���ͨ��
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    //���USART_IT_RXNE�����ж��Ƿ���
    {
		//�����˽����жϣ������ݵ���
        USART_ClearITPendingBit(USART1, USART_IT_RXNE );  //���־λ
        if((USART_ReceiveData(USART1) == 0XA5) || ((RxBuffer_2[0] == 0XA5)&&(USART_ReceiveData(USART1) == 0X5A))||((RxBuffer_2[0] == 0XA5)&&(RxBuffer_2[1] == 0X5A)))
        {
			//USART_ReceiveData()�ú�����Ϊ�˻�ȡUSART2��ȡ����������
            RxBuffer_2[RxTim2] = USART_ReceiveData(USART1);   
            
            if(RxTim2 > 2)//RxBuffer_2[2]������֡�ĳ���
            {
                if(RxTim2 >= RxBuffer_2[2] + 2 )
                {
                    Usart2_FLAG = 1;
                    RxTim2 = 0;
                }
            }
            else if(RxTim2>50)//���մ��󳬳���Χ
            {
                RxBuffer_2[0] = 0;
                RxBuffer_2[1] = 0;
                RxBuffer_2[2] = 0;
                RxTim2 = 0;
                Usart2_FLAG = 0;
            }
            RxTim2 ++;
        }
        else
        {
            RxBuffer_2[0] = 0;
            RxBuffer_2[1] = 0;
            RxBuffer_2[2] = 0;
            RxTim2 = 0;
            Usart2_FLAG = 0;
        }
    }
}

//void USART2_IRQHandler(void)   //�ʹ��������н���ͨ��
//{
//    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //���USART_IT_RXNE�����ж��Ƿ���
//    {
//		//�����˽����жϣ������ݵ���
//        USART_ClearITPendingBit(USART2, USART_IT_RXNE );  //���־λ
//        if((USART_ReceiveData(USART2) == 0XA5) || ((RxBuffer_2[0] == 0XA5)&&(USART_ReceiveData(USART2) == 0X5A))||((RxBuffer_2[0] == 0XA5)&&(RxBuffer_2[1] == 0X5A)))
//        {
//			//USART_ReceiveData()�ú�����Ϊ�˻�ȡUSART2��ȡ����������
//            RxBuffer_2[RxTim2] = USART_ReceiveData(USART2);   
//            
//            if(RxTim2 > 2)//�������
//            {
//                if(RxTim2 >= RxBuffer_2[2] + 2 )
//                {
//                    Usart2_FLAG = 1;
//                    RxTim2 = 0;
//                }
//            }
//            else if(RxTim3>50)//���մ��󳬳���Χ
//            {
//                RxBuffer_2[0] = 0;
//                RxBuffer_2[1] = 0;
//                RxBuffer_2[2] = 0;
//                RxTim2 = 0;
//                Usart2_FLAG = 0;
//            }
//            RxTim2 ++;
//        }
//        else
//        {
//            RxBuffer_2[0] = 0;
//            RxBuffer_2[1] = 0;
//            RxBuffer_2[2] = 0;
//            RxTim2 = 0;
//            Usart2_FLAG = 0;
//        }
//    }
//}


void USART2_IRQHandler(void)   //��PC���н���ͨ��   
{
	//У������: ��ʼ��    ����   ͨ��  ����	 ����   У��  ������
	//ʾ��:        $	 JZ/JT	  15	 6	115.27	  A	     ��	
	
	//У������: ��ʼ��	����	���Ƶ��	�˲�����	�����ʽ	���ͨ��	У��	������
	//ʾ��:       $	     CJ	       10	       9	        0	       FF	      A	       ��
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE );
        if((USART_ReceiveData(USART2) == '$') || (RxBuffer_3[0] ==  '$'))
        {
			//PC�����ݷ��͹�������ʼ��������
            RxBuffer_3[RxTim3] = USART_ReceiveData(USART2);      
            if(RxBuffer_3[RxTim3] == ';')//�������
            {
                NEW_ORDER_FLAG = 1;
            }
            else if(RxTim3>50)//���մ��󳬳���Χ
            {
                RxBuffer_3[0] = 0;
                RxTim3 = 0;
                NEW_ORDER_FLAG = 0;
            }
            RxTim3 ++;
        }
        else
        {
            RxBuffer_3[0] = 0;
            RxTim3 = 0;
            NEW_ORDER_FLAG = 0;
        }
    }
}


//void USART3_IRQHandler(void)   //��PC���н���ͨ��   
//{
//	//У������: ��ʼ��    ����   ͨ��  ����	 ����   У��  ������
//	//ʾ��:        $	 JZ/JT	  15	 6	115.27	  A	     ��	
//	
//	//У������: ��ʼ��	����	���Ƶ��	�˲�����	�����ʽ	���ͨ��	У��	������
//	//ʾ��:       $	     CJ	       10	       9	        0	       FF	      A	       ��
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//    {
//        USART_ClearITPendingBit(USART3, USART_IT_RXNE );
//        if((USART_ReceiveData(USART3) == '$') || (RxBuffer_3[0] ==  '$'))
//        {
//			//PC�����ݷ��͹�������ʼ��������
//            RxBuffer_3[RxTim3] = USART_ReceiveData(USART3);      
//            if(RxBuffer_3[RxTim3] == ';')//�������
//            {
//                NEW_ORDER_FLAG = 1;
//            }
//            else if(RxTim3>50)//���մ��󳬳���Χ
//            {
//                RxBuffer_3[0] = 0;
//                RxTim3 = 0;
//                NEW_ORDER_FLAG = 0;
//            }
//            RxTim3 ++;
//        }
//        else
//        {
//            RxBuffer_3[0] = 0;
//            RxTim3 = 0;
//            NEW_ORDER_FLAG = 0;
//        }
//    }
//}


/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);    
        //  GPIOA->ODR ^= GPIO_Pin_8;			      
        Delay_Tim3++;
        Display_Delay++;			
				LED_Status_Delay++;

		    if(LED_Status_Delay > 0)	        //20*5ms = 100ms����AD�ɼ�һ�Σ���������ʾϵͳ���е�״̬��LED���з�תһ��
        {
            LED_Status_Delay = 0;
            Tim3_Flag = 1;
        }
				
        if(Delay_Tim3 > Col_Times)	        //Ĭ��1s������PC������
        {
            Delay_Tim3 = 0;
            Data_Out_Flag = 1;
        }
        if(Display_Delay > 20)		        //20*20ms��������������
        {
            Display_Delay = 0;
            LCD_Display_Flag = 1;
        }
    }
}
void DMA2_Stream7_IRQHandler(void)
{
 if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7)!= RESET)
 {
		DMA_ClearFlag(DMA2_Stream7,DMA_IT_TCIF7);
		USART2_BUSY = 0;
   //  GPIOA->ODR ^= GPIO_Pin_8;
 }
}
//void DMA1_Stream0_IRQHandler(void)
//{
//    /* Check if the DMA transfer is complete */
//    if(DMA_GetFlagStatus(DMA1_Stream0, DMA_FLAG_TCIF0) != RESET)
//    {      
//        /*!< Send STOP Condition */
//        I2C_GenerateSTOP(I2C1, ENABLE);    
//        
//        /* Disable the DMA Rx Stream and Clear TC Flag */  
//        DMA_Cmd(DMA1_Stream0, DISABLE);
//        DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
//        
//        /* Reset the variable holding the number of data to be read */
//        *sEEDataReadPointer = 0;
//    }
//}
void DMA1_Stream6_IRQHandler(void)
{
    /* Check if the DMA transfer is complete */
    if(DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET)
    {  

        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
 //       USART2_BUSY = 0;
    }
}

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
