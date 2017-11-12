#include "ADS8832.h"
#include "System_Init.h"
#include "stm32f2xx_spi.h"
#include "main.h"
extern  u16 ADC_Value[20];
extern  u8 Mid_Filt_Win;
void Qpcr_Delay(unsigned int n)
{
   unsigned int i;
   unsigned int j;
   for( i = n; i > 0; i --)
   {
      j=1;
      for(;j>0;j--)
      {
      }
   }
}
/************************************************************************/
//   ��������:׼��ADS8832��SPI����
//   �������:��
//   �������:��
//	 ȫ�ֱ���:�� 
//   ����:    yufangmin
//   �޸�����:2015/12/01
//   ��ע:    8b��
/************************************************************************/
void ADS8832_PrepareSpi(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	//APB2����ʱ��(PCLK2 = SYSCLK/2) = 60MHz
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //ʹ��SPI1��ʱ��

    SPI_Cmd(SPI1, DISABLE);
    /*spi CONFIGURATION*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫���������ȫ˫��ģʽ
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //��SPI1����Ϊ����ģʽ
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; //����SPI����֡��ʽ����8λ��16λ�����Ǵ˴�ѡ��16λ������֡��ʽ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;   //����ʱ�ӵļ��ԣ��������ô���ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  //����ʱ����λ��Ҳ����ѡ���ڴ���ͬ��ʱ�ӵĵڼ���������(�����ػ����½���)���ݱ�����
    //����Ϊ��һ�����ߵڶ������زɼ�����������ѡ���һ��������
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;     //����NSS�ź�����Ӳ��(NSS�ܽ�)����������ơ��˴�����ѡ��������ؼ�Ƭѡ
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//SPIʱ�ӵ�Ԥ��Ƶֵ,��2��Ƶ��256��Ƶ���˴����ǵĴ����ٶ�=60MHz/SPI_BaudRatePrescaler_8 = 7.5MHz;;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //�˴������������ݵĴ���˳����MSBλ��ǰ����LSBλ��ǰ���˴�����ѡ��MSB��λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;     //CRC����͵Ķ���ʽ
    SPI_Init(SPI1, &SPI_InitStructure);   //��ʼ��SPI1��������
    SPI_Cmd(SPI1, ENABLE);      //ʹ��SPI1
    
}
/************************************************************************/
//   ��������:��ʼ��ADC
//   �������:��
//   �������:��
//	 ȫ�ֱ���:�� 
//   ����:    yufangmin
//   �޸�����:2015/12/01
//   ��ע:    GPIOG14->EOC
/************************************************************************/
void ADS8832_Init(void)
{
    u16 cfr1, cfr2;
    cfr1 = 0X07FD;//�ֶ����� 0X07FD = 0b 0000 0111 1111 1101 
    //bit[11] = 0; �ֶ�ѡ��ͨ��        bit[10] = 1; Conversion clock (CCLK) = internal OSC
	//bit[9] = 1;  �ֶ�����ת��        bit[8] = 1;  ��λ��Ҫ������Զ�����ģʽ�µĲ����ʵ�����

	//bit[7] = 1; EOC/INT active low   bit[6] = 1;  EOC/INT��������ΪEOC����
	//bit[5] = 1;                      bit[4] = 1;

	//bit[3] = 0;                      bit[2] = 1; 
	//bit[1] = 0;                      bit[0] = 1; 


	//#define ADC0_CS_H()       GPIO_SetBits(GPIOC, GPIO_Pin_4)
	//��PC4��1����ADC0δѡ��
    ADC0_CS_H();
	
	//#define ADC1_CS_H()       GPIO_SetBits(GPIOB, GPIO_Pin_1)
    //��PB1��1����ADC1δѡ��
    ADC1_CS_H();
	
	//#define ADC2_CS_H()       GPIO_SetBits(GPIOG, GPIO_Pin_1)
    ADC2_CS_H();

	//#define ADC0_CONV_H()     GPIO_SetBits(GPIOC, GPIO_Pin_5)
	//��PC5��1������ֹADC0ת��
    ADC0_CONV_H();

	//#define ADC1_CONV_H()     GPIO_SetBits(GPIOB, GPIO_Pin_2)
	//��PB2��1������ֹADC1ת��
    ADC1_CONV_H();
    ADC2_CONV_H();

	
    //****************************************************
	//ʹ��ADC0
	
    //#define ADC_0    0
    ADS8832_WriteCfr(cfr1,ADC_0);     //�ں����ڲ����ת��
    Qpcr_Delay(10);
    while(1)
    {
        cfr2  = ADS8832_ReadCfr(ADC_0);   //��ȡ��������
        Qpcr_Delay(10);
        if(cfr1==cfr2)                    //������óɹ����Ƴ���ѭ�����������δ�ɹ�������������
            break;
        ADS8832_WriteCfr(cfr1,ADC_0);
        Qpcr_Delay(10);
    }

	
    //***************************************************
	//ʹ��ADC1
	
    ADS8832_WriteCfr(cfr1,ADC_1);
    Qpcr_Delay(10);
    cfr2 = 0;
    while(1)
    {
        cfr2  = ADS8832_ReadCfr(ADC_1);
        Qpcr_Delay(10);
        if(cfr1==cfr2)
            break;
        ADS8832_WriteCfr(cfr1,ADC_1);
        Qpcr_Delay(10);
    }

	
    //***************************************************
//    ADS8832_WriteCfr(cfr1,ADC_2);
//    Qpcr_Delay(10);
//    while(1)
//    {
//        cfr2  = ADS8832_ReadCfr(ADC_2);
//        Qpcr_Delay(10);
//        if(cfr1==cfr2)
//            break;
//        ADS8832_WriteCfr(cfr1,ADC_2);
//        Qpcr_Delay(10);
//    }
}


/************************************************************************/
//   ��������:ADS8832��ʼת��
//   �������:��
//   �������:��
//	 ȫ�ֱ���:�� 
//   ����:    yufangmin
//   �޸�����:2015/12/01
//   ��ע:    Convst�½��ؿ�ʼת��
/************************************************************************/
void ADS8832_Convst(u8 ADC_x)
{
    if(ADC_x ==0)
    {
        ADC0_CONV_L();
        Qpcr_Delay(15);	
        ADC0_CONV_H();
    }
    else if(ADC_x ==1)
    {
        ADC1_CONV_L();
        Qpcr_Delay(15);	
        ADC1_CONV_H();
    }
    else
    {
        ADC2_CONV_L();
        Qpcr_Delay(15);	
        ADC2_CONV_H();
    }
}

/************************************************************************/
//   ��������:��ȡADS8832��������
//   �������:��
//   �������:u16, ADS8832��������
//	 ȫ�ֱ���:�� 
//   ����:    yufangmin
//   �޸�����:2015/12/01
//   ��ע:    
/************************************************************************/
u16 ADS8832_ReadCfr(u8 ADC_x)
{
	//������д�����û��Ƕ�ȡ���ã����ȵ�һ���϶�����Ҫ����Ӧ��ADCоƬѡ�У��͵�ƽ��Ч
    switch(ADC_x)
    {
        case 0: ADC0_CS_L();
        break;
        case 1: ADC1_CS_L();
        break;
        case 2: ADC2_CS_L();
        break;
        default:
        break;
    }
    Qpcr_Delay(10);

	//SPI_I2S_GetFlagStatus()Checks whether the specified SPIx/I2Sx flag is set or not.
	//SPI_I2S_FLAG_RXNE: Receive buffer not empty flag.
    if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET )
    {
        SPI_I2S_ReceiveData(SPI1);   //SPI_I2S_ReceiveData():Returns the most recent received data by the SPIx/I2Sx peripheral.
    }
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET );
    SPI_I2S_SendData(SPI1, ADS8332_READ_CFR); 
    Qpcr_Delay(2);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET );	
    while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
    Qpcr_Delay(2);
    switch(ADC_x)
    {
        case 0: ADC0_CS_H();
        break;
        case 1: ADC1_CS_H();
        break;
        case 2: ADC2_CS_H();
        break;
        default:
        break;
    } 
    SPI_I2S_ReceiveData(SPI1);
    return (SPI_I2S_ReceiveData(SPI1)&0xfff);
}

