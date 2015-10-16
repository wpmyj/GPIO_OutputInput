#include "hongwai.h"
   

void Tim_Config(void)	//定时器配置  用于测量红外高电平的时间
{
	/* Open Timer0 frequency to 50k Hz in periodic mode, and enable interrupt */
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 50000);
    TIMER_EnableInt(TIMER0);
	/* Open Timer1 frequency to 10k Hz in periodic mode, and enable interrupt */
	TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 10000);
    TIMER_EnableInt(TIMER1);
	/* Open Timer2 frequency to 10 Hz in periodic mode, and enable interrupt */
	//用于按键长按休眠
	TIMER_Open(TIMER2, TIMER_PERIODIC_MODE, 10);
    TIMER_EnableInt(TIMER2);
	/* Enable Timer0、1 NVIC */
    NVIC_EnableIRQ(TMR0_IRQn);
	NVIC_EnableIRQ(TMR1_IRQn);
	NVIC_EnableIRQ(TMR2_IRQn);
// 	/* Start Timer0、1counting */
//      TIMER_Start(TIMER0);	
//       TIMER_Start(TIMER1);
	TIMER_Start(TIMER2);		
}
void HWGPIO_Config(void)
{  
	/* Configure P3.2 as EINT0 pin and enable interrupt by falling edge trigger */
    GPIO_SetMode(P3, BIT2, GPIO_PMD_INPUT);
    GPIO_EnableEINT0(P3, 2, GPIO_INT_BOTH_EDGE);
    NVIC_EnableIRQ(EINT0_IRQn);
	
	/* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 10 KHz clock */
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_1);
    GPIO_ENABLE_DEBOUNCE(P3, BIT2);
}

/*end of file*/

