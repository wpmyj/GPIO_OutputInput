#include "Key_Board.h"


/*************variable*************/

unsigned char Key_Wind_State = 0;		//风速功能状态位 0、低速 1、高速 2、低速
unsigned char Key_Phst_State = 0;		//光触媒功能状态位 0、关闭 1、工作
unsigned char Key_Disi_State = 0;		//消毒功能状态位 0、关闭 1、工作
unsigned char Key_Time_State = 0;		//定时功能状态位 0、关闭 1、1小时 2、2小时 3、4小时 4、8小时 5、12小时
unsigned char Key_Mode_State = 0;		//模式功能状态位 0、自动模式 1、手动模式
unsigned char Key_Power_State = 0;	//电源功能状态位 0、关闭 1、开启

unsigned char Wind_High_Flag = 0;		//风速高速模式标志位
unsigned char Wind_Mid_Flag = 0;		//风速中速模式标志位
unsigned char Wind_Low_Flag = 0;		//风速低速模式标志位
unsigned char ManualMode_Flag = 0;		//自动模式标志位 0、自动模式 1、手动模式 
unsigned char Disi_Flag = 0;			//消毒工作状态位 0、关闭 1、工作	用于温第二个数码管和等离子标志位的冲突
unsigned char Phst_Flag = 0;			//光触媒的工作状态 0、不工作 1、工作

unsigned char AutoModeDisp_State = 0;		//记录自动模式下的自动圈圈的显示状态
unsigned char WindDisp_State = 0;		//记录风速图标的显示状态

/*键盘初始化*/
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
	AllRelay_OFF;						//关闭所有继电器
}

/*扫描按键*/
char KeyBoard_Scan(void)
{		
	if(0 == Key_Flag)
	{
		if((0 == OUT_DA2)&&(0 == OUT_DA1)&&(0 == OUT_DA0))
			return KEY_WIND;																			//风扇按键按下
		else if((0 == OUT_DA2)&&(0 == OUT_DA1)&&(1 == OUT_DA0))
			return KEY_PHST;																			//光触媒按键按下
		else if((0 == OUT_DA2)&&(1 == OUT_DA1)&&(0 == OUT_DA0))
			return KEY_DISI;																			//消毒按键按下
		else if((0 == OUT_DA2)&&(1 == OUT_DA1)&&(1 == OUT_DA0))
			return KEY_PWR;																				//电源按键按下
		else if((1 == OUT_DA2)&&(0 == OUT_DA1)&&(0 == OUT_DA0))
			return KEY_MOD;																				//模式按键按下
		else if((1 == OUT_DA2)&&(0 == OUT_DA1)&&(1 == OUT_DA0))
			return KEY_TMR;																				//定时按键按下
		else
			return KEY_NULL;
	}
	else
	{
		wait = 0;					//无按键时清零计时变量
		return KEY_NULL;
	}
}

