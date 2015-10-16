#ifndef		__LCD_H__
#define		__LCD_H__

#include "M051Series.h"
#include "RTL.h"
#include "Key_Board.h"

#define NOP()		delay()
//#define NOP()			os_dly_wait(1)
/*LCD 端口定义*/
#define PORT_LCD		P0	
/*LCD 引脚定义*/
#define HT1621_CS 		P04   			 
#define HT1621_WR 		P05					
#define HT1621_DAT		P06
#define HT1621_CS_BIT	BIT4
#define HT1621_WR_BIT	BIT5
#define HT1621_DAT_BIT	BIT6
/*电平高低*/
#define HIGH				1
#define LOW					0
/*数码管映射管脚*/
#define LED1_HIGH			24
#define LED1_LOW			25
#define LED2_HIGH			26
#define LED2_LOW			27
#define LED3_HIGH			28
#define LED3_LOW			7
#define LED4_HIGH			5
#define LED4_LOW			6
#define LED5_HIGH			12
#define LED5_LOW			13
#define LED6_HIGH			14
#define LED6_LOW			15
#define LED7_HIGH			16
#define LED7_LOW			17
#define LED8_HIGH			18
#define LED8_LOW			19
#define LED9_HIGH			20
#define LED9_LOW			21
#define LED10_HIGH			22
#define LED10_LOW			23


/*背光灯控制*/
#define LED_ON			P44 = 1;P26 = 0
#define	LED_OFF			P44 = 0;P26 = 1
#define LED				P44
/*蜂鸣器控制*/
#define BUZZER_ON		P37 = 1;os_dly_wait(80);P37 = 0		//蜂鸣器短响1声
#define BUZZER_OFF		P37 = 0

/*倒计时冒号闪烁*/	
#define COL_TWINKLE			Lcd_Write_Data[28]	^= 0x08
/*倒计时冒号关闭*/
#define	COL_OFF				Lcd_Write_Data[28]	&= 0xf7
/*各种标志位显示*/
#define ALLKEY_ON			Lcd_Write_Data[3]	|= 0x0c;Lcd_Write_Data[4] |= 0x0f	//显示所有按键标志
#define ALLKEY_OFF			Lcd_Write_Data[3]	&= 0xf3;Lcd_Write_Data[4] &= 0xf0	//关闭所有按键标志
#define KEY_MODEON			Lcd_Write_Data[4]	|= 0x02														//开启模式按键标志
#define	KEY_MODEOFF			Lcd_Write_Data[4]	&= 0x0d														//关闭模式按键标志
#define	KEY_POWERON			Lcd_Write_Data[4]	|= 0x01														//开启开关按键标志
#define	KEY_POWEROFF		Lcd_Write_Data[4]	&= 0x0e														//关闭开关按键标志
#define	WIND_ON				Lcd_Write_Data[0]	|= 0x0f;Lcd_Write_Data[8] |= 0x01;\
							Lcd_Write_Data[9]	|= 0x01;Lcd_Write_Data[6] |= 0x08	//显示风量标志
#define	WIND_OFF			Lcd_Write_Data[0]	&= 0x00;Lcd_Write_Data[8] &= 0x0e;\
							Lcd_Write_Data[9]	&= 0x0e;Lcd_Write_Data[6] &= 0x07	//关闭风量标志
