#include "lcd.h"

/*��������ABCD��EFGǰ*/
const unsigned char Led_Normal_Order[10] = {0xf5,0x60,0xd3,0xf2,0x66,0xb6,0xb7,0xe0,0xf7,0xf6};
/*��������EFG��ABCDǰ*/
const unsigned char Led_Inverse_Order[10] = {0x5f,0x06,0x3d,0x2f,0x66,0x6b,0x7b,0x0e,0x7f,0x6f};

//volatile char PM_State = 0;

void delay(void)
{
	unsigned char i;
	for(i = 0; i < 200; i++);
}

/*lcd д��cnt������λ*/
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
		NOP();			 //�����ʱ����Ҫ  ���û�еĻ�������������ʾ
		Data <<= 1;
	}
}
/*lcdд�����*/
void LCD_WrCmd(unsigned char Cmd)
{
  HT1621_CS = LOW;//P0_OUT&=~HT1621_CS;
	LCD_WrData(0x80,4);          //д�������־100
	LCD_WrData(Cmd,8);                 //д����������
	HT1621_CS = HIGH;//P0_OUT |= HT1621_CS;
}
/*lcdд��λ����*/
void LCD_WrOneData(unsigned char Addr, unsigned char Data)
{
	HT1621_CS = LOW;//P0_OUT &= ~HT1621_CS;
	LCD_WrData(0xA0, 3);                 //д�����ݱ�־101
	LCD_WrData(Addr << 2, 6);          //д���ַ����
	LCD_WrData(Data << 4, 4);          //д������
	HT1621_CS = HIGH;//P0_OUT |= HT1621_CS;
}
/*����*/
void LCD_WrAllData(void)
{
	unsigned char i;
	for (i = 0; i < 32; i++)
	{
		Lcd_Write_Data[i] = 0x00;
	}
}
/*Һ����ʼ��*/
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
	os_dly_wait(1000);                         //��ʱʹLCD������ѹ�ȶ�
	LCD_WrCmd(BIAS);								//�ĸ�COM��1/3ƫѹ
	LCD_WrCmd(RC256);              //ʹ���ڲ�����
	LCD_WrCmd(SYSDIS);							
	LCD_WrCmd(WDTDIS1);
	LCD_WrCmd(SYSEN);
	LCD_WrCmd(LCDON);
}
/*lcdд��1���ֽڵ�����*/
void LCD_WrByteData(unsigned char address, unsigned char what)
{
	HT1621_CS = LOW;//P0_OUT &= ~HT1621_CS;
	LCD_WrData(0xA0, 3);          //д�����ݱ�־101
	LCD_WrData(address << 2, 6);         //д���ַ����
	LCD_WrData(what, 8);         //д������

	HT1621_CS = HIGH;//P0_OUT |= HT1621_CS;
}
/*LCDˢ������*/
void LCD_Oper(unsigned char *pBuffer)		
{
	char i;
	for(i = 0;i < 32; i++)
		{
			LCD_WrOneData(i,*pBuffer);
			pBuffer++;
		}
}
/*ȡ���������ʾ���ݵĸ���λ,���õĲ���Ϊ�������������ֶ���*/
unsigned char Get_LedHigh(unsigned char Buffer)	
{
	unsigned char High = 0;
	High = (Buffer >> 4) & 0x0f;
	return High;
}
/*ȡ���������ʾ���ݵĸ���λ,���õĲ���Ϊ�������������ֶ���*/
unsigned char Get_LedLow(unsigned char Buffer)
{
	unsigned char Low = 0;
	Low = Buffer & 0x0f;
	return Low;
}
/*��ʾ����ʱ*/
void Disp_CountDown(void)
{
	char led1,led2,led3,led4;//�洢����ʱת�����ÿһλ����
	if(1 == CountDown_Flag)  //
	{
		led1 = (unsigned char)((Count_Down / 600));												//����ʱʱ��ת����ʾ
		led2 = (unsigned char)(((Count_Down / 60) % 10));
		led3 = (unsigned char)(((Count_Down % 60) / 10));
		led4 = (unsigned char)(((Count_Down % 60) % 10));
		Lcd_Write_Data[LED1_HIGH] = Get_LedHigh( Led_Inverse_Order[led1] ) | 0x08;	//����0x08�ǽ���ʣ�ࡰ���ֵ���
		Lcd_Write_Data[LED1_LOW ] = Get_LedLow ( Led_Inverse_Order[led1] );
		Lcd_Write_Data[LED2_HIGH] = Get_LedHigh( Led_Inverse_Order[led2] ) | 0x08;	//����0x08�ǽ���ʱ�䡱���ֵ���
		Lcd_Write_Data[LED2_LOW ] = Get_LedLow ( Led_Inverse_Order[led2] );
		if(1 == Second_Flag0)			//һ��ʱ�䵽
		{
			Second_Flag0 = 0;
			Lcd_Write_Data[LED3_HIGH] = Get_LedHigh( Led_Inverse_Order[led3] );	//��ʾ����ʱð��
		}
		else
		{
			Lcd_Write_Data[LED3_HIGH] = Get_LedHigh( Led_Inverse_Order[led3] ) | 0x08;	//��ʾ����ʱð��
		}
		Lcd_Write_Data[LED3_LOW ] = Get_LedLow ( Led_Inverse_Order[led3] );
		Lcd_Write_Data[LED4_HIGH] = Get_LedHigh( Led_Normal_Order[led4] );
		Lcd_Write_Data[LED4_LOW ] = Get_LedLow ( Led_Normal_Order[led4] )  | 0x08; //����0x08�ǽ����ٵ��ٷ������
		if(1 == Minute_Flag0)		//1���ӵ�
		{			
			Minute_Flag0 = 0;
			if(Count_Down > 0)
				Count_Down--;
				else
				{
 					Count_Down = 0;
// 					CLOSE_ALLRELAY;			//�ر����е�Դ
// 					CountDown_Flag = 0;
// 					//Bide_Flag = 1;
// 					OPER_Flag = 0;
// 					Key_Power_State = 0;//��Դ�����ر�
// 					LCD_WrAllData();		//����
// 					LED_OFF;				//�رձ���
// 					KEY_POWERON;			//���ذ���ͼ����ʾ
					Oper_Power_OFF();
				}
		}
		else
		{}
	}
}
// void Col_Disp(void)			//��ʾ����ʱ��ð��
// {
// 	if(1 == OPER_Flag)
// 	{
// 		if(1 == CountDown_Flag)
// 		{
// 			if(1 == Second_Flag0)			//һ��ʱ�䵽
// 			{
// 				Second_Flag0 = 0;
// 				//COL_TWINKLE;							//ʱ��ָ�ð����˸
// 				COL_OFF;									//�ر�ð����ʾ
// 			}
// 			else
// 			{}
// 		}
// 		else if(0 == CountDown_Flag)
// 		{
// 			COL_OFF;									//�ر�ð����ʾ
// 		}
// 		else
// 		{}
// 	}
// 	else
// 	{}
// }

