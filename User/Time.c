#include "Time.h"

//倒计时的计时函数 输出1s和1min
void Time_Count0(void)
{
	if(1 == CountDown_Flag)
	{
		if(0 == (Count0 % 498))				//1分钟--498(矫正之后)  调试的时候用5s--50 
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

/*开机计时5秒、开机1分钟无动作关闭背光*/
void Time_Count1(void)
{
	if(1 == SelfTest_flag)   //自检标志位 0、为正常状态1、为自检模式 2、为手动自动模式选择 3、运行自动模式
	{
		OPER_Flag = 0;					//未进入运行状态
		if(0 == (Count1 % 40)) 	//5秒钟
		{
			Count1 = 1;
			SelfTest_flag = 2;		//设置为模式选择状态
		}
		else
			Count1++;
	}
		else if(2 == SelfTest_flag)
		{
			OPER_Flag = 0;				//未进入运行状态
			if(1 == ManualMode_Flag)
			{
				SelfTest_flag = 4;
			}
			else
			{}
			if(0 == (Count1 % 40)) //5秒钟
			{
				Count1 = 1;
				SelfTest_flag = 3;		//设置为自动模式状态
			}
			else
				Count1++;
		}
		else if((0 == SelfTest_flag)&&(0 == OPER_Flag)&&(1 == LED))		//正常状态下 未开机 背光灯开启 1分钟到关闭背光
		{
			if(0 == (Count1 % 498))			//1分钟关闭背光  1min--498  调试时用10s--80
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
		if(0 == (Count2 % 2490))				//5min--2490  调试的时候用5s--50
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

void Time_Count3(void)						//5分钟倒计时 定时采样
{
	if(1 == OPER_Flag)						//开机状态
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

// void Time_Count3(void)						//5分钟倒计时 定时采样
// {
// 	if(1 == OPER_Flag)						//开机状态
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
void Time_Count4(void)						//2小时到关闭光触媒消毒
{
	if((1 == OPER_Flag)&&(0 == ManualMode_Flag))						//开机状态 自动模式
	{
		if((1 == Phst_Flag)&&(1 == Disi_Flag))
		{
			if(0 == (Count4 % 59760))				//2小时到--59760(矫正之后)	测试时用300s--2490
			{
				Count4 = 1;
				Relay4_OFF;							//光触媒电源关闭
				PHST_OFF;							//关闭光触媒
				Phst_Flag = 0;
				Relay5_OFF;							//消毒电源关闭	
				DISI_OFF;							//关闭消毒
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
void Time_Count5(void)						//异味5分钟下降档位
{
	if(1 == Count5_Flag)
	{
		if(0 == (Count5 % 2490))				//5min--2490  调试的时候用5s--50
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

// void Time_Count6(void)						//1分钟倒计时 用于传感器采集数据
// {
// 	if(1 == OPER_Flag)						//开机状态
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
