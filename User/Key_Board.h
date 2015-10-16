#ifndef  __KEY_BOARD_H__
#define  __KEY_BOARD_H__

#include "M051Series.h"
#include "RTL.h"
#include "lcd.h"
#include "Time.h"
/*�ܽŶ���*/
#define Key_Flag			P31
#define OUT_DA2				P01
#define OUT_DA1				P02
#define OUT_DA0				P03
/*�̵����ܽŶ���*/
#define Relay1_ON     P22 = 1					//���ȵ��ٵ�Դ����
#define Relay2_ON     P23 = 1					//�������ٵ�Դ����
#define Relay3_ON     P24 = 1					//���ȸ��ٵ�Դ����
#define Relay4_ON     P40 = 1					//�ⴥý��Դ����
#define Relay5_ON     P21 = 1					//���������Դ����

#define Relay1_OFF    P22 = 0					//���ȵ��ٵ�Դ�ر�
#define Relay2_OFF    P23 = 0					//�������ٵ�Դ�ر�
#define Relay3_OFF    P24 = 0					//���ȵ��ٵ�Դ�ر�
#define Relay4_OFF    P40 = 0					//�ⴥý��Դ�ر�
#define Relay5_OFF    P21 = 0					//���������Դ�ر�
#define AllRelay_OFF	P21 = 0;P22 = 0;P23 = 0;P24 = 0;P40 = 0		//�ر����м̵���

#define CLOSE_ALLRELAY  P21 = 0;P22 = 0;P23 = 0;P24 = 0;P40 = 0 //�ر����е�Դ
/*��ʱʱ��*/
#define ONE_HOUR			60
#define TWO_HOUR			120
#define FOUR_HOUR			240
#define EIGHT_HOUR			480
#define TWELVE_HOUR			720
/*����ɨ�践�صļ�ֵ*/
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
extern unsigned char SelfTest_flag;//�Լ��־λ 0��Ϊ����״̬1��Ϊ�Լ�ģʽ 2��Ϊ�ֶ��Զ�ģʽѡ��	
extern unsigned int wait;			//0.1s����һ��  
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
void LEDS_OFF(void);							//�ر����������
void LEDS_ON(void);								//��ʾ���������
void Symbols_ON(void);					//��ʾ���б�־
void Symbols_OFF(void);					//��ʾ���б�־
extern void Ht1621WrAllData(void);
extern void LCD_Oper(unsigned char *pBuffer);



#endif /*__KEY_BOARD_H__*/