#define PHST_ON				Lcd_Write_Data[3]	|= 0x02														//显示光触媒标志
#define PHST_OFF			Lcd_Write_Data[3]	&= 0xfd														//关闭光触媒标志
#define DISI_ON				Lcd_Write_Data[14]	|= 0x08;Lcd_Write_Data[3] |= 0x01	//显示消毒标志
#define DISI_OFF			Lcd_Write_Data[14]	&= 0xf7;Lcd_Write_Data[3] &= 0xfe	//关闭消毒标志
#define MODE_ON				Lcd_Write_Data[2]	|= 0X07;Lcd_Write_Data[1] |= 0x0f	//显示模式标志
#define MODE_OFF			Lcd_Write_Data[2]	&= 0xf0;Lcd_Write_Data[1] &= 0x00  //关闭模式标志
#define MANUAL_ON			Lcd_Write_Data[2]	= 0x04;Lcd_Write_Data[1] &= 0xf0	//显示手动模式标志
//#define MANUAL_OFF		Lcd_Write_Data[2]	&= 0xf0;													//关闭手动模式标志
#define AUTO_ON				Lcd_Write_Data[1]	|= 0x0f;Lcd_Write_Data[2] &= 0xf8	//显示自动模式标志
//#define AUTO_OFF			Lcd_Write_Data[1]	&=	0x00;													//关闭自动模式标志
#define TIME_ON				Lcd_Write_Data[24]	|= 0x08;Lcd_Write_Data[26]|= 0x08	//显示剩余时间标志
#define TIME_OFF			Lcd_Write_Data[24]	&= 0xf7;Lcd_Write_Data[26]&= 0xf7	//关闭剩余时间标志
#define TEMP_ON				Lcd_Write_Data[12]	|= 0x08														//显示温度标志
#define TEMP_OFF			Lcd_Write_Data[12]	&= 0xf7														//关闭温度标志
#define HUMI_ON				Lcd_Write_Data[18]	|= 0x08														//显示湿度标志
#define	HUMI_OFF			Lcd_Write_Data[18]	&= 0xf7														//关闭湿度标志
#define PM_ON				Lcd_Write_Data[9]	|= 0x02														//显示PM2.5标志
#define	PM_OFF				Lcd_Write_Data[9]	&= 0x0d														//关闭PM2.5标志
#define	TVOC_ON				Lcd_Write_Data[8]	|= 0x02														//显示粉尘标志
#define	TVOC_OFF			Lcd_Write_Data[8]	&= 0x0d														//关闭粉尘标志
/*关闭某一位数码管*/
#define	LED1_OFF			Lcd_Write_Data[24]	&= 0x08;Lcd_Write_Data[25] &= 0x00 	//关闭数码管1
#define	LED2_OFF			Lcd_Write_Data[26]	&= 0x08;Lcd_Write_Data[27] &= 0x00 	//关闭数码管2
#define	LED3_OFF			Lcd_Write_Data[28]	&= 0x00;Lcd_Write_Data[7 ] &= 0x00 	//关闭数码管3
#define	LED4_OFF			Lcd_Write_Data[5 ]	&= 0x00;Lcd_Write_Data[6 ] &= 0x08 	//关闭数码管4
#define	LED5_OFF			Lcd_Write_Data[12]	&= 0x08;Lcd_Write_Data[13] &= 0x00 	//关闭数码管5
#define	LED6_OFF			Lcd_Write_Data[14]	&= 0x08;Lcd_Write_Data[15] &= 0x00 	//关闭数码管6
#define	LED7_OFF			Lcd_Write_Data[16]	&= 0x00;Lcd_Write_Data[17] &= 0x00 	//关闭数码管7
#define	LED8_OFF			Lcd_Write_Data[18]	&= 0x08;Lcd_Write_Data[19] &= 0x00 	//关闭数码管8
#define	LED9_OFF			Lcd_Write_Data[20]	&= 0x08;Lcd_Write_Data[21] &= 0x00 	//关闭数码管9
#define	LED10_OFF			Lcd_Write_Data[22]	&= 0x00;Lcd_Write_Data[23] &= 0x00 	//关闭数码管10

/*显示某一位数码管*/
#define LED1_ON				Lcd_Write_Data[24]	|= 0x07;Lcd_Write_Data[25] |= 0x0f 	//显示数码管1
#define LED2_ON				Lcd_Write_Data[26]	|= 0x07;Lcd_Write_Data[27] |= 0x0f 	//显示数码管2
#define	LED3_ON				Lcd_Write_Data[28]	|= 0x07;Lcd_Write_Data[7 ] |= 0x0f 	//显示数码管3
#define LED4_ON				Lcd_Write_Data[5 ]	|= 0x0f;Lcd_Write_Data[6 ] |= 0x07 	//显示数码管1
#define LED5_ON				Lcd_Write_Data[12]	|= 0x07;Lcd_Write_Data[13] |= 0x0f 	//显示数码管2
#define	LED6_ON				Lcd_Write_Data[14]	|= 0x07;Lcd_Write_Data[15] |= 0x0f 	//显示数码管3
#define LED7_ON				Lcd_Write_Data[16]	|= 0x07;Lcd_Write_Data[17] |= 0x0f 	//显示数码管1
#define LED8_ON				Lcd_Write_Data[18]	|= 0x07;Lcd_Write_Data[19] |= 0x0f 	//显示数码管2
#define	LED9_ON				Lcd_Write_Data[20]	|= 0x07;Lcd_Write_Data[21] |= 0x0f 	//显示数码管3
#define	LED10_ON			Lcd_Write_Data[22]	|= 0x07;Lcd_Write_Data[23] |= 0x0f 	//关闭数码管10
/*显示第x片扇叶和光柱*/
#define WIND1				Lcd_Write_Data[0] 	= 0x03;Lcd_Write_Data[6] |= 0x08	//显示第1片扇叶和最里面的方框
#define WIND2				Lcd_Write_Data[0] 	= 0x05;Lcd_Write_Data[9] |= 0x01	//显示第2片扇叶和第二个光柱
#define WIND3				Lcd_Write_Data[0] 	= 0x09;Lcd_Write_Data[8] |= 0x01	//显示第3片扇叶和第三个光柱
#define	CLR_WIND			Lcd_Write_Data[9]  &= 0x0e;Lcd_Write_Data[8] &= 0x0e	//清除风量光柱显示

