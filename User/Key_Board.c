#include "Key_Board.h"


/*************variable*************/

unsigned char Key_Wind_State = 0;		//���ٹ���״̬λ 0������ 1������ 2������
unsigned char Key_Phst_State = 0;		//�ⴥý����״̬λ 0���ر� 1������
unsigned char Key_Disi_State = 0;		//��������״̬λ 0���ر� 1������
unsigned char Key_Time_State = 0;		//��ʱ����״̬λ 0���ر� 1��1Сʱ 2��2Сʱ 3��4Сʱ 4��8Сʱ 5��12Сʱ
unsigned char Key_Mode_State = 0;		//ģʽ����״̬λ 0���Զ�ģʽ 1���ֶ�ģʽ
unsigned char Key_Power_State = 0;	//��Դ����״̬λ 0���ر� 1������

unsigned char Wind_High_Flag = 0;		//���ٸ���ģʽ��־λ
unsigned char Wind_Mid_Flag = 0;		//��������ģʽ��־λ
unsigned char Wind_Low_Flag = 0;		//���ٵ���ģʽ��־λ
unsigned char ManualMode_Flag = 0;		//�Զ�ģʽ��־λ 0���Զ�ģʽ 1���ֶ�ģʽ 
unsigned char Disi_Flag = 0;			//��������״̬λ 0���ر� 1������	�����µڶ�������ܺ͵����ӱ�־λ�ĳ�ͻ
unsigned char Phst_Flag = 0;			//�ⴥý�Ĺ���״̬ 0�������� 1������

unsigned char AutoModeDisp_State = 0;		//��¼�Զ�ģʽ�µ��Զ�ȦȦ����ʾ״̬
unsigned char WindDisp_State = 0;		//��¼����ͼ�����ʾ״̬

/*���̳�ʼ��*/
void KeyBoard_Init(void)
{
	 /* Configure P3.3 as input mode */
    GPIO_SetMode(P3, BIT1, GPIO_PMD_INPUT );//FLAG
	/* Configure P0.0 as input mode */
    GPIO_SetMode(P0, BIT1, GPIO_PMD_INPUT );//D2
	/* Configure P0.1 as input mode */
    GPIO_SetMode(P0, BIT2, GPIO_PMD_INPUT );//D1
	/* Configure P0.0 as input mode */
    GPIO_SetMode(P0, BIT3, GPIO_PMD_INPUT );//D0

}
void Relay_Init(void)
{
	/* Configure P2.1 as output mode */
    GPIO_SetMode(P2, BIT1, GPIO_PMD_OUTPUT );//Relay1
	/* Configure P2.1 as output mode */
    GPIO_SetMode(P2, BIT2, GPIO_PMD_OUTPUT );//Relay2
	/* Configure P2.1 as output mode */
    GPIO_SetMode(P2, BIT3, GPIO_PMD_OUTPUT );//Relay3
	/* Configure P2.1 as output mode */
    GPIO_SetMode(P2, BIT4, GPIO_PMD_OUTPUT );//Relay4
	/* Configure P2.1 as output mode */
    GPIO_SetMode(P4, BIT0, GPIO_PMD_OUTPUT );//Relay5
	AllRelay_OFF;						//�ر����м̵���
}

/*ɨ�谴��*/
char KeyBoard_Scan(void)
{		
	if(0 == Key_Flag)
	{
		if((0 == OUT_DA2)&&(0 == OUT_DA1)&&(0 == OUT_DA0))
			return KEY_WIND;																			//���Ȱ�������
		else if((0 == OUT_DA2)&&(0 == OUT_DA1)&&(1 == OUT_DA0))
			return KEY_PHST;																			//�ⴥý��������
		else if((0 == OUT_DA2)&&(1 == OUT_DA1)&&(0 == OUT_DA0))
			return KEY_DISI;																			//������������
		else if((0 == OUT_DA2)&&(1 == OUT_DA1)&&(1 == OUT_DA0))
			return KEY_PWR;																				//��Դ��������
		else if((1 == OUT_DA2)&&(0 == OUT_DA1)&&(0 == OUT_DA0))
			return KEY_MOD;																				//ģʽ��������
		else if((1 == OUT_DA2)&&(0 == OUT_DA1)&&(1 == OUT_DA0))
			return KEY_TMR;																				//��ʱ��������
		else
			return KEY_NULL;
	}
	else
	{
		wait = 0;					//�ް���ʱ�����ʱ����
		return KEY_NULL;
	}
}

