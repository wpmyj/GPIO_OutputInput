#include "Time.h"

//����ʱ�ļ�ʱ���� ���1s��1min
void Time_Count0(void)
{
	if(1 == CountDown_Flag)
	{
		if(0 == (Count0 % 498))				//1����--498(����֮��)  ���Ե�ʱ����5s--50 
		{
			Count0 = 1;
			Minute_Flag0 = 1;	
			Second_Flag0 = 1;
		}
		else if(0 == (Count0 % 10)) //1s--10 
			{
				Count0++;
				Second_Flag0 = 1;
			}
			else
				Count0++;
	}
	else
	{}
}

/*������ʱ5�롢����1�����޶����رձ���*/
void Time_Count1(void)
{
	if(1 == SelfTest_flag)   //�Լ��־λ 0��Ϊ����״̬1��Ϊ�Լ�ģʽ 2��Ϊ�ֶ��Զ�ģʽѡ�� 3�������Զ�ģʽ
	{
		OPER_Flag = 0;					//δ��������״̬
		if(0 == (Count1 % 40)) 	//5����
		{
			Count1 = 1;
			SelfTest_flag = 2;		//����Ϊģʽѡ��״̬
		}
		else
			Count1++;
	}
		else if(2 == SelfTest_flag)
		{
			OPER_Flag = 0;				//δ��������״̬
			if(1 == ManualMode_Flag)
			{
				SelfTest_flag = 4;
			}
			else
			{}
			if(0 == (Count1 % 40)) //5����
			{
				Count1 = 1;
				SelfTest_flag = 3;		//����Ϊ�Զ�ģʽ״̬
			}
			else
				Count1++;
		}
		else if((0 == SelfTest_flag)&&(0 == OPER_Flag)&&(1 == LED))		//����״̬�� δ���� ����ƿ��� 1���ӵ��رձ���
		{
			if(0 == (Count1 % 498))			//1���ӹرձ���  1min--498  ����ʱ��10s--80
			{
				Count1 = 1;
				LED_OFF;
			}
			else
				Count1++;
		}
		else
		{}
}

void Time_Count2(void)
{
	if(1 == Count2_Flag)
	{
		if(0 == (Count2 % 2490))				//5min--2490  ���Ե�ʱ����5s--50
		{
			Count2 = 1;
			Count2_Flag = 0;
			FiveMinute_Flag = 1;	
		}
		else
			Count2++;
	}
	else
	{
// 		Count2 = 1;
	}
}

void Time_Count3(void)						//5���ӵ���ʱ ��ʱ����
{
	if(1 == OPER_Flag)						//����״̬
	{
		if(0 == (Count3 % 2490))				//5min -- 2490 
		{
			Count3 = 1;
			Minute_Flag3 = 1;	
 			TVOC_Ctrl = 0;
 			TVOCSample_Flag = 0;
		}
		else if(249 == Count3 % 498)				//1min -- 498 10s -- 83
		{
			Count3++;
			TVOCSample_Flag = 1;
 			//TVOC_Ctrl = 1;
		}
		else if(0 == Count3 % 498)
		{
			Count3++;
			TVOCSample_Flag = 0;
			TVOC_Ctrl = 0;
		}
			else
			Count3++;
	}
	else
	{}	
}

// void Time_Count3(void)						//5���ӵ���ʱ ��ʱ����
// {
// 	if(1 == OPER_Flag)						//����״̬
// 	{
// // 		if(0 == (Count3 % 2490))				//5min -- 2490 
// // 		{
// // 			Count3 = 1;
// // 			Minute_Flag3 = 1;	
// //  			TVOC_Ctrl = 0;
// // 		}
// // 		else if(80 == Count3 )				//1min -- 489 
// // 		{
// // 			Count3++;
// // 			TVOCSample_Flag = 1;
// //  			TVOC_Ctrl = 1;
// // 		}
// // 		else if(320 == Count3)
// // 		{
// // 			Count3++;
// // 			TVOC_Ctrl = 0;		
// // 		}
// // 		else if(400 == Count3)
// // 		{
// // 			TVOCSample_Flag = 1;
// // 			TVOC_Ctrl = 1;
// // 		}
// // 		
// // 			Count3++;
// // 	}
// // 	else
// // 	{}
// 		if(Count3 >= 2939)
// 			Count3 = 0;
// 			else
// 				Count3++;
// 		switch(Count3)
// 		{
// 			case 0:
// 				Minute_Flag3 = 1;	
// 				TVOC_Ctrl = 0;
// 				break;
// 			case 80:
// 				TVOCSample_Flag = 1;
// 				break;
// 			case 240:
// 				TVOCSample_Flag = 2;
// 				TVOC_Ctrl = 1;
// 				break;
// 			case 489:
// 				TVOC_Ctrl = 0;
// 				break;
// 			case 569:
// 				TVOCSample_Flag = 1;
// 				break;
// 			case 729:
// 				TVOCSample_Flag = 2;
// 				TVOC_Ctrl = 1;
// 				break;
// 			default:
// 				break;	
// 		}
// 	}
// 	else
// 	{}
// }
void Time_Count4(void)						//2Сʱ���رչⴥý����
{
	if((1 == OPER_Flag)&&(0 == ManualMode_Flag))						//����״̬ �Զ�ģʽ
	{
		if((1 == Phst_Flag)&&(1 == Disi_Flag))
		{
			if(0 == (Count4 % 59760))				//2Сʱ��--59760(����֮��)	����ʱ��300s--2490
			{
				Count4 = 1;
				Relay4_OFF;							//�ⴥý��Դ�ر�
				PHST_OFF;							//�رչⴥý
				Phst_Flag = 0;
				Relay5_OFF;							//������Դ�ر�	
				DISI_OFF;							//�ر�����
				Disi_Flag = 0;
			}
			else
				Count4++;
		}
		else
		{
			Count4 = 1;
		}
	}
	else
	{}	
}
void Time_Count5(void)						//��ζ5�����½���λ
{
	if(1 == Count5_Flag)
	{
		if(0 == (Count5 % 2490))				//5min--2490  ���Ե�ʱ����5s--50
		{
			Count5 = 1;
			Count5_Flag = 0;
			FiveMinute5_Flag = 1;	
		}
		else
			Count5++;
	}
	else
	{
// 		Count5 = 1;
	}
	
}

// void Time_Count6(void)						//1���ӵ���ʱ ���ڴ������ɼ�����
// {
// 	if(1 == OPER_Flag)						//����״̬
// 	{
// 		if(0 == (Count6 % 600))				//1min--600
// 		{
// 			Count6 = 1;
// 			Minute_Flag6 = 1;	
// 		}
// 		else
// 			Count6++;
// 	}
// 	else
// 	{}	
// }
/****************end of file***************/
