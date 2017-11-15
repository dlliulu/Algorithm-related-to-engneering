#include <stdio.h>

//�ݹ����ϸ���̷���

void recursion(int n);
/****************************************
ִ�н����
Before recursion, n = 3.
Before recursion, n = 2.
Before recursion, n = 1.
End recursion, n = 1.
After recursion, n = 1.
After recursion, n = 2.
After recursion, n = 3.
******************************************/

int main(void)
{
	recursion(3);
	
	return 0;
}

void recursion(int n)
{
	printf("Before recursion, n = %d.\n", n);
	if (n > 1)
	{
		recursion(n-1);
	}
	else
	{
		printf("End recursion, n = %d.\n", n);
	}
	printf("After recursion, n = %d.\n", n);
}
/*
ִ�н��������
       (1)n=3; Before recursion, n = 3.
       (2)n>1. n-1 = 2; Before recursion, n = 2.
	   (3)n>2. n-1 = 1; Before recursion, n = 1.
	   (4)n=1. Ŀǰ�ոս�����recursion(1);�˺����У����ڲ�����n>1���������ʻ�ֱ��ִ�к�
	   ��ĵ�������䣬��ʱ��End recursion, n = 1.After recursion, n = 1.
	   (5)��ʱ��recursion(1);�ú���ִ����ɣ�����ʱ�൱����recursion(2);�ú�����if(2>1)
	   �ڲ����ִ����ɣ�else����䲻���㣬��������ִ�У��ʴ�ʱ��ִ��After recursion, n = 2.
	   (6)�������������֮���൱����recursion(3);�˺�����if(3>1)����䱻ִ����ɣ���
	      else�����������������Բ�ִ��End recursion...,����ִ��After recursion, n = 3.������䡣
*/