/*����0������ 1������ 2������*/
void Set_Wind(void)
{
	if(1 == OPER_Flag)				//ϵͳ�����Լ����������
	{
// 		if(0 == Sleep_Flag)		//����ʱ��û��
// 		{
			if(1 == Key_Mode_State)
			{
				Key_Wind_State++;			//��һ��״̬��1
				if(3 == Key_Wind_State)		//״ֻ̬��0������ 1������ 2������
					Key_Wind_State = 0;
					else
					{}
				switch(Key_Wind_State)
				{
					case 0:
						Oper_Wind_Low();		
						break;
					case 1:
						Oper_Wind_High();
						break;
					case 2:
						Oper_Wind_Mid();
						break;
						default:
							Oper_Wind_Low();
							break;
				}
			}
			else					//����ʱ�䵽 ������
			{}
// 		}
// 		else
// 		{}
	}
	else
	{
		LED_ON;
		ALLKEY_OFF;				//�ر����а���
		KEY_POWERON;			//���ذ�����ʾ
		Symbols_OFF();			//�ر����б�־
		//KEY_MODEON;				//��ʾģʽ����
		CLOSE_ALLRELAY;
		LEDS_OFF();				//�ر����������
	}
			//wind_flag = 1;
}
/*�ⴥý0���ر� 1������*/
void Set_Phst(void)
{
	if(1 == OPER_Flag)				//ϵͳ�����Լ����������
	{
// 		if(0 == Sleep_Flag)		//����ʱ��û��
// 		{
			if(1== Key_Mode_State)
			{
				Key_Phst_State++;
				if(2 == Key_Phst_State)	//״ֻ̬��0���ر� 1������
					Key_Phst_State = 0;
					else
					{}
				switch(Key_Phst_State)
				{
					case 0:
						Oper_Phst_OFF();
						break;
					case 1:
						Oper_Phst_ON();
						break;
					default:
						break;
				}
			}
			else
			{}
// 		}
// 		else
// 		{}
	}
	else
	{
		LED_ON;
		ALLKEY_OFF;				//�ر����а���
		KEY_POWERON;			//���ذ�����ʾ
		Symbols_OFF();			//�ر����б�־
		//KEY_MODEON;				//��ʾģʽ����
		LEDS_OFF();				//�ر����������
		CLOSE_ALLRELAY;
	}
		//wind_flag = 1;
}
/*����0���ر� 1������*/
void Set_Disi(void)
{
	if(1 == OPER_Flag)				//ϵͳ�����Լ����������
	{
// 		if(0 == Sleep_Flag)		//����ʱ��û��
// 		{
			if(1 == Key_Mode_State)
			{
				Key_Disi_State++;
				if(2 == Key_Disi_State)//״ֻ̬��0���ر� 1������
					Key_Disi_State = 0;
				switch(Key_Disi_State)
				{
					case 0:
						Oper_Disi_OFF();
						break;
					case 1:
						Oper_Disi_ON();
						break;
						default:
							break;
				}
			}
			else
			{}
// 		}
// 		else
// 		{}
	}
	else
	{
		LED_ON;
		ALLKEY_OFF;				//�ر����а���
		KEY_POWERON;			//���ذ�����ʾ
		Symbols_OFF();			//�ر����б�־
		//KEY_MODEON;				//��ʾģʽ����
		LEDS_OFF();				//�ر����������
		CLOSE_ALLRELAY;
	}
}
/*��ʱ0���ر� 1��1Сʱ 2��2Сʱ 3��4Сʱ 4��8Сʱ 5��12Сʱ*/
void Set_Time(void)
{
	if(1 == OPER_Flag)				//ϵͳ�����Լ����������
	{
// 		if(0 == Sleep_Flag)		//����ʱ��û��
// 		{
			if(0 == ManualMode_Flag)
			{
				Key_Time_State++;
				if(6 == Key_Time_State)
					Key_Time_State = 1;
					else
					{}
				Count2 = 1;					//PM2.5 5����״̬�仯����������1
				Count3 = 0;					//5���Ӳ�����ʱ��������
				Count4 = 1;					//�ⴥý������2Сʱ�رռ�ʱ������1
				Count5 = 1;					//TVOC 5����״̬�仯����������1
				switch(Key_Time_State)
				{
					case 0:
						Oper_Time_0H();
						break;
					case 1:
						Oper_Time_1H();
						break;
					case 2:
						Oper_Time_2H();
						break;
					case 3:
						Oper_Time_4H();
						break;
					case 4:
						Oper_Time_8H();
						break;
					case 5:
						Oper_Time_12H();
						break;
						default:
							Oper_Time_1H();
							break;
				}
			}
			else
			{}
// 		}
// 		else
// 		{}
	}
	else
	{
		LED_ON;
		ALLKEY_OFF;				//�ر����а���
		KEY_POWERON;			//���ذ�����ʾ
		Symbols_OFF();			//�ر����б�־
		//KEY_MODEON;				//��ʾģʽ����
		LEDS_OFF();				//�ر����������
		CLOSE_ALLRELAY;
	}
}
/*ģʽѡ��0���Զ�ģʽ 1���ֶ�ģʽ*/
void Set_Mode(void)
{
	if((1 == OPER_Flag) || (2 == SelfTest_flag))			//ϵͳ�����Լ����������
	{
// 		if(0 == Sleep_Flag)		//����ʱ��û��
// 		{
			Key_Mode_State++;
			if(2 == Key_Mode_State)	//״ֻ̬��0���Զ�ģʽ 1���ֶ�ģʽ
				Key_Mode_State = 0;
				else
				{}
			switch(Key_Mode_State)
			{
				case 0:
					Oper_Mode_Auto();
					break;
				case 1:
					Oper_Mode_Manual();
					break;
					default:	
						break;
			}
// 		}
// 		else
// 		{}
	}
	else
	{
		LED_ON;
		ALLKEY_OFF;				//�ر����а���
		KEY_POWERON;			//���ذ�����ʾ
		Symbols_OFF();			//�ر����б�־
		//KEY_MODEON;				//��ʾģʽ����
		LEDS_OFF();				//�ر����������
		CLOSE_ALLRELAY;
	}
}
/*����0���ر� 1������*/
void Set_Power(void)
{
// 	if(0 == Sleep_Flag)		//����ʱ��û��
// 	{
		Key_Power_State++;
		if(2 == Key_Power_State)//״ֻ̬��0���ر� 1����
			Key_Power_State = 0;
			else
			{}
		switch(Key_Power_State)
		{
			case 0:
				Oper_Power_OFF();
				break;
			case 1:
				Oper_Power_ON();
				break;
				default:
					break;
		}
// 	}
// 	else
// 	{}
}
/*���ָ���ģʽ*/
void Oper_Wind_High(void)
{
	Relay1_OFF;							//����͵���Դ�ر�
	Relay2_OFF;							//����е���Դ�ر�
	Relay3_ON;							//����ߵ���Դ����
	Wind_High_Flag = 1;
	Wind_Mid_Flag = 0;
	Wind_Low_Flag = 0;
}
/*��������ģʽ*/
void Oper_Wind_Mid(void)
{
	Relay1_OFF;							//����͵���Դ�ر�
	Relay2_ON;							//����е���Դ����
	Relay3_OFF;							//����ߵ���Դ�ر�
	Wind_High_Flag = 0;
	Wind_Mid_Flag = 1;
	Wind_Low_Flag = 0;
}
/*���ֵ���ģʽ*/
void Oper_Wind_Low(void)
{
	Relay1_ON;							//����͵���Դ����
	Relay2_OFF;							//����е���Դ�ر�
	Relay3_OFF;							//����ߵ���Դ�ر�
	Wind_High_Flag = 0;
	Wind_Mid_Flag = 0;
	Wind_Low_Flag = 1;
}