/************************************************************************/
//   ��������:д��ADS8832��������
//   �������:u16, ADS8832��������
//   �������:��
//	 ȫ�ֱ���:�� 
//   ����:    yufangmin
//   �޸�����:2015/12/01
//   ��ע:    
/************************************************************************/
void ADS8832_WriteCfr(u16 CfrData,u8 ADC_x)
{
    switch(ADC_x)
    {
        case 0: ADC0_CS_L();
        break;
        case 1: ADC1_CS_L();
        break;
        case 2: ADC2_CS_L();
        break;
        default:
        break;
    }
    Qpcr_Delay(10);

	
	//**************************************************************

	//���漸�������Ҫ��:����һֱ�ȵ�SPI��bufferΪ��Ϊֹ��Ȼ�������ݣ��ȵ����ݷ�����ɣ�bufferΪ�գ���Ƭѡ�ź����ߣ�����ADC
	//SPI_I2S_GetFlagStatus()Checks whether the specified SPIx/I2Sx flag is set or not.
	//SPI_I2S_FLAG_TXE: Transmit buffer empty flag.
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (ADS8332_WRITE_CFR | (0x0fff&CfrData)));
    Qpcr_Delay(2);
    while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);	
    Qpcr_Delay(2);
	//**************************************************************


	//��Ƭѡ�ź����ߣ�����ADC
    switch(ADC_x)
    {
        case 0: ADC0_CS_H();
        break;
        case 1: ADC1_CS_H();
        break;
        case 2: ADC2_CS_H();
        break;
        default:
        break;
    }
}