/*显示第x片扇叶*/
#define FAN1				Lcd_Write_Data[0] 	= 0x03														//只显示第一片扇叶
#define FAN2				Lcd_Write_Data[0] 	= 0x05														//只显示第二片扇叶
#define FAN3				Lcd_Write_Data[0] 	= 0x09														//只显示第三片扇叶
/*显示第x个光柱*/
#define FANPOST1			Lcd_Write_Data[6] |= 0x08;Lcd_Write_Data[9]  &= 0x0e;\
							Lcd_Write_Data[8] &= 0x0e								//显示最里面的方框
#define FANPOST2			Lcd_Write_Data[9] |= 0x01;Lcd_Write_Data[8] &= 0x0e		//显示第二个光柱
#define FANPOST3			Lcd_Write_Data[8] |= 0x01								//只显示第三片扇叶
/*显示空气质量：PM2.5和TVOC*/
#define	PM0_CLR				Lcd_Write_Data[9]  &= 0x03;Lcd_Write_Data[11]  = 0x00	//清除PM2.5显示
#define	PM1_ON				Lcd_Write_Data[9]  |= 0x0c;Lcd_Write_Data[11]  = 0x00	//显示PM2.5质量优
#define	PM2_ON				Lcd_Write_Data[9]  &= 0x03;Lcd_Write_Data[11]  = 0x0c 	//显示PM2.5质量中
#define	PM3_ON				Lcd_Write_Data[9]  &= 0x03;Lcd_Write_Data[11]  = 0x03 	//显示PM2.5质量差
#define PMPOST_ON			Lcd_Write_Data[9]  |= 0x0c;Lcd_Write_Data[11]  = 0x0f	//显示PM2.5所有光柱
#define PMPOST_OFF			Lcd_Write_Data[9]  &= 0x03;Lcd_Write_Data[11]  = 0x00	//关闭PM2.5所有光柱
// #define	PM4_ON			Lcd_Write_Data[9]  |= 0x0c;Lcd_Write_Data[11]  = 0x0c 	//显示PM2.5质量中-
// #define	PM5_ON			Lcd_Write_Data[9]  |= 0x0c;Lcd_Write_Data[11]  = 0x0e	//显示PM2.5质量差+
// #define	PM6_ON			Lcd_Write_Data[9]  |= 0x0c;Lcd_Write_Data[11]  = 0x0f	//显示PM2.5质量差-
#define	TVOC0_CLR			Lcd_Write_Data[8]  &= 0x03;Lcd_Write_Data[10]  = 0x00	//清除TVOC显示
#define	TVOC1_ON			Lcd_Write_Data[8]  |= 0x0c;Lcd_Write_Data[10]  = 0x00	//显示TVOC质量优
#define	TVOC2_ON			Lcd_Write_Data[8]  &= 0x03;Lcd_Write_Data[10]  = 0x0c	//显示TVOC质量中
#define	TVOC3_ON			Lcd_Write_Data[8]  &= 0x03;Lcd_Write_Data[10]  = 0x03 	//显示TVOC质量差
#define TVOCPOST_ON			Lcd_Write_Data[8]  |= 0x0c;Lcd_Write_Data[10]  = 0x0f	//显示TVOC所有光柱
#define TVOCPOST_OFF		Lcd_Write_Data[8]  &= 0x03;Lcd_Write_Data[10]  = 0x00	//关闭TVOC所有光柱
// #define	TVOC4_ON		Lcd_Write_Data[8]  |= 0x0c;Lcd_Write_Data[10]  = 0x0c	//显示TVOC质量中-
// #define	TVOC5_ON		Lcd_Write_Data[8]  |= 0x0c;Lcd_Write_Data[10]  = 0x0e	//显示TVOC质量差+
// #define	TVOC6_ON		Lcd_Write_Data[8]  |= 0x0c;Lcd_Write_Data[10]  = 0x0f;	//显示TVOC质量差-

