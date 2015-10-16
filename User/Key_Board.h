#ifndef  __KEY_BOARD_H__
#define  __KEY_BOARD_H__

#include "M051Series.h"
#include "RTL.h"
#include "lcd.h"
#include "Time.h"
/*管脚定义*/
#define Key_Flag			P31
#define OUT_DA2				P01
#define OUT_DA1				P02
#define OUT_DA0				P03
/*继电器管脚定义*/
#define Relay1_ON     P22 = 1					//风扇低速电源开启
#define Relay2_ON     P23 = 1					//风扇中速电源开启
#define Relay3_ON     P24 = 1					//风扇高速电源开启
#define Relay4_ON     P40 = 1					//光触媒电源开启
#define Relay5_ON     P21 = 1					//等离子体电源开启

#define Relay1_OFF    P22 = 0					//风扇低速电源关闭
#define Relay2_OFF    P23 = 0					//风扇中速电源关闭
#define Relay3_OFF    P24 = 0					//风扇低速电源关闭
#define Relay4_OFF    P40 = 0					//光触媒电源关闭
#define Relay5_OFF    P21 = 0					//等离子体电源关闭
#define AllRelay_OFF	P21 = 0;P22 = 0;P23 = 0;P24 = 0;P40 = 0		//关闭所有继电器

#define CLOSE_ALLRELAY  P21 = 0;P22 = 0;P23 = 0;P24 = 0;P40 = 0 //关闭所有电源
/*定时时间*/
#define ONE_HOUR			60
#define TWO_HOUR			120
#define FOUR_HOUR			240
#define EIGHT_HOUR			480
#define TWELVE_HOUR			720
/*按键扫描返回的键值*/
#define KEY_WIND	0x18
#define KEY_PHST	0x10
#define KEY_DISI	0x00
#define KEY_PWR		0x01
#define KEY_MOD		0x04
#define KEY_TMR		0x02
#define KEY_NULL	0xff

/*************varialbes*******************************/
extern unsigned int Count0;
extern unsigned char Lcd_Write_Data[32] ;
extern unsigned char CountDown_Flag;				
extern unsigned int Count_Down;
extern unsigned char SelfTest_flag;//自检标志位 0、为正常状态1、为自检模式 2、为手动自动模式选择	
extern unsigned int wait;			//0.1s自增一次  
extern volatile char PM_State;
extern unsigned char FiveMinute5_Flag;

//extern unsigned char OPER_Flag;
//extern uint8_t Sleep_Flag;

/*************functions**************************/
void KeyBoard_Init(void);
void Relay_Init(void);
char KeyBoard_Scan(void);

void Set_Wind(void);
void Set_Phst(void);
void Set_Disi(void);
void Set_Time(void);
void Set_Mode(void);
void Set_Power(void);

void Oper_Wind_High(void);
void Oper_Wind_Mid(void);
void Oper_Wind_Low(void);

//void Wind_High(void);


void Oper_Phst_ON(void);
void Oper_Phst_OFF(void);

void Oper_Disi_ON(void);
void Oper_Disi_OFF(void);

void Oper_Time_0H(void);
void Oper_Time_1H(void);
void Oper_Time_2H(void);
void Oper_Time_4H(void);
void Oper_Time_8H(void);
void Oper_Time_12H(void);

void Oper_Mode_Manual(void);
void Oper_Mode_Auto(void);

void Oper_Power_ON(void);
void Oper_Power_OFF(void);

void AutoMode_Disp(void);
void Wind_Disp(void);

void SelfTest(void);

void AllFlag_Clr(void);
void LEDS_OFF(void);							//关闭所有数码管
void LEDS_ON(void);								//显示所有数码管
void Symbols_ON(void);					//显示所有标志
void Symbols_OFF(void);					//显示所有标志
extern void Ht1621WrAllData(void);
extern void LCD_Oper(unsigned char *pBuffer);



#endif /*__KEY_BOARD_H__*/
