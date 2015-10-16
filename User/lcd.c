#include "lcd.h"

/*编码正序ABCD在EFG前*/
const unsigned char Led_Normal_Order[10] = {0xf5,0x60,0xd3,0xf2,0x66,0xb6,0xb7,0xe0,0xf7,0xf6};
/*编码逆序EFG在ABCD前*/
const unsigned char Led_Inverse_Order[10] = {0x5f,0x06,0x3d,0x2f,0x66,0x6b,0x7b,0x0e,0x7f,0x6f};

//volatile char PM_State = 0;

void delay(void)
{
	unsigned char i;
	for(i = 0; i < 200; i++);
}

/*lcd 写入cnt个比特位*/
void LCD_WrData(unsigned char Data, unsigned char cnt)
{
	unsigned char i;
	for (i = 0; i < cnt; i++)
	{
		HT1621_WR = LOW;//P0_OUT &= ~HT1621_WR;
		if ((Data & 0x80) == 0x80)
		{
			HT1621_DAT = HIGH;//P0_OUT |=HT1621_DAT;
		}
		else
		{
			HT1621_DAT = LOW;//P0_OUT &=~HT1621_DAT;
		}
		NOP();
		HT1621_WR = HIGH;//P0_OUT |=HT1621_WR;
		NOP();			 //这个延时很重要  如果没有的话将不能正常显示
		Data <<= 1;
	}
}
/*lcd写命令函数*/
void LCD_WrCmd(unsigned char Cmd)
{
  HT1621_CS = LOW;//P0_OUT&=~HT1621_CS;
	LCD_WrData(0x80,4);          //写入命令标志100
	LCD_WrData(Cmd,8);                 //写入命令数据
	HT1621_CS = HIGH;//P0_OUT |= HT1621_CS;
}
/*lcd写四位数据*/
void LCD_WrOneData(unsigned char Addr, unsigned char Data)
{
	HT1621_CS = LOW;//P0_OUT &= ~HT1621_CS;
	LCD_WrData(0xA0, 3);                 //写入数据标志101
	LCD_WrData(Addr << 2, 6);          //写入地址数据
	LCD_WrData(Data << 4, 4);          //写入数据
	HT1621_CS = HIGH;//P0_OUT |= HT1621_CS;
}
/*清屏*/
void LCD_WrAllData(void)
{
	unsigned char i;
	for (i = 0; i < 32; i++)
	{
		Lcd_Write_Data[i] = 0x00;
	}
}
/*液晶初始化*/
void LCD_Init(void)
{
	/* Configure CS as Output mode */
	GPIO_SetMode(PORT_LCD, HT1621_CS_BIT, GPIO_PMD_OUTPUT);
	 /* Configure WR as Output mode */
	GPIO_SetMode(PORT_LCD, HT1621_WR_BIT, GPIO_PMD_OUTPUT);
	 /* Configure DATA as Output mode */
	GPIO_SetMode(PORT_LCD, HT1621_DAT_BIT, GPIO_PMD_OUTPUT);
	 HT1621_CS = HIGH;//P0_OUT |= HT1621_CS;
	HT1621_WR = HIGH;//P0_OUT |= HT1621_WR;
	HT1621_DAT = HIGH;//P0_OUT |= HT1621_DAT;
	os_dly_wait(1000);                         //延时使LCD工作电压稳定
	LCD_WrCmd(BIAS);								//四个COM和1/3偏压
	LCD_WrCmd(RC256);              //使用内部晶振
	LCD_WrCmd(SYSDIS);							
	LCD_WrCmd(WDTDIS1);
	LCD_WrCmd(SYSEN);
	LCD_WrCmd(LCDON);
}
/*lcd写入1个字节的数据*/
void LCD_WrByteData(unsigned char address, unsigned char what)
{
	HT1621_CS = LOW;//P0_OUT &= ~HT1621_CS;
	LCD_WrData(0xA0, 3);          //写入数据标志101
	LCD_WrData(address << 2, 6);         //写入地址数据
	LCD_WrData(what, 8);         //写入数据

	HT1621_CS = HIGH;//P0_OUT |= HT1621_CS;
}
/*LCD刷新数据*/
void LCD_Oper(unsigned char *pBuffer)		
{
	char i;
	for(i = 0;i < 32; i++)
		{
			LCD_WrOneData(i,*pBuffer);
			pBuffer++;
		}
}
/*取出数码管显示数据的高四位,调用的参数为正序或逆序的数字段码*/
unsigned char Get_LedHigh(unsigned char Buffer)	
{
	unsigned char High = 0;
	High = (Buffer >> 4) & 0x0f;
	return High;
}
/*取出数码管显示数据的高四位,调用的参数为正序或逆序的数字段码*/
unsigned char Get_LedLow(unsigned char Buffer)
{
	unsigned char Low = 0;
	Low = Buffer & 0x0f;
	return Low;
}
/*显示倒计时*/
void Disp_CountDown(void)
{
	char led1,led2,led3,led4;//存储倒计时转换后的每一位数字
	if(1 == CountDown_Flag)  //
	{
		led1 = (unsigned char)((Count_Down / 600));												//倒计时时间转换显示
		led2 = (unsigned char)(((Count_Down / 60) % 10));
		led3 = (unsigned char)(((Count_Down % 60) / 10));
		led4 = (unsigned char)(((Count_Down % 60) % 10));
		Lcd_Write_Data[LED1_HIGH] = Get_LedHigh( Led_Inverse_Order[led1] ) | 0x08;	//或上0x08是将”剩余“两字点亮
		Lcd_Write_Data[LED1_LOW ] = Get_LedLow ( Led_Inverse_Order[led1] );
		Lcd_Write_Data[LED2_HIGH] = Get_LedHigh( Led_Inverse_Order[led2] ) | 0x08;	//或上0x08是将“时间”两字点亮
		Lcd_Write_Data[LED2_LOW ] = Get_LedLow ( Led_Inverse_Order[led2] );
		if(1 == Second_Flag0)			//一秒时间到
		{
			Second_Flag0 = 0;
			Lcd_Write_Data[LED3_HIGH] = Get_LedHigh( Led_Inverse_Order[led3] );	//显示倒计时冒号
		}
		else
		{
			Lcd_Write_Data[LED3_HIGH] = Get_LedHigh( Led_Inverse_Order[led3] ) | 0x08;	//显示倒计时冒号
		}
		Lcd_Write_Data[LED3_LOW ] = Get_LedLow ( Led_Inverse_Order[led3] );
		Lcd_Write_Data[LED4_HIGH] = Get_LedHigh( Led_Normal_Order[led4] );
		Lcd_Write_Data[LED4_LOW ] = Get_LedLow ( Led_Normal_Order[led4] )  | 0x08; //或上0x08是将风速低速方框点亮
		if(1 == Minute_Flag0)		//1分钟到
		{			
			Minute_Flag0 = 0;
			if(Count_Down > 0)
				Count_Down--;
				else
				{
 					Count_Down = 0;
// 					CLOSE_ALLRELAY;			//关闭所有电源
// 					CountDown_Flag = 0;
// 					//Bide_Flag = 1;
// 					OPER_Flag = 0;
// 					Key_Power_State = 0;//电源按键关闭
// 					LCD_WrAllData();		//清屏
// 					LED_OFF;				//关闭背光
// 					KEY_POWERON;			//开关按键图标显示
					Oper_Power_OFF();
				}
		}
		else
		{}
	}
}
// void Col_Disp(void)			//显示倒计时的冒号
// {
// 	if(1 == OPER_Flag)
// 	{
// 		if(1 == CountDown_Flag)
// 		{
// 			if(1 == Second_Flag0)			//一秒时间到
// 			{
// 				Second_Flag0 = 0;
// 				//COL_TWINKLE;							//时间分隔冒号闪烁
// 				COL_OFF;									//关闭冒号显示
// 			}
// 			else
// 			{}
// 		}
// 		else if(0 == CountDown_Flag)
// 		{
// 			COL_OFF;									//关闭冒号显示
// 		}
// 		else
// 		{}
// 	}
// 	else
// 	{}
// }