/************************************************************************/
//   ��������:����ADS8832����ͨ��
//   �������:u8, ͨ��ֵ 0-7
//   �������:��
//	 ȫ�ֱ���:�� 
//   ����:    yufangmin
//   �޸�����:2015/12/01
//   ��ע:    
/************************************************************************/
void ADS8832_SetChannel(u8 ChannelNum)
{
    
    u16 ChnSelCmd;
    if(ChannelNum<8)
    {
        ADC0_CS_L();
    }
    else if(ChannelNum<16)
    {
        ADC1_CS_L();
        ChannelNum -= 8;
    }
    else
    {
        ADC2_CS_L();
        ChannelNum -= 16;
    }
    ChnSelCmd = (ChannelNum<<12);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, ChnSelCmd);
    Qpcr_Delay(100);
    while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
    if(ChannelNum<8)
    {
        ADC0_CS_H();
    }
    else if(ChannelNum<16)
    {
        ADC1_CS_H();
    }
    else
    {
        ADC2_CS_H();
    }
}

/************************************************************************/
//   ��������:������ȡADS8832��16��ͨ���Ĳ������
//   �������:u8, ͨ��ֵ 0-15
//   �������:��
//	 ȫ�ֱ���:�� 
//   ����:    yufangmin
//   �޸�����:2015/12/01
//   ��ע:    
/************************************************************************/
void ADS8832_16Channel_Sample(void)
{
    u16 ChnSelCmd = 0;
    u8  i = 0; 
   
    for(i = 0; i < 8; ++i)
    {         
       //******************�����л�ͨ������ͬʱ�����ϴ�ת������**********
        ADC0_CS_H();    
        ADC0_CS_L(); 
        ChnSelCmd = (i<<12);	//ÿ��i++ʵ��ͨ���л�
        SPI_I2S_SendData(SPI1, ChnSelCmd); 
        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);  
        ADC0_CS_H();
        SPI_I2S_ReceiveData(SPI1);
        //******************��ʼת��***********************
        Qpcr_Delay(3000); //��������ʱ�� ʱ��̫�ͻᵼ�²������ݳ䲻��
        ADS8832_Convst(ADC_0); 
        
        //*****************************************
        ADC0_CS_L();      
        SPI_I2S_SendData(SPI1, ADS8332_READ_DATA);
        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
        ADC_Value[i] = SPI_I2S_ReceiveData(SPI1);
        ADC0_CS_H();
		//ÿ�βɼ���ɺ��������˲�
        ADC_Value[i] = Mid_Data_Filter(ADC_Value[i] , Mid_Filt_Win , i);//Mid_Data_Num Ϊ����
        //Qpcr_Delay(10);
    }
   for(i = 0; i < 8; ++i)
    {         
       //******************�����л�ͨ������ͬʱ�����ϴ�ת������**********
        ADC1_CS_H();    
        ADC1_CS_L(); 
        ChnSelCmd = (i<<12);	//(8<<12);// 
        SPI_I2S_SendData(SPI1, ChnSelCmd); 
        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);  
        ADC1_CS_H();
        SPI_I2S_ReceiveData(SPI1);
        //******************��ʼת��***********************
        Qpcr_Delay(3000); //��������ʱ�� ʱ��̫�ͻᵼ�²������ݳ䲻��
        ADS8832_Convst(ADC_1); 
        //*****************************************
        ADC1_CS_L();      
        SPI_I2S_SendData(SPI1, ADS8332_READ_DATA);
        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
        ADC_Value[i+8] = SPI_I2S_ReceiveData(SPI1);
        ADC1_CS_H();
        ADC_Value[i+8] = Mid_Data_Filter(ADC_Value[i+8] , Mid_Filt_Win , i+8);//Mid_Data_Num Ϊ����
      //  Qpcr_Delay(10);
    }
}