/*�ⴥý����*/
void Oper_Phst_ON(void)
{
	Phst_Flag = 1;
	Relay4_ON;							//�ⴥý��Դ����
	PHST_ON;								//�ⴥýͼ����ʾ
}
/*�ⴥý�ر�*/
void Oper_Phst_OFF(void)
{
	Phst_Flag = 0;
	Relay4_OFF;							//�ⴥý��Դ�ر�
	PHST_OFF;								//�ⴥýͼ��ر�
}
/*��������*/
void Oper_Disi_ON(void)
{
	Relay5_ON;							//������Դ����
	Disi_Flag = 1;
	DISI_ON;								//����ͼ����ʾ
}
/*�����ر�*/
void Oper_Disi_OFF(void)
{
	Relay5_OFF;							//������Դ�ر�		
	Disi_Flag = 0;
	DISI_OFF;								//����ͼ��ر�
}
/*δ���õ���ʱ*/
void Oper_Time_0H(void)
{	
	CountDown_Flag = 0;
}
/*����ʱ 1H*/
void Oper_Time_1H(void)
{
	Count_Down = ONE_HOUR;	//����ʱ����ʱ��Ϊ1Сʱ
	Count0 = 1;							//����ʱ������������
	CountDown_Flag = 1;			//����ʱ��־λ��1  ��1Ϊ����״̬
}
/*����ʱ 2H*/
void Oper_Time_2H(void)
{
	Count_Down = TWO_HOUR;//����ʱ����ʱ��Ϊ2Сʱ
	Count0 = 1;							//����ʱ������������
	CountDown_Flag = 1;			//����ʱ��־λ��1  ��1Ϊ����״̬
}
/*����ʱ 4H*/
void Oper_Time_4H(void)
{
	Count_Down = FOUR_HOUR;//����ʱ����ʱ��Ϊ4Сʱ
	CountDown_Flag = 1;			//����ʱ��־λ��1  ��1Ϊ����״̬
	Count0 = 1;							//����ʱ������������
}
/*����ʱ 8H*/
void Oper_Time_8H(void)
{
	Count_Down = EIGHT_HOUR;//����ʱ����ʱ��Ϊ8Сʱ
	Count0 = 1;							//����ʱ������������
	CountDown_Flag = 1;			//����ʱ��־λ��1  ��1Ϊ����״̬
}
/*����ʱ 12*/
void Oper_Time_12H(void)
{
	Count_Down = TWELVE_HOUR;//����ʱ����ʱ��Ϊ12Сʱ
	Count0 = 1;							//����ʱ������������
	CountDown_Flag = 1;			//����ʱ��־λ��1  ��1Ϊ����״̬
}
/*�ֶ�ģʽ*/
void Oper_Mode_Manual(void)
{
	ManualMode_Flag = 1;
	Oper_Wind_Low();			//���ٵ�������
	Key_Wind_State = 0;		//���ٹ���״̬λ 0������ 1������ 2������
	Oper_Phst_OFF();			//�ⴥý�ر�
	Key_Phst_State = 0;		//��¼�ⴥý����״̬λ����
	Oper_Disi_OFF();			//�����ر�
	Key_Disi_State = 0;		//��¼��������״̬λ����
	TEMP_ON;
	HUMI_ON;
	MANUAL_ON;
	Count2 = 1;	
	Count3 = 0;	
	Count5 = 1;
	CountDown_Flag	= 0;
	TIME_OFF;
	LED1_OFF;LED2_OFF;LED3_OFF;LED4_OFF; //�رյ���ʱ�������ʾ
// 	/*����ʱ����Ϊ12Сʱ*/
// 	Key_Time_State = 5;		
// 	Oper_Time_12H();					
	
	PM_ON;
	TVOC_ON;
}
/*�Զ�ģʽ*/
void Oper_Mode_Auto(void)
{
	ManualMode_Flag = 0;
	Oper_Wind_Low();
	Oper_Phst_ON();			//�ⴥý����
	Key_Phst_State = 1;		//��¼�ⴥý����״̬λ��1
	Oper_Disi_ON();			//��������
	Key_Disi_State = 1;		//��¼��������״̬λ��1
	Count2 = 1;
	Count3 = 0;	
	Count4 = 1;
	Count5 = 1;
	//Count4 = 1;				//��¼�ⴥý��������״̬��ʱ������1
	PHST_ON;							
	DISI_ON;
	TEMP_ON;
	HUMI_ON;
	AUTO_ON;
	PM_ON;
	TVOC_ON;
	/*����ʱ����Ϊ1Сʱ*/
	Key_Time_State = 1;		
	Oper_Time_1H();				

}
/*���ش�*/
void Oper_Power_ON(void)
{
	SelfTest_flag = 1;	//����������Լ�״̬
	Count1 = 1;					//�����Լ��ʱ������1
}
/*���عر�*/
void Oper_Power_OFF(void)
{
	LED_OFF;						//�رձ���
	CLOSE_ALLRELAY;			//�ر����е�Դ
	TVOC_Ctrl = 1;			//��ζ�ر�
	PMLEDPOWER_OFF;			//pm2.5�ر�
	LCD_WrAllData();		//����
	AllFlag_Clr();			//�������а���״̬��־λ
	LEDS_OFF();					//�ر����������
}
void SelfTest(void)
{
	if(1 == SelfTest_flag)
	{
		LED_ON;								//��ʾ����
		ALLKEY_ON;						//��ʾ���а�����־
		Symbols_ON();					//��ʾ���б�־
		LEDS_ON();						//��ʾ���������
	}
	else if(2 == SelfTest_flag)
	{
		ALLKEY_OFF;				//�ر����а���
		Symbols_OFF();			//�ر����б�־
		KEY_MODEON;				//��ʾģʽ����
		LEDS_OFF();				//�ر����������
	}
	else if(3 == SelfTest_flag)
	{
		OPER_Flag = 1;			//ϵͳ��������״̬
 		Minute_Flag3 = 1;		//��ʪ��������ʾ  PM2.5������ʾ
 		TVOCSample_Flag = 0;	//��ʾ��ζ����
		Count0 = 1;
		Count2 = 1;
		Count3 = 0;	
		Count4 = 1;
		Count5 = 1;
		PM_State = 1;
		TVOC_State = 1;
		FiveMinute_Flag = 0; //5���Ӽ�ʱ����	
		FiveMinute5_Flag = 0;		
		ALLKEY_ON;				//��ʾ���а���
		Oper_Mode_Auto();		//�����Զ�ģʽ
		SelfTest_flag = 0;
	}
	else if(4 == SelfTest_flag)
	{
		OPER_Flag = 1;			//ϵͳ��������״̬
		Minute_Flag3 = 1;		//��ʪ��������ʾ  PM2.5������ʾ
		Count0 = 1;
		Count2 = 1;
		Count3 = 0;	
		Count4 = 1;	
		Count5 = 1;
		PM_State = 1;
		TVOC_State = 1;
		TVOCSample_Flag = 0;	//��ʾ��ζ����
		FiveMinute_Flag = 0; 	//5���Ӽ�ʱ����
		FiveMinute5_Flag = 0;
		ALLKEY_ON;				//��ʾ���а���
		Oper_Mode_Manual(); 	//�����ֶ�ģʽ
		SelfTest_flag = 0;
	}
	else 
	{}
}

