#include "pid.h"

/*
��OUT= OUTK- OUTk-1 =kp(EK-EK-1)+((Kp*T)/Ti)Ek+(((Kp*TD)/T)*(Ek-2Ek-1+Ek-2))��
Kp:�㷨�������
Ek:���ε�ƫ��
Ek-1:�ϴε�ƫ��
Ek-2:���ϴε�ƫ��
Ti:����ʱ��
TD:΢��ʱ�䳣��
Ek = Sv-Xk;����: SvΪ�趨ֵ��XkΪ��ǰ����ֵ�������һ�β���ֵ
T:�������ڣ�Ҳ�п������ڣ�ÿ��Tʱ��ν���һ��PID����
*/

PID pid;
extern u16 pidcalcms;
void PIDParament_Init()  
{
  	pid.set =100;         //��ʼ�趨�¶�Ϊ100��
	pid.currpwm=0;        //��ʼPWM����Ϊ0
	pid.pwm_cycle=100;    //PWM������Ϊ100
	pid.calc_cycle=100;  
	
	pid.Td=2000;
	pid.Ti=4000;
	pid.Kp=5;
	pid.Tsam=500;
}

void pid_calc()  //500ms����һ��PID���㣬200ms���²���һ�Σ���ǰֵ
{
  float dk1;float dk2;
  float t1,t2,t3;
  
	//pidcalcmsΪȫ�ֱ�����ÿ��1ms�Լ�һ
	if(pidcalcms<pid.Tsam) return ;  //�ӵ�Tsam = 500 msʱ��PID�ļ�������ʱ�䵽����ʼPID���������򲻴������Ƴ��ú���
	pid.En=pid.set-pid.curr;         //������� ����P
	//ִ��������ָ���En,En_1,En_2��ֵ�ı仯
	//        ��һ��: En = 5; En_2 = 0; En_1 = 0;
	//        �ڶ���: En = 2; En_2 = 0; En_1 = 5; //Ҳ��˵ÿ�Σ�En������ֵ����En_2����һ�ε�En_1,������һ�ε�ƫ�En_1����һ�ε�En
	//        ������: En = 3; En_2 = 5; En_1 = 2; //��˾��ܱ�֤ÿ�ζ��Ǳ���ƫ���һ��ƫ�����һ��ƫ��
	//		  ���Ĵ�: En = 11; En_2 = 2; En_1 = 3;
	//        �����: En = -2; En_2 = 3; En_1 = 11;
	dk1=pid.En-pid.En_1;             //����ƫ�����ϴ�ƫ��֮�΢��D
	dk2=pid.En-2*pid.En_1+pid.En_2;  //dk2 = (pid.En_2 - pid.En_1) - (pid.En_1 - pid.En)

/*��OUT= OUTK- OUTk-1 =kp(EK-EK-1)+((Kp*T)/Ti)Ek+(((Kp*TD)/T)*(Ek-2Ek-1+Ek-2))��*/	
    //һ�����ȼ�����ʽ�е�kp(EK-EK-1)����
    t1=pid.Kp*dk1; 
	//����Ȼ�������ʽ�е�((Kp*T)/Ti)Ek��һ��(((Kp*TD)/T)*(Ek-2Ek-1+Ek-2)) = ((Kp*TD)/T)*dk2;
	t2=(pid.Kp*pid.Tsam)/pid.Ti;
	t2=t2*pid.En;
	//������������ʽ�е����һ�
	t3=(pid.Kp*pid.Td)/pid.Tsam;
	t3=t3*dk2;
	
  	pid.Dout=t1+t2+t3;  //����Ӧ�����������
  	
	pid.currpwm+=pid.Dout;  //�����һ�μ��������PID�����Ժ�PIDֵ����Ҫ�ı仯�������һ��

	//���ռ�ձ�>=100%�������������100%���������
	if(pid.currpwm>pid.pwm_cycle)
	{
	  pid.currpwm=pid.pwm_cycle;
	}
	//�������ռ�ձ�<0%�������������0%���������
	if(pid.currpwm<0)
	{
	 pid.currpwm=0;
	}
	//����ƫ��ֵ����Ϊÿ�α仯�Ķ���pid.En,

	pid.En_2=pid.En_1;
	pid.En_1=pid.En;
	//����:��ָ������һ�����֮������
	//        ��һ��: En = 5; En_2 = 0; En_1 = 5;
	//        �ڶ���: En = 2; En_2 = 5; En_1 = 2;
	//        ������: En = 3; En_2 = 2; En_1 = 3;
	//		  ���Ĵ�: En = 11; En_2 = 3; En_1 = 11;
	//        �����: En = -2; En_2 = 11; En_1 = -2;

	//��pidcalcms��ֵΪ0����ʼ��һ��PID����
    pidcalcms=0;
}