/*显示温度    数值乘以十之后调用*/
void Disp_Temp(unsigned int Temp)
{
	unsigned char led5,led6,led7;
	led5 = (unsigned char)((Temp / 100));												//温度转换用于显示
	led6 = (unsigned char)((Temp / 10) % 10);
	led7 = (unsigned char)(((Temp % 10)));
	Lcd_Write_Data[LED5_HIGH] = Get_LedHigh( Led_Inverse_Order[led5] ) | 0x08; //显示温度标志
	Lcd_Write_Data[LED5_LOW ] = Get_LedLow ( Led_Inverse_Order[led5] );
	/*消毒工作的时候显示等离子标志*/
	if(1 == Disi_Flag)																										
		Lcd_Write_Data[LED6_HIGH] = Get_LedHigh( Led_Inverse_Order[led6] ) | 0x08;	//或上0x08是将等离子标志点亮
		else if(0 == Disi_Flag)
			Lcd_Write_Data[LED6_HIGH] = Get_LedHigh( Led_Inverse_Order[led6] );
		else
		{}
	Lcd_Write_Data[LED6_LOW ] = Get_LedLow ( Led_Inverse_Order[led6] );
	Lcd_Write_Data[LED7_HIGH] = Get_LedHigh( Led_Inverse_Order[led7] ) | 0x08; //或上0x08是将温度的小数点点亮
	Lcd_Write_Data[LED7_LOW ] = Get_LedLow ( Led_Inverse_Order[led7] );
	os_dly_wait(100);
}
/*湿度显示		数值乘以十后使用*/
void Disp_Humi(unsigned int Humi)
{
	unsigned char led8,led9,led10;
	led8 = (unsigned char)((Humi / 100));												//湿度转换显示
	led9 = (unsigned char)((Humi / 10) % 10);
	led10 = (unsigned char)(((Humi % 10)));
	Lcd_Write_Data[LED8_HIGH] = Get_LedHigh( Led_Inverse_Order[led8] ) | 0x08; //显示湿度标志
	Lcd_Write_Data[LED8_LOW ] = Get_LedLow ( Led_Inverse_Order[led8] );
	Lcd_Write_Data[LED9_HIGH] = Get_LedHigh( Led_Inverse_Order[led9] );
	Lcd_Write_Data[LED9_LOW ] = Get_LedLow ( Led_Inverse_Order[led9] );
	Lcd_Write_Data[LED10_HIGH] = Get_LedHigh( Led_Inverse_Order[led10] ) | 0x08; //或上0x08是将湿度的小数点点亮
	Lcd_Write_Data[LED10_LOW ] = Get_LedLow ( Led_Inverse_Order[led10] );
	os_dly_wait(100);
}
/*pm2.5显示*/
void Disp_PM(unsigned int Pm)
{
	PM_ON;
	if(Pm <= PM1_LIMI) //空气PM2.5质量优
	{
//		PM1_ON;
		PM_State = 1;
	}
		else if((Pm > PM1_LIMI)&&(Pm <= (PM2_LIMI - 1)))		//空气PM2.5质量中
		{
//			PM2_ON;
			PM_State = 2;
		}
		else if(Pm > PM2_LIMI)		//空气PM2.5质量差
		{
//			PM3_ON;
			PM_State = 3;
		}
		else
		{
			PM_State = 0;
		//	PM0_CLR;					//清除PM2.5显示
		}
// 		if(PMState_Prev < PM_State)
// 		{
// 			
// 			PMState_Prev = PM_State;
// 		}
// 		else if(PMState_Prev > PM_State)
// 		{
// 			
// 		}
// 		else if(PMState_Prev > PM_State)
// 		{
// 			
// 		}
// 		else
// 		{}
}
/*异味显示*/
void Disp_TVOC(unsigned int Tvoc)
{
	TVOC_ON;
	if(Tvoc >= TVOC1_LIMI) //空气TVOC质量优
	{
		TVOC1_ON;
		TVOC_State = 1;
	}
		else if((Tvoc < (TVOC1_LIMI - 1))&&(Tvoc >= TVOC2_LIMI))		//空气TVOC质量中
		{
			TVOC2_ON;
			TVOC_State = 2;
		}
		else if(Tvoc < (TVOC2_LIMI - 1))		//空气TVOC质量差
		{
			TVOC3_ON;
			TVOC_State = 3;
		}	
		else
		{
			TVOC_State = 0;
// 			TVOC0_CLR;				//清除TVOC显示
		}
}
/*****************************end of file****************/