void Wind_Disp(void)
{
	if(1 == OPER_Flag)
	{
		if((1 == Wind_High_Flag) || (1 == Wind_Mid_Flag) || (1 == Wind_Low_Flag))			//�з��ٰ�������
		{
			if(WindDisp_State >= 5)				//������״̬ ÿ��״̬100ms
					WindDisp_State = 0;
					else
						WindDisp_State++;
			if((1 == Wind_High_Flag)&&(0 == Wind_Mid_Flag)&&(0 == Wind_Low_Flag))			//���ٰ�������
			{
				switch(WindDisp_State)
				{
					case 0:
						FAN1;CLR_WIND;FANPOST1;		//��ʾ��һƬ���ֺ͹���
						break;
					case 1:
						FAN2;						//��ʾ�ڶ�Ƭ����
						break;
					case 2:
						FAN3;FANPOST2;				//��ʾ����Ƭ���ֺ͵ڶ�������
						break;
					case 3:
						FAN1;						//��ʾ��һƬ����
						break;
					case 4:
						FAN2;FANPOST3;				//��ʾ�ڶ�Ƭ���ֺ͵���������
						break;
					case 5:
						FAN3;						//��ʾ����Ƭ����
						break;	
					default:
						break;
				}
			}
			else if((0 == Wind_High_Flag)&&(1 == Wind_Mid_Flag)&&(0 == Wind_Low_Flag))	//���ٰ�������
			{
				switch(WindDisp_State)
				{
					case 0:
						FAN1;CLR_WIND;FANPOST1;		//��ʾ��һƬ���ֺ͹���
						break;
					case 1:
						FAN1;						//��ʾ��һƬ����
						break;
					case 2:
						FAN2;						//��ʾ�ڶ�Ƭ����
						break;
					case 3:
						FAN2;FANPOST2;				//��ʾ�ڶ�Ƭ���ֺ͵ڶ�������
						break;
					case 4:
						FAN3;						//��ʾ����Ƭ����			
						break;
					case 5:
						FAN3;						//��ʾ����Ƭ����	
						break;
					default:
						break;
				}		
			}
			else if((0 == Wind_High_Flag)&&(0 == Wind_Mid_Flag)&&(1 == Wind_Low_Flag))	//���ٰ�������
			{
				switch(WindDisp_State)
				{
					case 0:
						FAN1;FANPOST1;				//��ʾ��һƬ���ֺ͹���	
						break;
					case 1:
						FAN1;						//��ʾ��һƬ����	
						break;
					case 2:
						FAN2;						//��ʾ�ڶ�Ƭ����	
						break;
					case 3:
						FAN2;						//��ʾ�ڶ�Ƭ����	
						break;
					case 4:
						FAN3;						//��ʾ����Ƭ����	
						break;
					case 5:
						FAN3;						//��ʾ����Ƭ����	
						break;
					default:
						break;
				}
			}
			else
			{}
		}
		else
		{}
	}
	else
	{}
}