void ADS8832_16Channel_10TimesSample(void)
{
//    u16 ChnSelCmd = 0;
//    u8 j, i = 0;
//    u8 k = 0;
//    u16 Max_Data = 0;
//    u16 Min_Data = 65535;
//    u32 Sum_Data = 0;
//    u16 Temp[15] = 0;
//    for(i = 0; i < 8; ++i)
//    {         
//       //******************�����л�ͨ������ͬʱ�����ϴ�ת������**********
//        ADC0_CS_H();    
//        ADC0_CS_L(); 
//        ChnSelCmd = (i<<12);	//0;// 
//        SPI_I2S_SendData(SPI1, ChnSelCmd); 
//        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET); 
//        Sum_Data = 0;
//        Max_Data = 0;
//        Min_Data = 65535;
//        ADC0_CS_H();
//        Qpcr_Delay(150); //��������ʱ�� ʱ��̫�ͻᵼ�²������ݳ䲻��
//       // for(j = 0; j < 10; ++j)
//        {
//
//          SPI_I2S_ReceiveData(SPI1);
//          //******************��ʼת��***********************
//          ADS8832_Convst(ADC_0); 
//          //*****************************************
//          ADC0_CS_L();      
//          SPI_I2S_SendData(SPI1, ADS8332_READ_DATA);
//          while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
//          ADC_Value[i] = SPI_I2S_ReceiveData(SPI1);//Temp[j] = SPI_I2S_ReceiveData(SPI1);//ADC_Value[i] = SPI_I2S_ReceiveData(SPI1);
////          Sum_Data += Temp[j];
////          if(Max_Data<Temp[j])
////          {
////            Max_Data = Temp[j];
////          }
////          if(Min_Data>Temp[j])
////          {
////            Min_Data = Temp[j];
////          }
//          ADC0_CS_H();
//         // Qpcr_Delay(10);
//        }
////        Sum_Data -= Max_Data;
////        Sum_Data -= Min_Data;
////        ADC_Value[i] = Sum_Data>>3;
//    }
//   for(i = 0; i < 8; ++i)
//    {         
//       //******************�����л�ͨ������ͬʱ�����ϴ�ת������**********
//        ADC1_CS_H();    
//        ADC1_CS_L(); 
//        ChnSelCmd = (i<<12);	//(8<<12);// 
//        SPI_I2S_SendData(SPI1, ChnSelCmd); 
//        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);  
//        Sum_Data = 0;
//        Max_Data = 0;
//        Min_Data = 65535;
//        ADC1_CS_H();
//        Qpcr_Delay(150); //��������ʱ�� ʱ��̫�ͻᵼ�²������ݳ䲻��
//    //     for(j = 0; j < 10; ++j)
//         {
//
//           SPI_I2S_ReceiveData(SPI1);
//           //******************��ʼת��***********************
//           ADS8832_Convst(ADC_1); 
//           //*****************************************
//           ADC1_CS_L();      
//           SPI_I2S_SendData(SPI1, ADS8332_READ_DATA);
//           while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
//          ADC_Value[i+8] = SPI_I2S_ReceiveData(SPI1);// Temp[j] = SPI_I2S_ReceiveData(SPI1);//  ADC_Value[i+8] = SPI_I2S_ReceiveData(SPI1);
////           Sum_Data += Temp[j];
////           if(Max_Data<Temp[j])
////           {
////             Max_Data = Temp[j];
////           }
////           if(Min_Data>Temp[j])
////           {
////             Min_Data = Temp[j];
////           }
//           ADC1_CS_H();
//         //  Qpcr_Delay(10);
//         }
////         Sum_Data -= Max_Data;
////        Sum_Data -= Min_Data;
////        ADC_Value[i + 8] = Sum_Data>>3;
//    }
}
/************************************************************************/
//   ��������:��λADS8832
//   �������:��
//   �������:��
//	 ȫ�ֱ���:�� 
//   ����:    yufangmin
//   �޸�����:2015/12/01
//   ��ע:    Reset�͵�ƽ��λADC
/************************************************************************/
//void ADS8832_Reset(void)
//{
//}

