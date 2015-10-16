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



__task void Task_KeyProcess(void);			//����ɨ�貢ִ����Ӧ����������
__task void Task_ADC(void);						//��������������
__task void Task_IOTogle(void);				//
__task void Task_LcdDisp(void);			//Һ����ʾ����ͼ�ʱ����
__task void Task_Wind(void);					//����ת������
__task void Task_KeyScan(void);


os_mbx_declare (mbx_key, 20);

void SYS_Init(void);

/*******variable declaration****************/

unsigned char Lcd_Write_Data[32] = {
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char OPER_Flag = 0;				//���ر�־λ(���б�־)0.�ر� 1.����
unsigned char CountDown_Flag = 0;		//����ʱִ�б�־λ
unsigned int 	Count_Down = 0;				//��¼����ʱʱ�����
unsigned char Minute_Flag0 = 0;			//1���ӵ����־λ ���ڵ���ʱ
unsigned char Second_Flag0 = 0; 		//1���ӵ����־λ ���ڵ���ʱ
unsigned int 	Count0 = 1;						//����0.1s�Լ�һ�� ���ڵ���ʱ	
unsigned char SelfTest_flag = 0;		//�Լ��־λ 0��Ϊ����״̬1��Ϊ�Լ�ģʽ 2��Ϊ�ֶ��Զ�ģʽѡ��	
unsigned int 	Count1 = 1;						//�������� 0.1s�Լ�һ�� ���ڿ����Լ��ģʽѡ��
unsigned char Second_Flag1 = 0; 		//1���ӵ����־λ ���ڿ����Լ��ģʽѡ��
unsigned int 	Count2 = 1;						//����PM2.5 5����״̬�仯��ʱ
unsigned char FiveMinute_Flag = 0; 	//PM2.5 5���Ӽ�ʱ�����־λ
unsigned char Count2_Flag = 0;			//PM2.5 5���Ӽ�ʱ��ʼ��־λ
unsigned char Minute_Flag3 = 1;			//5���ӱ�־λ ���ڴ�������ʱ����
unsigned int 	Count3 = 0;						//����0.1s�Լ�һ�� ���ڶ�ʱ����
unsigned int 	Count4 = 1;		  			//���ڹⴥý����������2Сʱ���Զ��ر�
unsigned char Count5_Flag = 0;			//��ζ5���Ӽ�ʱ��ʼ��־
unsigned int 	Count5 = 1;		  			//5���Ӽ�ʱ����
unsigned char FiveMinute5_Flag = 0;	//��ζ5���Ӽ�ʱ������־
unsigned char TVOCSample_Flag = 0;	//��ζ������������־ 0��Ԥ�� 1������ 2��������ȴ�
DHT11_Data_TypeDef DHT11_Data;			//��ʪ�ȴ��������ݼ�¼�ṹ��
	
volatile uint32_t Width = 0;				//20us����һ�� 
volatile uint32_t Period = 0;				//0.1ms����һ��
unsigned int wait = 0;							//0.1s����һ�� 
uint32_t code = 0;									//��¼�Ӻ���ң�ؽ��ձ���
uint8_t index = 0;									//������ձ���λ����¼����
U8  Remote_Rdy = 0;    							//������յ�����  

char data = KEY_NULL,data_prev = KEY_NULL;
int RemoTime = 0,RemoTime_prev = 0;
uint8_t number = 0;									//��¼������12��Ϊ1������	
uint8_t Sleep_Flag =0;	/*����״̬λ 0�������������ɲ��� 1����������֮�� ����رհ���������
										2�������� 3���̰��������ѱ��� ����������*/		
								
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
	U8 OK = 0;			//�ж��Ƿ���յ�1������λ
	U8 RODATA = 0;		//���ڼ�¼�жϸߵ͵�ƽ�ı�����0����1
	/* For P3.2, clear the INT flag */
	GPIO_CLR_INT_FLAG(P3, BIT2);
	if(0 == HONGWAI)			
	{
 		TIMER_Stop(TIMER0);		//���յ��͵�ƽ�رն�ʱ��
 		TIMER_Stop(TIMER1);		//���յ��͵�ƽ�رն�ʱ��
		if(Width == 75)			//���յ��ߵ�ƽʱ�䳬��1.5msʱ ���յ��Ĳ������������
		{
			Remote_Rdy = 0;		//����û���յ�����
			index = 0;			//�������λ��������
		}
		else if((Width >= 40) && (Width < 75)) {OK=1;RODATA = 0;}//�ߵ�ƽʱ����0.5ms���ǡ�0��
		else if((Width >= 5) && (Width < 40)) {OK=1;RODATA = 1;}//�ߵ�ƽʱ����1.5s���ǡ�1��
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
		/*�͵�ƽ��ʱ������ߵ�ƽ��������*/
		Width = 0;
	}
	else if(HONGWAI)
	{
		/* Start Timer0 counting */
		TIMER_Start(TIMER0);	//���յ��ߵ�ƽ�򿪶�ʱ�� �Ըߵ�ƽ��ʱ����м���
		/* Start Timer1 counting */
		TIMER_Start(TIMER1);	//���յ��ߵ�ƽ�򿪶�ʱ�� �Ըߵ�ƽ��ʱ����м���
		//Width = 0;
		Period = 0;				//��ʱ��������
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
void TMR1_IRQHandler(void)				//0.1ms����һ���ж� ����һ�� 
{
    if(TIMER_GetIntFlag(TIMER1) == 1)
    {
        /* Clear Timer1 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER1);
		if(Period >= 150) 				//15msһ��ѭ��
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
void TMR2_IRQHandler(void)				//0.1s����һ���ж� ����һ�� 
{
    if(TIMER_GetIntFlag(TIMER2) == 1)
    {
			/* Clear Timer1 time-out interrupt flag */
			TIMER_ClearIntFlag(TIMER2);
			if(wait >= 20) 				//2sһ��ѭ��
			{
				wait = 0;				//�����ʱ����
				Sleep_Flag = 1;			/*����״̬λ��1����������֮�� ����رհ���������*/	
				LED_OFF;				//�رձ���
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
	KEY_POWERON;										//��ʾ���ذ�����־
	while(1)
	{
		Time_Count0();								//����ʱ�ļ�ʱ����
		Time_Count1();								//�����Լ��ģʽѡ���ʱ����
		Time_Count2();								//pm2.5 5����״̬�仯
		Time_Count3();								//����5���Ӷ�ʱ����
		Time_Count4();								//�ⴥý����2Сʱ��ر�
		Time_Count5();								//��ζ5����״̬�仯
		LCD_Oper(Lcd_Write_Data);			//ˢ����Ļ����
		AutoMode_Disp();							//�Զ�ͼ����ʾ
		Wind_Disp();									//����ͼ����ʾ
		os_dly_wait(100);
	}
}
__task void Task_IOTogle(void)
{
	/* Configure P4.4 as Output mode */
	GPIO_SetMode(P4, BIT4, GPIO_PMD_OUTPUT);		//����ܽ�����Ϊ���
	/* Configure P2.6 as Output mode */
	GPIO_SetMode(P2, BIT6, GPIO_PMD_OUTPUT);		//�����ƿ��ƹܽ�����Ϊ���
	GPIO_SetMode(P3, BIT7, GPIO_PMD_OUTPUT);		//�������ܽ�����Ϊ���
	Tim_Config();																//�������ƽ��ʱ������
	HWGPIO_Config();														//����GPIO����
	Relay_Init();																//�̵�����ʼ�� ���ر����м̵���
	BUZZER_OFF;																	//�رշ�����
	LED_ON;																			//�������
	while(1)
	{	
		SelfTest();										//�����Լ�
		if(1 == OPER_Flag)						//ϵͳ���� �Լ����
		{
			Disp_CountDown();						//��ʱ����ʾ	
			Auto_Wind();								//�Զ�ģʽ�� ������pm2.5����ζ�����ݱ仯
 		}
		else
		{
			CLOSE_ALLRELAY;							//�ر����е�Դ
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
				case KEY_WIND:						//���ٰ�������
					BUZZER_ON;						//����������һ��
					Set_Wind();						//ִ�з��ٱ任����
					break;
				case KEY_PHST:						//�ⴥý��������
					BUZZER_ON;						//����������һ��
					Set_Phst();						//ִ�йⴥý
					break;
				case KEY_DISI:						//������������
					BUZZER_ON;						//����������һ��
					Set_Disi();						//ִ����������
					break;
				case KEY_PWR:						//��Դ��������
					BUZZER_ON;						//����������һ��
					Set_Power();					//����
					break;
				case KEY_MOD:						//ģʽ��������
					BUZZER_ON;						//����������һ��
					Set_Mode();						//ģʽѡ�����
					break;								
				case KEY_TMR:						//��ʱ��������
					BUZZER_ON;						//����������һ��
					Set_Time();						//ִ�ж�ʱ����
//					TIME3_INIT;						//���ڴ�����ʱ �а������������¼��� 
					break;
				default:
					break;
			}
	}
}

__task void Task_KeyScan(void)		//����ɨ����� ͨ������Ͱ��������������ͨ��
{
	char key,key_prev;
	KeyBoard_Init();			//������ʼ��
	key = KEY_NULL;
	key_prev = KEY_NULL;
	while(1)
	{
 		key = KeyBoard_Scan();	//����ɨ��
		if(key != KEY_NULL)			//����Ч��������
		{	
			if(key_prev != key)
			{
				key_prev = key;			//����Ч�������µ�ʱ�� ����ǰ��������֮ǰ��������
			}
		}
		else					
		{
			if(key_prev != KEY_NULL)		//��ǰ����Ч�������� ���Ǽ�¼֮ǰ��������Ϊ�ǿ�
			{
				os_mbx_send (&mbx_key, (void**)key_prev, 20);	//����֮ǰ������ֵ
				key_prev = KEY_NULL;							//���֮ǰ��������
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
	TVOC_Ctrl = 1;								//��ζ����Ԥ�ȿ��ƹܽ���1 0������ 1��������
	/* Configure P2.0 as Output mode */
	GPIO_SetMode(P2, BIT0, GPIO_PMD_OUTPUT);	//pm2.5�Ĳ����ƿ���
	ADC_Config();									//adc����
	DHT11_GPIO_Config();
	while(1)
	{
		//Tvoc();
		if((1 == OPER_Flag)&& (1 == Minute_Flag3))			//��Դ����״̬ 5���Ӳ���ʱ�䵽
		{
			Minute_Flag3 = 0;			//���㶨ʱ������־λ
			if( Read_DHT11(&DHT11_Data)==SUCCESS)	//����DHT11������
			{
				temp = DHT11_Data.temp_int * 10 + DHT11_Data.temp_deci;	//DHT11�����¶����ݼ���ת��
				humi = DHT11_Data.humi_int * 10 + DHT11_Data.humi_deci;	//DHT11����ʪ�����ݼ���ת��

				Disp_Temp(temp);	//�������ʾ�¶�����
				Disp_Humi(humi);	//�������ʾ�¶�����
			}
			else
			{}	
				
		}
 			else if((1 == OPER_Flag)&&(0 == SelfTest_flag)&&(1 == TVOCSample_Flag)) //||( )
			{
				TVOCSample_Flag = 2;
				Tvoc();						//��ζ�������������ݲ���ʾ
			}
			else if((1 == OPER_Flag)&&(0 == SelfTest_flag)&&(0 == TVOCSample_Flag))		//Ϊ�˿�����ʾ��ζ����
			{
				TVOC_Disp(TVOC_State);
			}
			else
			{}
		if((1 == OPER_Flag)&&(0 == SelfTest_flag))
		{
			loop();						//pm2.5��������ʾ
			//Tvoc();						//��ζ�������������ݲ���ʾ
		}
			else
			{}
		os_dly_wait(100);	
	}
	
}
__task void Task_Init(void)
{
	os_tsk_prio_self(8);										//�������ȼ�Ϊ�� ���ж�������֮�� ������ĳɿ��Ź�����
	os_mbx_init (&mbx_key, sizeof(mbx_key));

	os_tsk_create(Task_IOTogle ,3);		
	os_tsk_create(Task_Sensor   ,3);		//
	os_tsk_create(Task_KeyProcess,4);							//������������
	os_tsk_create(Task_KeyScan, 4);								//����ɨ������
	os_tsk_create(Task_LcdDisp,6);								//��ʾ����
	
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