void AutoMode_Disp(void)				//��̬��ʾ����ͼ��
{
	if(1 == OPER_Flag)
	{
		if(0 == ManualMode_Flag )
		{
			if(AutoModeDisp_State >= 3)
				AutoModeDisp_State = 0;
				else
					AutoModeDisp_State++;
			if(0 == AutoModeDisp_State)
			{
				AutoMode123;
			}
				else if(1 == AutoModeDisp_State)
				{	
					AutoMode234;
				}
				else if(2 == AutoModeDisp_State)
				{
					AutoMode341;
				}
				else if(3 == AutoModeDisp_State)
				{
					AutoMode412;
				}
				else
				{
					AUTO_ON;
				}
		}
		else if(1 == ManualMode_Flag)
		{
			MANUAL_ON;									//�ֶ�ģʽ��־��ʾ
		}
		else
		{}
	}
	else
	{
		MODE_OFF;										//�ر�ģʽ��ʾ��־
	}
}
void AllFlag_Clr(void)				//�������а���״̬��־λ ��Դ����
{
	Key_Wind_State = 0;						//���ٹ���״̬λ 0������ 1������ 2������
	Key_Phst_State = 0;						//�ⴥý����״̬λ 0���ر� 1������
	Key_Disi_State = 0;						//��������״̬λ 0���ر� 1������
	Key_Time_State = 0;						//��ʱ����״̬λ 0���ر� 1��1Сʱ 2��2Сʱ 3��4Сʱ 4��8Сʱ 5��12Сʱ
	Key_Mode_State = 0;						//ģʽ����״̬λ 0���Զ�ģʽ 1���ֶ�ģʽ
	Key_Power_State = 0;
	ManualMode_Flag = 0;					//�Զ��ֶ�������־λ����
	OPER_Flag = 0;								//ϵͳ�ر�  ��̬��ʾ���
	Disi_Flag = 0;								//��������״̬λ 0���ر� 1������	�����µڶ�������ܺ͵����ӱ�־λ�ĳ�ͻ
	Wind_High_Flag = 0;						//���ٸ���ģʽ��־λ
	Wind_Mid_Flag = 0;						//��������ģʽ��־λ
	Wind_Low_Flag = 0;						//���ٵ���ģʽ��־λ
	SelfTest_flag = 0;    				//�Լ��־λ 0��Ϊ����״̬1��Ϊ�Լ�ģʽ 2��Ϊ�ֶ��Զ�ģʽѡ��	
	CountDown_Flag = 0;						//����ʱ������־λ
	Count2_Flag = 0;							//PM2.5 5����״̬��ʱ��־λ����
	FiveMinute_Flag = 0;					//PM2.5 5����״̬��ʱʱ�䵽���־λ����
	Count5_Flag = 0;							//TVOC 5����״̬��ʱ��־λ����
	FiveMinute5_Flag = 0;					//TVOC 5����״̬��ʱʱ�䵽���־λ����
}
void LEDS_OFF(void)							//�ر����������
{
	LED1_OFF;											//�ر������1
	LED2_OFF;											//�ر������2
	LED3_OFF;											//�ر������3
	LED4_OFF;											//�ر������4
	LED5_OFF;											//�ر������5
	LED6_OFF;											//�ر������6
	LED7_OFF;											//�ر������7
	LED8_OFF;											//�ر������8
	LED9_OFF;											//�ر������9
	LED10_OFF;											//�ر������10
}
void LEDS_ON(void)							//��ʾ���������
{
	LED1_ON;											//�������1
	LED2_ON;											//�������2
	LED3_ON;											//�������3
	LED4_ON;											//�������4
	LED5_ON;											//�������5
	LED6_ON;											//�������6
	LED7_ON;											//�������7
	LED8_ON;											//�������8
	LED9_ON;											//�������9
	LED10_ON;											//�������10
}
void Symbols_ON(void)					//��ʾ���б�־
{
	WIND_ON;						//��ʾ���ٱ�־
	PHST_ON;						//��ʾ�ⴥý��־
	DISI_ON;						//��ʾ������־
	MODE_ON;						//��ʾģʽ��־
	TIME_ON;						//��ʾ����ʱ��־
	TEMP_ON;						//��ʾ�¶ȱ�־
	HUMI_ON;						//��ʾʪ�ȱ�־
	PM_ON;							//��ʾPM2.5
	PMPOST_ON;						//��ʾPM2.5����
	TVOC_ON;						//��ʾTVOC��־
	TVOCPOST_ON;					//��ʾTVOC���й���
}
void Symbols_OFF(void)					//��ʾ���б�־
{
	WIND_OFF;					//�رշ��ٱ�־
	PHST_OFF;					//�رչⴥý��־
	DISI_OFF;					//�ر�������־
	MODE_OFF;					//�ر�ģʽ��־
	TIME_OFF;					//�رն�ʱ��־
	TEMP_OFF;					//�ر��¶ȱ�־
	HUMI_OFF;					//�ر�ʪ�ȱ�־
	PM_OFF;						//�ر�PM2.5
	PMPOST_OFF;					//�ر�PM2.5����
	TVOC_OFF;					//�ر�TVOC��־
	TVOCPOST_OFF;				//�ر�TVOC����
}
/*end of file*/