/*显示第几瓣自动模式圆弧*/
#define AutoMode1			Lcd_Write_Data[1] = 0x08;Lcd_Write_Data[2] = 0x03		//显示第1段圆弧
#define AutoMode2			Lcd_Write_Data[1] = 0x04;Lcd_Write_Data[2] = 0x03		//显示第2段圆弧
#define AutoMode3			Lcd_Write_Data[1] = 0x02;Lcd_Write_Data[2] = 0x03		//显示第3段圆弧
#define AutoMode4			Lcd_Write_Data[1] = 0x01;Lcd_Write_Data[2] = 0x03		//显示第4段圆弧

#define AutoMode12			Lcd_Write_Data[1] = 0x0c;Lcd_Write_Data[2] = 0x03		//显示第1、2、3段圆弧
#define AutoMode23			Lcd_Write_Data[1] = 0x06;Lcd_Write_Data[2] = 0x03  	//显示第2、3、4段圆弧
#define AutoMode34			Lcd_Write_Data[1] = 0x03;Lcd_Write_Data[2] = 0x03		//显示第3、4、1段圆弧
#define AutoMode41			Lcd_Write_Data[1] = 0x09;Lcd_Write_Data[2] = 0x03		//显示第4、1、2段圆弧

#define AutoMode123			Lcd_Write_Data[1] = 0x0e;Lcd_Write_Data[2] = 0x03		//显示第1、2、3段圆弧
#define AutoMode234			Lcd_Write_Data[1] = 0x07;Lcd_Write_Data[2] = 0x03  	//显示第2、3、4段圆弧
#define AutoMode341			Lcd_Write_Data[1] = 0x0b;Lcd_Write_Data[2] = 0x03		//显示第3、4、1段圆弧
#define AutoMode412			Lcd_Write_Data[1] = 0x0d;Lcd_Write_Data[2] = 0x03		//显示第4、1、2段圆弧
/*PM2.5质量阈值*/
#define	PM1_LIMI			13
#define	PM2_LIMI			25
/*TVOC质量阈值*/
#define	TVOC1_LIMI		35
#define	TVOC2_LIMI		29
/*液晶命令*/
#define  BIAS     0x52             //0b1000 0101 0010  1/3duty 4com
#define  SYSDIS   0X00             //0b1000 0000 0000  关振系统荡器和LCD偏压发生器
#define  SYSEN    0X02             //0b1000 0000 0010 打开系统振荡器
#define  LCDOFF   0X04             //0b1000 0000 0100  关LCD偏压
#define  LCDON    0X06             //0b1000 0000 0110  打开LCD偏压
#define  XTAL     0x28             //0b1000 0010 1000 外部接时钟
#define  RC256    0X30             //0b1000 0011 0000  内部时钟
#define  TONEON   0X12             //0b1000 0001 0010  打开声音输出
#define  TONEOFF  0X10             //0b1000 0001 0000 关闭声音输出
#define  WDTDIS1   0X0A        			//0b1000 0000 1010  禁止看门狗


/*
 * ======== variables declaration ========
 */


extern unsigned char Lcd_Write_Data[32]; 
extern unsigned int Count_Down;
extern unsigned char Minute_Flag0;
extern unsigned char CountDown_Flag;
extern unsigned char Second_Flag0;
extern unsigned char Disi_Flag;
extern unsigned char Key_Power_State;	//电源功能状态位 0、关闭 1、开启
extern volatile char PM_State;				//pm2.5传感器输出等级状态位 1.优 2.中 3.差
extern volatile char TVOC_State;			//TVOC传感器输出等级状态位 1.优 2.中 3.差
/*
 * ======== Functions declaration ========
 */

void LCD_Init(void);
void LCD_WrCmd(unsigned char Cmd);
void LCD_WrOneData(unsigned char Addr, unsigned char Data);
void LCD_WrData(unsigned char Data, unsigned char cnt);
void LCD_WrByteData(unsigned char address, unsigned char what);
void LCD_WrAllData(void);
void LCD_Oper(unsigned char *pBuffer);
void delay(void);
unsigned char Get_LedHigh(unsigned char Buffer);
unsigned char Get_LedLow(unsigned char Buffer);
void Disp_Temp(unsigned int Temp);
void Disp_Humi(unsigned int Humi);
void Disp_PM(unsigned int Pm);
void Disp_TVOC(unsigned int Tvoc);
void Disp_CountDown(void);
//void Col_Disp(void);			//显示倒计时的冒号

extern void Oper_Wind_High(void);
extern void Oper_Wind_Mid(void);
extern void Oper_Wind_Low(void);
#endif		/*__LCD_H__*/
/********end of file******************************************/
