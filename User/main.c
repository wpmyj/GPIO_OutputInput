/**************************************************************************//**
 * @file     main.c
 * @version  V2.03
 * $Revision: 2 $
 * $Date: 13/06/28 10:14a $ 
 * @brief    M051 Series General Purpose I/O Driver Sample Code
 *
 * @note
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "M051Series.h"
#include <RTL.h>
#include "Key_Board.h"
#include "lcd.h"
#include "Time.h"
#include "Sensor.h"
#include "hongwai.h"

#define PLLCON_SETTING      SYSCLK_PLLCON_50MHz_XTAL
#define PLL_CLOCK           50000000



__task void Task_KeyProcess(void);			//键盘扫描并执行相应操作的任务
__task void Task_ADC(void);						//传感器工作任务
__task void Task_IOTogle(void);				//
__task void Task_LcdDisp(void);			//液晶显示任务和计时任务
__task void Task_Wind(void);					//风扇转动任务
__task void Task_KeyScan(void);


os_mbx_declare (mbx_key, 20);

void SYS_Init(void);

/*******variable declaration****************/

unsigned char Lcd_Write_Data[32] = {
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char OPER_Flag = 0;				//开关标志位(运行标志)0.关闭 1.运行
unsigned char CountDown_Flag = 0;		//倒计时执行标志位
unsigned int 	Count_Down = 0;				//记录倒计时时间变量
unsigned char Minute_Flag0 = 0;			//1分钟到达标志位 用于倒计时
unsigned char Second_Flag0 = 0; 		//1秒钟到达标志位 用于倒计时
unsigned int 	Count0 = 1;						//计数0.1s自加一次 用于倒计时	
unsigned char SelfTest_flag = 0;		//自检标志位 0、为正常状态1、为自检模式 2、为手动自动模式选择	
unsigned int 	Count1 = 1;						//计数变量 0.1s自加一次 用于开机自检和模式选择
unsigned char Second_Flag1 = 0; 		//1秒钟到达标志位 用于开机自检和模式选择
unsigned int 	Count2 = 1;						//用于PM2.5 5分钟状态变化计时
unsigned char FiveMinute_Flag = 0; 	//PM2.5 5分钟计时到达标志位
unsigned char Count2_Flag = 0;			//PM2.5 5分钟计时开始标志位
unsigned char Minute_Flag3 = 1;			//5分钟标志位 用于传感器定时采样
unsigned int 	Count3 = 0;						//计数0.1s自加一次 用于定时采样
unsigned int 	Count4 = 1;		  			//用于光触媒和消毒运行2小时后自动关闭
unsigned char Count5_Flag = 0;			//异味5分钟计时开始标志
unsigned int 	Count5 = 1;		  			//5分钟计时变量
unsigned char FiveMinute5_Flag = 0;	//异味5分钟计时结束标志
unsigned char TVOCSample_Flag = 0;	//异味传感器采样标志 0、预热 1、采样 2、采样后等待
DHT11_Data_TypeDef DHT11_Data;			//温湿度传感器数据记录结构体
	
volatile uint32_t Width = 0;				//20us自增一次 
volatile uint32_t Period = 0;				//0.1ms自增一次
unsigned int wait = 0;							//0.1s自增一次 
uint32_t code = 0;									//记录从红外遥控接收编码
uint8_t index = 0;									//红外接收编码位数记录变量
U8  Remote_Rdy = 0;    							//红外接收到数据  

char data = KEY_NULL,data_prev = KEY_NULL;
int RemoTime = 0,RemoTime_prev = 0;
uint8_t number = 0;									//记录脉冲数12个为1个周期	
uint8_t Sleep_Flag =0;	/*休眠状态位 0、背光亮按键可操作 1、长按按键之后 背光关闭按键不动作
										2、不操作 3、短按按键唤醒背光 按键不动作*/		
								
/**
 * @brief       External INT0 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The External INT0(P3.2) default IRQ, declared in startup_M051Series.s.
 */
void EINT0_IRQHandler(void)
{
	U8 OK = 0;			//判断是否接收到1个编码位
	U8 RODATA = 0;		//用于记录判断高低电平的编码是0还是1
	/* For P3.2, clear the INT flag */
	GPIO_CLR_INT_FLAG(P3, BIT2);
	if(0 == HONGWAI)			
	{
 		TIMER_Stop(TIMER0);		//接收到低电平关闭定时器
 		TIMER_Stop(TIMER1);		//接收到低电平关闭定时器
		if(Width == 75)			//接收到高电平时间超过1.5ms时 接收到的不是所需的数据
		{
			Remote_Rdy = 0;		//红外没接收到数据
			index = 0;			//红外编码位计数清零
		}
		else if((Width >= 40) && (Width < 75)) {OK=1;RODATA = 0;}//高电平时间是0.5ms，是“0”
		else if((Width >= 5) && (Width < 40)) {OK=1;RODATA = 1;}//高电平时间是1.5s，是“1”
		else
		{}
		if(OK)
		{
			OK=0;
			index++;
			code >>= 1;
			if(RODATA)
				code += 0x800;
			else
			{}
		}
		else
		{}
		if(11 == index)
		{
			code = code >> 7;
			Remote_Rdy = 1;
		}
		else
		{}		
		/*低电平的时候清零高电平计数变量*/
		Width = 0;
	}
	else if(HONGWAI)
	{
		/* Start Timer0 counting */
		TIMER_Start(TIMER0);	//接收到高电平打开定时器 对高电平的时间进行计数
		/* Start Timer1 counting */
		TIMER_Start(TIMER1);	//接收到高电平打开定时器 对高电平的时间进行计数
		//Width = 0;
		Period = 0;				//计时变量清零
	}
	else
	{}
	if(Remote_Rdy)
	{
		data = code;
	}	
}


/**
 * @brief       Timer0 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The Timer0 default IRQ, declared in startup_M051Series.s.
 */
void TMR0_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER0) == 1)
    {
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER0);
		if(Width >= 75)
			Width = 75;
		else
			Width++;
    }
}
/**
 * @brief       Timer1 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The Timer1 default IRQ, declared in startup_M051Series.s.
 */