/************************************************************************/
//   ��������:��ȡADS8832ָ��ͨ���Ĳ������
//   �������:u8, ͨ��ֵ 0-7
//   �������:��
//	 ȫ�ֱ���:�� 
//   ����:    yufangmin
//   �޸�����:2015/12/01
//   ��ע:    
/************************************************************************/
//u16 ADS8832_TakeSample(void)
//{
//	u16 res=0;
//
//        Qpcr_Delay(2);
//        ADS8832_Convst(); 
//        while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)==Bit_RESET);		// wait for EOC high
//       ADC1_CS_L();
//       if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET )
//        {
//            SPI_I2S_ReceiveData(SPI1);
//        }
//        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);    
//        SPI_I2S_SendData(SPI1, ADS8332_READ_DATA);
//        Qpcr_Delay(2);
//        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
//        res = SPI_I2S_ReceiveData(SPI1);
//	ADC1_CS_H();
//	return  res;
//}

/************************************************************************/
//   ��������:����ADS8832��������
//   �������:��
//   �������:��
//	 ȫ�ֱ���:�� 
//   ����:    yufangmin
//   �޸�����:2015/12/01
//   ��ע:    
/************************************************************************/
//void ADS8832_Cont_Sample(void)
//{
//	ADS8832_WriteCfr(ADS8332_CH_AUTO|ADS8332_INT_CLK|ADS8332_SMPL_250K|ADS8332_EOC_LOW|ADS8332_P10_EOC|ADS8332_P10_OUT|ADS8332_AUTONAP_DIS|ADS8332_NAP_DIS|ADS8332_PD_DIS|ADS8332_NOSW_RST);//|ADS8332_TAG_OUT
//}
