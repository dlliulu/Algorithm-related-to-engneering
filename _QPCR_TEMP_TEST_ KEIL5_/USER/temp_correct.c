
#include "main.h"
#include "temp_correct.h"

/*
���ۺ�ø����Ӧ������У׼�淶����PCR�ǵ��¶ȿ��Ƴ����趨Ҫ������:
1 30��C 1min --> 2 95��C 3min --> 3 30��C 2min --> 4 90��C 3min -->
5 50��C 3min --> 6 70��C 3min -->7 60��C 3min --> 8 30��C 1min
*/

//�ñ���������
const float Temp_Comp[20] = {105.00, 95.00, 90.00, 70.00, 60.00, 50.00, 30.00};
//�ñ���¼У׼�豸��У׼�������
const float Temp_Correct[20] = {105.00, 95.00, 90.00, 70.00, 60.00, 50.00, 30.00};
//�ñ���¼��װ�ڸ�У׼�������
const float Temp_Gongzhc[20] = {105.00, 95.00, 90.00, 70.00, 60.00, 50.00, 30.00};


/*
�������ܣ�ʵ�ֶ�����ϵͳ���¶�У׼
���������float temprature,������ҪУ׼���¶�ֵ
���������У׼����¶�ֵ
NOTE:�˴����õ�бʽ����y = k*x + b�����Է���
*/
float Temp_Correction(float temprature)
{
	u16 X = 0;
	u16 i = 0;
	float T = 0;
	float k = 0;
	
	//������ڡ��ۺ�ø����Ӧ������У׼�淶���涨���¶�֮�⣬�򲻽���У׼��ԭ���Ƕ��پ��Ƕ���
	if ((temprature < 30.00) || (temprature > 105.00))
	{
		return temprature;
	}

	//�ڡ��ۺ�ø����Ӧ������У׼�淶���涨���¶�֮�⣬����зֶ�У׼
	if(temprature >= Temp_Comp[3])
	{
		/*
		����1��temprature = 93���� 93 > 70; i = 1; 93>90; i =2; 93<95; ����i = 2�� X = 3-2 = 1;
		����2��temprature = 98���� 98 > 70; i = 1; 98>90; i =2; 98<95; i=3�� 95<105; X = 3-3 = 0
		*/
		while(temprature >= Temp_Comp[3-i])
		{
			i++;
		} 
		X = 3 - i ;  
		//��б��
		k = (Temp_Correct[X+1] - Temp_Correct[X]) / (Temp_Gongzhc[X+1] - Temp_Gongzhc[X]);
		//���õ�бʽʵ�����Բ�ֵ
		T = Temp_Correct[X] + k*(temprature - Temp_Gongzhc[X]);  
	}
	else
	{
		/*
		����1��temprature = 55���� 55 < 70; i = 1; 55<60; i =2; 55>50; ����i = 2�� X = 3+2 = 5;
		����2��temprature = 36����36 < 70; i = 1; 36<60; i =2; 36<50; i=3�� 36>30;  ����i =3; X = 3+3 = 6
		*/
		while(temprature < Temp_Comp[3+i])
		{
			i++;
		}
		X = 3 + i; 
		k = (Temp_Correct[X] - Temp_Correct[X-1]) / (Temp_Gongzhc[X] - Temp_Gongzhc[X-1]);
		T = Temp_Correct[X-1] + k*(temprature - Temp_Gongzhc[X-1]); 
	}	
	return T;	
}

/*
NOTE:����ĺ������õ�бʽʵ�ֶַ����Բ�ֵ
	(1)temprature >= Temp_Comp[3]
	(x0, y0)-->( Temp_Gongzhc[X], Temp_Correct[X])
	(x1, y1)-->(Temp_Gongzhc[X+1], Temp_Correct[X+1])
	k = (y1-y0)/(x1-x0) = (Temp_Correct[X+1] - Temp_Correct[X]) / (Temp_Gongzhc[X+1] - Temp_Gongzhc[X]);
	y = y0 + k(x -x0)   =  Temp_Correct[X] + k*(temprature - Temp_Gongzhc[X]);               
	==> y = y0 + (y1-y0)*(x-x0)/(x1-x0);
	
	(2)temprature < Temp_Comp[3]
	(x0, y0)-->( Temp_Gongzhc[X-1], Temp_Correct[X-1])
	(x1, y1)-->(Temp_Gongzhc[X], Temp_Correct[X])
	k = (y1-y0)/(x1-x0) = (Temp_Correct[X] - Temp_Correct[X-1]) / (Temp_Gongzhc[X] - Temp_Gongzhc[X-1]);
	y = y0 + k(x -x0)   =  Temp_Correct[X-1] + k*(temprature - Temp_Gongzhc[X-1]);               
	==> y = y0 + (y1-y0)*(x-x0)/(x1-x0);	
*/