/*风速0、低速 1、高速 2、低速*/
void Set_Wind(void)
{
	if(1 == OPER_Flag)				//系统经过自检后正常运行
	{
// 		if(0 == Sleep_Flag)		//待机时间没到
// 		{
			if(1 == Key_Mode_State)
			{
				Key_Wind_State++;			//按一下状态加1
				if(3 == Key_Wind_State)		//状态只有0、低速 1、高速 2、低速
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
			else					//待机时间到 不操作
			{}
// 		}
// 		else
// 		{}
	}
	else
	{
		LED_ON;
		ALLKEY_OFF;				//关闭所有按键
		KEY_POWERON;			//开关按键显示
		Symbols_OFF();			//关闭所有标志
		//KEY_MODEON;				//显示模式按键
		CLOSE_ALLRELAY;
		LEDS_OFF();				//关闭所有数码管
	}
			//wind_flag = 1;
}
/*光触媒0、关闭 1、工作*/
void Set_Phst(void)
{
	if(1 == OPER_Flag)				//系统经过自检后正常运行
	{
// 		if(0 == Sleep_Flag)		//待机时间没到
// 		{
			if(1== Key_Mode_State)
			{
				Key_Phst_State++;
				if(2 == Key_Phst_State)	//状态只有0、关闭 1、工作
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
		ALLKEY_OFF;				//关闭所有按键
		KEY_POWERON;			//开关按键显示
		Symbols_OFF();			//关闭所有标志
		//KEY_MODEON;				//显示模式按键
		LEDS_OFF();				//关闭所有数码管
		CLOSE_ALLRELAY;
	}
		//wind_flag = 1;
}
/*消毒0、关闭 1、工作*/
void Set_Disi(void)
{
	if(1 == OPER_Flag)				//系统经过自检后正常运行
	{
// 		if(0 == Sleep_Flag)		//待机时间没到
// 		{
			if(1 == Key_Mode_State)
			{
				Key_Disi_State++;
				if(2 == Key_Disi_State)//状态只有0、关闭 1、工作
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
		ALLKEY_OFF;				//关闭所有按键
		KEY_POWERON;			//开关按键显示
		Symbols_OFF();			//关闭所有标志
		//KEY_MODEON;				//显示模式按键
		LEDS_OFF();				//关闭所有数码管
		CLOSE_ALLRELAY;
	}
}
/*定时0、关闭 1、1小时 2、2小时 3、4小时 4、8小时 5、12小时*/
void Set_Time(void)
{
	if(1 == OPER_Flag)				//系统经过自检后正常运行
	{
// 		if(0 == Sleep_Flag)		//待机时间没到
// 		{
			if(0 == ManualMode_Flag)
			{
				Key_Time_State++;
				if(6 == Key_Time_State)
					Key_Time_State = 1;
					else
					{}
				Count2 = 1;					//PM2.5 5分钟状态变化计数变量置1
				Count3 = 0;					//5分钟采样计时变量清零
				Count4 = 1;					//光触媒、消毒2小时关闭计时变量置1
				Count5 = 1;					//TVOC 5分钟状态变化计数变量置1
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
		ALLKEY_OFF;				//关闭所有按键
		KEY_POWERON;			//开关按键显示
		Symbols_OFF();			//关闭所有标志
		//KEY_MODEON;				//显示模式按键
		LEDS_OFF();				//关闭所有数码管
		CLOSE_ALLRELAY;
	}
}
/*模式选择0、自动模式 1、手动模式*/
void Set_Mode(void)
{
	if((1 == OPER_Flag) || (2 == SelfTest_flag))			//系统经过自检后正常运行
	{
// 		if(0 == Sleep_Flag)		//待机时间没到
// 		{
			Key_Mode_State++;
			if(2 == Key_Mode_State)	//状态只有0、自动模式 1、手动模式
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
		ALLKEY_OFF;				//关闭所有按键
		KEY_POWERON;			//开关按键显示
		Symbols_OFF();			//关闭所有标志
		//KEY_MODEON;				//显示模式按键
		LEDS_OFF();				//关闭所有数码管
		CLOSE_ALLRELAY;
	}
}
/*开关0、关闭 1、开启*/
void Set_Power(void)
{
// 	if(0 == Sleep_Flag)		//待机时间没到
// 	{
		Key_Power_State++;
		if(2 == Key_Power_State)//状态只有0、关闭 1、打开
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
/*风轮高速模式*/
void Oper_Wind_High(void)
{
	Relay1_OFF;							//电机低档电源关闭
	Relay2_OFF;							//电机中档电源关闭
	Relay3_ON;							//电机高档电源开启
	Wind_High_Flag = 1;
	Wind_Mid_Flag = 0;
	Wind_Low_Flag = 0;
}
/*风轮中速模式*/
void Oper_Wind_Mid(void)
{
	Relay1_OFF;							//电机低档电源关闭
	Relay2_ON;							//电机中档电源开启
	Relay3_OFF;							//电机高档电源关闭
	Wind_High_Flag = 0;
	Wind_Mid_Flag = 1;
	Wind_Low_Flag = 0;
}
/*风轮低速模式*/
void Oper_Wind_Low(void)
{
	Relay1_ON;							//电机低档电源开启
	Relay2_OFF;							//电机中档电源关闭
	Relay3_OFF;							//电机高档电源关闭
	Wind_High_Flag = 0;
	Wind_Mid_Flag = 0;
	Wind_Low_Flag = 1;
}

/*光触媒开启*/
void Oper_Phst_ON(void)
{
	Phst_Flag = 1;
	Relay4_ON;							//光触媒电源开启
	PHST_ON;								//光触媒图标显示
}
/*光触媒关闭*/
void Oper_Phst_OFF(void)
{
	Phst_Flag = 0;
	Relay4_OFF;							//光触媒电源关闭
	PHST_OFF;								//光触媒图标关闭
}
/*消毒开启*/
void Oper_Disi_ON(void)
{
	Relay5_ON;							//消毒电源开启
	Disi_Flag = 1;
	DISI_ON;								//消毒图标显示
}
/*消毒关闭*/
void Oper_Disi_OFF(void)
{
	Relay5_OFF;							//消毒电源关闭		
	Disi_Flag = 0;
	DISI_OFF;								//消毒图标关闭
}
/*未设置倒计时*/
void Oper_Time_0H(void)
{	
	CountDown_Flag = 0;
}
/*倒计时 1H*/
void Oper_Time_1H(void)
{
	Count_Down = ONE_HOUR;	//倒计时设置时间为1小时
	Count0 = 1;							//倒计时计数变量清零
	CountDown_Flag = 1;			//倒计时标志位置1  置1为工作状态
}
/*倒计时 2H*/
void Oper_Time_2H(void)
{
	Count_Down = TWO_HOUR;//倒计时设置时间为2小时
	Count0 = 1;							//倒计时计数变量清零
	CountDown_Flag = 1;			//倒计时标志位置1  置1为工作状态
}
/*倒计时 4H*/
void Oper_Time_4H(void)
{
	Count_Down = FOUR_HOUR;//倒计时设置时间为4小时
	CountDown_Flag = 1;			//倒计时标志位置1  置1为工作状态
	Count0 = 1;							//倒计时计数变量清零
}
/*倒计时 8H*/
void Oper_Time_8H(void)
{
	Count_Down = EIGHT_HOUR;//倒计时设置时间为8小时
	Count0 = 1;							//倒计时计数变量清零
	CountDown_Flag = 1;			//倒计时标志位置1  置1为工作状态
}
/*倒计时 12*/
void Oper_Time_12H(void)
{
	Count_Down = TWELVE_HOUR;//倒计时设置时间为12小时
	Count0 = 1;							//倒计时计数变量清零
	CountDown_Flag = 1;			//倒计时标志位置1  置1为工作状态
}
/*手动模式*/
void Oper_Mode_Manual(void)
{
	ManualMode_Flag = 1;
	Oper_Wind_Low();			//风速低速运行
	Key_Wind_State = 0;		//风速功能状态位 0、低速 1、高速 2、低速
	Oper_Phst_OFF();			//光触媒关闭
	Key_Phst_State = 0;		//记录光触媒按键状态位清零
	Oper_Disi_OFF();			//消毒关闭
	Key_Disi_State = 0;		//记录消毒按键状态位清零
	TEMP_ON;
	HUMI_ON;
	MANUAL_ON;
	Count2 = 1;	
	Count3 = 0;	
	Count5 = 1;
	CountDown_Flag	= 0;
	TIME_OFF;
	LED1_OFF;LED2_OFF;LED3_OFF;LED4_OFF; //关闭倒计时数码管显示
// 	/*倒计时设置为12小时*/
// 	Key_Time_State = 5;		
// 	Oper_Time_12H();					
	
	PM_ON;
	TVOC_ON;
}
/*自动模式*/
void Oper_Mode_Auto(void)
{
	ManualMode_Flag = 0;
	Oper_Wind_Low();
	Oper_Phst_ON();			//光触媒开启
	Key_Phst_State = 1;		//记录光触媒按键状态位置1
	Oper_Disi_ON();			//消毒开启
	Key_Disi_State = 1;		//记录消毒按键状态位置1
	Count2 = 1;
	Count3 = 0;	
	Count4 = 1;
	Count5 = 1;
	//Count4 = 1;				//记录光触媒和消毒的状态计时变量置1
	PHST_ON;							
	DISI_ON;
	TEMP_ON;
	HUMI_ON;
	AUTO_ON;
	PM_ON;
	TVOC_ON;
	/*倒计时设置为1小时*/
	Key_Time_State = 1;		
	Oper_Time_1H();				

}
/*开关打开*/
void Oper_Power_ON(void)
{
	SelfTest_flag = 1;	//开机后进入自检状态
	Count1 = 1;					//开机自检计时变量置1
}
/*开关关闭*/
void Oper_Power_OFF(void)
{
	LED_OFF;						//关闭背光
	CLOSE_ALLRELAY;			//关闭所有电源
	TVOC_Ctrl = 1;			//异味关闭
	PMLEDPOWER_OFF;			//pm2.5关闭
	LCD_WrAllData();		//清屏
	AllFlag_Clr();			//清零所有按键状态标志位
	LEDS_OFF();					//关闭所有数码管
}
void SelfTest(void)
{
	if(1 == SelfTest_flag)
	{
		LED_ON;								//显示背光
		ALLKEY_ON;						//显示所有按键标志
		Symbols_ON();					//显示所有标志
		LEDS_ON();						//显示所有数码管
	}
	else if(2 == SelfTest_flag)
	{
		ALLKEY_OFF;				//关闭所有按键
		Symbols_OFF();			//关闭所有标志
		KEY_MODEON;				//显示模式按键
		LEDS_OFF();				//关闭所有数码管
	}
	else if(3 == SelfTest_flag)
	{
		OPER_Flag = 1;			//系统进入运行状态
 		Minute_Flag3 = 1;		//温湿度数据显示  PM2.5数据显示
 		TVOCSample_Flag = 0;	//显示异味数据
		Count0 = 1;
		Count2 = 1;
		Count3 = 0;	
		Count4 = 1;
		Count5 = 1;
		PM_State = 1;
		TVOC_State = 1;
		FiveMinute_Flag = 0; //5分钟计时到达	
		FiveMinute5_Flag = 0;		
		ALLKEY_ON;				//显示所有按键
		Oper_Mode_Auto();		//运行自动模式
		SelfTest_flag = 0;
	}
	else if(4 == SelfTest_flag)
	{
		OPER_Flag = 1;			//系统进入运行状态
		Minute_Flag3 = 1;		//温湿度数据显示  PM2.5数据显示
		Count0 = 1;
		Count2 = 1;
		Count3 = 0;	
		Count4 = 1;	
		Count5 = 1;
		PM_State = 1;
		TVOC_State = 1;
		TVOCSample_Flag = 0;	//显示异味数据
		FiveMinute_Flag = 0; 	//5分钟计时到达
		FiveMinute5_Flag = 0;
		ALLKEY_ON;				//显示所有按键
		Oper_Mode_Manual(); 	//运行手动模式
		SelfTest_flag = 0;
	}
	else 
	{}
}

void Wind_Disp(void)
{
	if(1 == OPER_Flag)
	{
		if((1 == Wind_High_Flag) || (1 == Wind_Mid_Flag) || (1 == Wind_Low_Flag))			//有风速按键按下
		{
			if(WindDisp_State >= 5)				//有六种状态 每种状态100ms
					WindDisp_State = 0;
					else
						WindDisp_State++;
			if((1 == Wind_High_Flag)&&(0 == Wind_Mid_Flag)&&(0 == Wind_Low_Flag))			//高速按键按下
			{
				switch(WindDisp_State)
				{
					case 0:
						FAN1;CLR_WIND;FANPOST1;		//显示第一片风轮和光柱
						break;
					case 1:
						FAN2;						//显示第二片风轮
						break;
					case 2:
						FAN3;FANPOST2;				//显示第三片风轮和第二个光柱
						break;
					case 3:
						FAN1;						//显示第一片风轮
						break;
					case 4:
						FAN2;FANPOST3;				//显示第二片风轮和第三个光柱
						break;
					case 5:
						FAN3;						//显示第三片风轮
						break;	
					default:
						break;
				}
			}
			else if((0 == Wind_High_Flag)&&(1 == Wind_Mid_Flag)&&(0 == Wind_Low_Flag))	//中速按键按下
			{
				switch(WindDisp_State)
				{
					case 0:
						FAN1;CLR_WIND;FANPOST1;		//显示第一片风轮和光柱
						break;
					case 1:
						FAN1;						//显示第一片风轮
						break;
					case 2:
						FAN2;						//显示第二片风轮
						break;
					case 3:
						FAN2;FANPOST2;				//显示第二片风轮和第二个光柱
						break;
					case 4:
						FAN3;						//显示第三片风轮			
						break;
					case 5:
						FAN3;						//显示第三片风轮	
						break;
					default:
						break;
				}		
			}
			else if((0 == Wind_High_Flag)&&(0 == Wind_Mid_Flag)&&(1 == Wind_Low_Flag))	//低速按键按下
			{
				switch(WindDisp_State)
				{
					case 0:
						FAN1;FANPOST1;				//显示第一片风轮和光柱	
						break;
					case 1:
						FAN1;						//显示第一片风轮	
						break;
					case 2:
						FAN2;						//显示第二片风轮	
						break;
					case 3:
						FAN2;						//显示第二片风轮	
						break;
					case 4:
						FAN3;						//显示第三片风轮	
						break;
					case 5:
						FAN3;						//显示第三片风轮	
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


void AutoMode_Disp(void)				//动态显示消毒图标
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
			MANUAL_ON;									//手动模式标志显示
		}
		else
		{}
	}
	else
	{
		MODE_OFF;										//关闭模式显示标志
	}
}
void AllFlag_Clr(void)				//清零所有按键状态标志位 电源除外
{
	Key_Wind_State = 0;						//风速功能状态位 0、低速 1、高速 2、低速
	Key_Phst_State = 0;						//光触媒功能状态位 0、关闭 1、工作
	Key_Disi_State = 0;						//消毒功能状态位 0、关闭 1、工作
	Key_Time_State = 0;						//定时功能状态位 0、关闭 1、1小时 2、2小时 3、4小时 4、8小时 5、12小时
	Key_Mode_State = 0;						//模式功能状态位 0、自动模式 1、手动模式
	Key_Power_State = 0;
	ManualMode_Flag = 0;					//自动手动工作标志位清零
	OPER_Flag = 0;								//系统关闭  动态显示清除
	Disi_Flag = 0;								//消毒工作状态位 0、关闭 1、工作	用于温第二个数码管和等离子标志位的冲突
	Wind_High_Flag = 0;						//风速高速模式标志位
	Wind_Mid_Flag = 0;						//风速中速模式标志位
	Wind_Low_Flag = 0;						//风速低速模式标志位
	SelfTest_flag = 0;    				//自检标志位 0、为正常状态1、为自检模式 2、为手动自动模式选择	
	CountDown_Flag = 0;						//倒计时计数标志位
	Count2_Flag = 0;							//PM2.5 5分钟状态计时标志位清零
	FiveMinute_Flag = 0;					//PM2.5 5分钟状态计时时间到达标志位清零
	Count5_Flag = 0;							//TVOC 5分钟状态计时标志位清零
	FiveMinute5_Flag = 0;					//TVOC 5分钟状态计时时间到达标志位清零
}
void LEDS_OFF(void)							//关闭所有数码管
{
	LED1_OFF;											//关闭数码管1
	LED2_OFF;											//关闭数码管2
	LED3_OFF;											//关闭数码管3
	LED4_OFF;											//关闭数码管4
	LED5_OFF;											//关闭数码管5
	LED6_OFF;											//关闭数码管6
	LED7_OFF;											//关闭数码管7
	LED8_OFF;											//关闭数码管8
	LED9_OFF;											//关闭数码管9
	LED10_OFF;											//关闭数码管10
}
void LEDS_ON(void)							//显示所有数码管
{
	LED1_ON;											//打开数码管1
	LED2_ON;											//打开数码管2
	LED3_ON;											//打开数码管3
	LED4_ON;											//打开数码管4
	LED5_ON;											//打开数码管5
	LED6_ON;											//打开数码管6
	LED7_ON;											//打开数码管7
	LED8_ON;											//打开数码管8
	LED9_ON;											//打开数码管9
	LED10_ON;											//打开数码管10
}
void Symbols_ON(void)					//显示所有标志
{
	WIND_ON;						//显示风速标志
	PHST_ON;						//显示光触媒标志
	DISI_ON;						//显示消毒标志
	MODE_ON;						//显示模式标志
	TIME_ON;						//显示倒计时标志
	TEMP_ON;						//显示温度标志
	HUMI_ON;						//显示湿度标志
	PM_ON;							//显示PM2.5
	PMPOST_ON;						//显示PM2.5光柱
	TVOC_ON;						//显示TVOC标志
	TVOCPOST_ON;					//显示TVOC所有光柱
}
void Symbols_OFF(void)					//显示所有标志
{
	WIND_OFF;					//关闭风速标志
	PHST_OFF;					//关闭光触媒标志
	DISI_OFF;					//关闭消毒标志
	MODE_OFF;					//关闭模式标志
	TIME_OFF;					//关闭定时标志
	TEMP_OFF;					//关闭温度标志
	HUMI_OFF;					//关闭湿度标志
	PM_OFF;						//关闭PM2.5
	PMPOST_OFF;					//关闭PM2.5光柱
	TVOC_OFF;					//关闭TVOC标志
	TVOCPOST_OFF;				//关闭TVOC光柱
}
/*end of file*/