void TMR1_IRQHandler(void)				//0.1ms进入一次中断 计数一次 
{
    if(TIMER_GetIntFlag(TIMER1) == 1)
    {
        /* Clear Timer1 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER1);
		if(Period >= 150) 				//15ms一个循环
			Period = 0;
		else
			Period++;
		if(data != KEY_NULL)
		{	
			if(Period >= 100)
			{
				Period = 0;
				if(isr_mbx_check(&mbx_key) != 0){
				isr_mbx_send(&mbx_key, (void**)data);
				}
				data = KEY_NULL;
			}
		}
		else
		{}	
    }
}
/**
 * @brief       Timer2 IRQ
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The Timer1 default IRQ, declared in startup_M051Series.s.
 */
void TMR2_IRQHandler(void)				//0.1s进入一次中断 计数一次 
{
    if(TIMER_GetIntFlag(TIMER2) == 1)
    {
			/* Clear Timer1 time-out interrupt flag */
			TIMER_ClearIntFlag(TIMER2);
			if(wait >= 20) 				//2s一个循环
			{
				wait = 0;				//清零计时变量
				Sleep_Flag = 1;			/*休眠状态位置1、长按按键之后 背光关闭按键不动作*/	
				LED_OFF;				//关闭背光
			}
			else
			{
				wait++;
				//Sleep_Flag =0;
			}
    }
}

__task void Task_LcdDisp(void)
{
	LCD_Init();
	KEY_POWERON;										//显示开关按键标志
	while(1)
	{
		Time_Count0();								//倒计时的计时函数
		Time_Count1();								//开机自检和模式选择计时函数
		Time_Count2();								//pm2.5 5分钟状态变化
		Time_Count3();								//用于5分钟定时采样
		Time_Count4();								//光触媒运行2小时后关闭
		Time_Count5();								//异味5分钟状态变化
		LCD_Oper(Lcd_Write_Data);			//刷新屏幕数据
		AutoMode_Disp();							//自动图标显示
		Wind_Disp();									//风速图标显示
		os_dly_wait(100);
	}
}
__task void Task_IOTogle(void)
{
	/* Configure P4.4 as Output mode */
	GPIO_SetMode(P4, BIT4, GPIO_PMD_OUTPUT);		//背光管脚设置为输出
	/* Configure P2.6 as Output mode */
	GPIO_SetMode(P2, BIT6, GPIO_PMD_OUTPUT);		//待机灯控制管脚设置为输出
	GPIO_SetMode(P3, BIT7, GPIO_PMD_OUTPUT);		//蜂鸣器管脚设置为输出
	Tim_Config();																//红外检测电平定时器配置
	HWGPIO_Config();														//红外GPIO配置
	Relay_Init();																//继电器初始化 并关闭所有继电器
	BUZZER_OFF;																	//关闭蜂鸣器
	LED_ON;																			//背光灯亮
	while(1)
	{	
		SelfTest();										//开机自检
		if(1 == OPER_Flag)						//系统开启 自检结束
		{
			Disp_CountDown();						//定时器显示	
			Auto_Wind();								//自动模式下 风速随pm2.5和异味的数据变化
 		}
		else
		{
			CLOSE_ALLRELAY;							//关闭所有电源
		}
		os_dly_wait(100);
	}
}

__task void Task_KeyProcess(void)
{
	unsigned int key;
	while(1)
 	{
		os_mbx_wait (&mbx_key, (void**)&key, 0xffff);
		if((Sleep_Flag != 0)&&(key != KEY_NULL))
		{
			key = KEY_NULL;
			Sleep_Flag++;
		}
		else 
		{}
		if(3 == Sleep_Flag)
		{
			LED_ON;
			Sleep_Flag = 0;
		}
			else
			{}
			switch(key)
			{
				case KEY_WIND:						//风速按键按下
					BUZZER_ON;						//蜂鸣器短响一声
					Set_Wind();						//执行风速变换操作
					break;
				case KEY_PHST:						//光触媒按键按下
					BUZZER_ON;						//蜂鸣器短响一声
					Set_Phst();						//执行光触媒
					break;
				case KEY_DISI:						//消毒按键按下
					BUZZER_ON;						//蜂鸣器短响一声
					Set_Disi();						//执行消毒操作
					break;
				case KEY_PWR:						//电源按键按下
					BUZZER_ON;						//蜂鸣器短响一声
					Set_Power();					//开关
					break;
				case KEY_MOD:						//模式按键按下
					BUZZER_ON;						//蜂鸣器短响一声
					Set_Mode();						//模式选择操作
					break;								
				case KEY_TMR:						//定时按键按下
					BUZZER_ON;						//蜂鸣器短响一声
					Set_Time();						//执行定时操作
//					TIME3_INIT;						//用于待机计时 有按键按下则重新计数 
					break;
				default:
					break;
			}
	}
}

__task void Task_KeyScan(void)		//按键扫描程序 通过信箱和按键操作程序进行通信
{
	char key,key_prev;
	KeyBoard_Init();			//按键初始化
	key = KEY_NULL;
	key_prev = KEY_NULL;
	while(1)
	{
 		key = KeyBoard_Scan();	//按键扫描
		if(key != KEY_NULL)			//无有效按键按下
		{	
			if(key_prev != key)
			{
				key_prev = key;			//有有效按键按下的时候 将当前按键赋予之前按键变量
			}
		}
		else					
		{
			if(key_prev != KEY_NULL)		//当前无有效按键按下 但是记录之前按键变量为非空
			{
				os_mbx_send (&mbx_key, (void**)key_prev, 20);	//发送之前按键键值
				key_prev = KEY_NULL;							//清空之前按键变量
			}
		}
		os_dly_wait(10);
	}
}

__task void Task_Sensor(void)
{
	int temp = 0, humi = 0;
	/* Configure P1.7 as output mode */
	GPIO_SetMode(P1, BIT7, GPIO_PMD_OUTPUT);
	TVOC_Ctrl = 1;								//异味传感预热控制管脚置1 0、工作 1、不工作
	/* Configure P2.0 as Output mode */
	GPIO_SetMode(P2, BIT0, GPIO_PMD_OUTPUT);	//pm2.5的采样灯控制
	ADC_Config();									//adc配置
	DHT11_GPIO_Config();
	while(1)
	{
		//Tvoc();
		if((1 == OPER_Flag)&& (1 == Minute_Flag3))			//电源开启状态 5分钟采样时间到
		{
			Minute_Flag3 = 0;			//清零定时采样标志位
			if( Read_DHT11(&DHT11_Data)==SUCCESS)	//读出DHT11的数据
			{
				temp = DHT11_Data.temp_int * 10 + DHT11_Data.temp_deci;	//DHT11读出温度数据计算转化
				humi = DHT11_Data.humi_int * 10 + DHT11_Data.humi_deci;	//DHT11读出湿度数据计算转化

				Disp_Temp(temp);	//数码管显示温度数据
				Disp_Humi(humi);	//数码管显示温度数据
			}
			else
			{}	
				
		}
 			else if((1 == OPER_Flag)&&(0 == SelfTest_flag)&&(1 == TVOCSample_Flag)) //||( )
			{
				TVOCSample_Flag = 2;
				Tvoc();						//异味传感器读出数据并显示
			}
			else if((1 == OPER_Flag)&&(0 == SelfTest_flag)&&(0 == TVOCSample_Flag))		//为了开机显示异味数据
			{
				TVOC_Disp(TVOC_State);
			}
			else
			{}
		if((1 == OPER_Flag)&&(0 == SelfTest_flag))
		{
			loop();						//pm2.5测量及显示
			//Tvoc();						//异味传感器读出数据并显示
		}
			else
			{}
		os_dly_wait(100);	
	}
	
}
__task void Task_Init(void)
{
	os_tsk_prio_self(8);										//设置优先级为高 所有都测试完之后 该任务改成看门狗任务
	os_mbx_init (&mbx_key, sizeof(mbx_key));

	os_tsk_create(Task_IOTogle ,3);		
	os_tsk_create(Task_Sensor   ,3);		//
	os_tsk_create(Task_KeyProcess,4);							//按键操作任务
	os_tsk_create(Task_KeyScan, 4);								//按键扫描任务
	os_tsk_create(Task_LcdDisp,6);								//显示任务
	
	os_tsk_delete_self();
}
/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/

int main (void)
{

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();     

	os_sys_init(Task_Init);
    while (1)
	{}
}

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

/*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for external XTAL clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);
		
	/* Enable peripheral clock */
    CLK->APBCLK = CLK_APBCLK_ADC_EN_Msk | CLK_APBCLK_TMR0_EN_Msk | CLK_APBCLK_TMR1_EN_Msk | CLK_APBCLK_TMR2_EN_Msk;
	
	/* ADC clock source is 22.1184MHz, set divider to 7, ADC clock is 22.1184/7 MHz */
    CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_CLKDIV_ADC(7));
	
	/* Peripheral clock source */
    CLK->CLKSEL1 = CLK_CLKSEL1_TMR0_S_HXT | CLK_CLKSEL1_TMR1_S_HCLK | CLK_CLKSEL1_TMR2_S_HIRC ;
	
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    //SystemCoreClockUpdate();
    PllClock        = PLL_CLOCK;            // PLL
    SystemCoreClock = PLL_CLOCK / 1;        // HCLK
    CyclesPerUs     = PLL_CLOCK / 1000000;  // For SYS_SysTickDelay()

/*---------------------------------------------------------------------------------------------------------*/
/* Init I/O Multi-function                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

    /* Disable the P1.5 - P1.6 digital input path to avoid the leakage current */
     GPIO_DISABLE_DIGITAL_PATH(P1, 0x60);

   /* Set P3 multi-function pins for UART0 RXD, TXD, EINT0 and EINT1 */
    SYS->P3_MFP &= ~( SYS_MFP_P32_Msk );
    SYS->P3_MFP |=  SYS_MFP_P32_INT0;
	/* Configure the P1.5 - P1.6 ADC analog input pins */
    SYS->P1_MFP &= ~(SYS_MFP_P15_Msk | SYS_MFP_P16_Msk);
    SYS->P1_MFP |= SYS_MFP_P15_AIN5 | SYS_MFP_P16_AIN6; 
	/* Set P3 multi-function pins for EINT0 */
    SYS->P3_MFP &= ~( SYS_MFP_P32_Msk );
    SYS->P3_MFP |= (SYS_MFP_P32_INT0);
	/* Lock protected registers */
    SYS_LockReg();
}

/*end of file*/