/*��ʾ�¶�    ��ֵ����ʮ֮�����*/
void Disp_Temp(unsigned int Temp)
{
	unsigned char led5,led6,led7;
	led5 = (unsigned char)((Temp / 100));												//�¶�ת��������ʾ
	led6 = (unsigned char)((Temp / 10) % 10);
	led7 = (unsigned char)(((Temp % 10)));
	Lcd_Write_Data[LED5_HIGH] = Get_LedHigh( Led_Inverse_Order[led5] ) | 0x08; //��ʾ�¶ȱ�־
	Lcd_Write_Data[LED5_LOW ] = Get_LedLow ( Led_Inverse_Order[led5] );
	/*����������ʱ����ʾ�����ӱ�־*/
	if(1 == Disi_Flag)																										
		Lcd_Write_Data[LED6_HIGH] = Get_LedHigh( Led_Inverse_Order[led6] ) | 0x08;	//����0x08�ǽ������ӱ�־����
		else if(0 == Disi_Flag)
			Lcd_Write_Data[LED6_HIGH] = Get_LedHigh( Led_Inverse_Order[led6] );
		else
		{}
	Lcd_Write_Data[LED6_LOW ] = Get_LedLow ( Led_Inverse_Order[led6] );
	Lcd_Write_Data[LED7_HIGH] = Get_LedHigh( Led_Inverse_Order[led7] ) | 0x08; //����0x08�ǽ��¶ȵ�С�������
	Lcd_Write_Data[LED7_LOW ] = Get_LedLow ( Led_Inverse_Order[led7] );
	os_dly_wait(100);
}
/*ʪ����ʾ		��ֵ����ʮ��ʹ��*/
void Disp_Humi(unsigned int Humi)
{
	unsigned char led8,led9,led10;
	led8 = (unsigned char)((Humi / 100));												//ʪ��ת����ʾ
	led9 = (unsigned char)((Humi / 10) % 10);
	led10 = (unsigned char)(((Humi % 10)));
	Lcd_Write_Data[LED8_HIGH] = Get_LedHigh( Led_Inverse_Order[led8] ) | 0x08; //��ʾʪ�ȱ�־
	Lcd_Write_Data[LED8_LOW ] = Get_LedLow ( Led_Inverse_Order[led8] );
	Lcd_Write_Data[LED9_HIGH] = Get_LedHigh( Led_Inverse_Order[led9] );
	Lcd_Write_Data[LED9_LOW ] = Get_LedLow ( Led_Inverse_Order[led9] );
	Lcd_Write_Data[LED10_HIGH] = Get_LedHigh( Led_Inverse_Order[led10] ) | 0x08; //����0x08�ǽ�ʪ�ȵ�С�������
	Lcd_Write_Data[LED10_LOW ] = Get_LedLow ( Led_Inverse_Order[led10] );
	os_dly_wait(100);
}
/*pm2.5��ʾ*/
void Disp_PM(unsigned int Pm)
{
	PM_ON;
	if(Pm <= PM1_LIMI) //����PM2.5������
	{
//		PM1_ON;
		PM_State = 1;
	}
		else if((Pm > PM1_LIMI)&&(Pm <= (PM2_LIMI - 1)))		//����PM2.5������
		{
//			PM2_ON;
			PM_State = 2;
		}
		else if(Pm > PM2_LIMI)		//����PM2.5������
		{
//			PM3_ON;
			PM_State = 3;
		}
		else
		{
			PM_State = 0;
		//	PM0_CLR;					//���PM2.5��ʾ
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
/*��ζ��ʾ*/
void Disp_TVOC(unsigned int Tvoc)
{
	TVOC_ON;
	if(Tvoc >= TVOC1_LIMI) //����TVOC������
	{
		TVOC1_ON;
		TVOC_State = 1;
	}
		else if((Tvoc < (TVOC1_LIMI - 1))&&(Tvoc >= TVOC2_LIMI))		//����TVOC������
		{
			TVOC2_ON;
			TVOC_State = 2;
		}
		else if(Tvoc < (TVOC2_LIMI - 1))		//����TVOC������
		{
			TVOC3_ON;
			TVOC_State = 3;
		}	
		else
		{
			TVOC_State = 0;
// 			TVOC0_CLR;				//���TVOC��ʾ
		}
}
/*****************************end of file****************/

