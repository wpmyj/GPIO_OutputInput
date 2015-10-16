#ifndef		__LCD_H__
#define		__LCD_H__

#include "M051Series.h"
#include "RTL.h"
#include "Key_Board.h"

#define NOP()		delay()
//#define NOP()			os_dly_wait(1)
/*LCD �˿ڶ���*/
#define PORT_LCD		P0	
/*LCD ���Ŷ���*/
#define HT1621_CS 		P04   			 
#define HT1621_WR 		P05					
#define HT1621_DAT		P06
#define HT1621_CS_BIT	BIT4
#define HT1621_WR_BIT	BIT5
#define HT1621_DAT_BIT	BIT6
/*��ƽ�ߵ�*/
#define HIGH				1
#define LOW					0
/*�����ӳ��ܽ�*/
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


/*����ƿ���*/
#define LED_ON			P44 = 1;P26 = 0
#define	LED_OFF			P44 = 0;P26 = 1
#define LED				P44
/*����������*/
#define BUZZER_ON		P37 = 1;os_dly_wait(80);P37 = 0		//����������1��
#define BUZZER_OFF		P37 = 0

/*����ʱð����˸*/	
#define COL_TWINKLE			Lcd_Write_Data[28]	^= 0x08
/*����ʱð�Źر�*/
#define	COL_OFF				Lcd_Write_Data[28]	&= 0xf7
/*���ֱ�־λ��ʾ*/
#define ALLKEY_ON			Lcd_Write_Data[3]	|= 0x0c;Lcd_Write_Data[4] |= 0x0f	//��ʾ���а�����־
#define ALLKEY_OFF			Lcd_Write_Data[3]	&= 0xf3;Lcd_Write_Data[4] &= 0xf0	//�ر����а�����־
#define KEY_MODEON			Lcd_Write_Data[4]	|= 0x02														//����ģʽ������־
#define	KEY_MODEOFF			Lcd_Write_Data[4]	&= 0x0d														//�ر�ģʽ������־
#define	KEY_POWERON			Lcd_Write_Data[4]	|= 0x01														//�������ذ�����־
#define	KEY_POWEROFF		Lcd_Write_Data[4]	&= 0x0e														//�رտ��ذ�����־
#define	WIND_ON				Lcd_Write_Data[0]	|= 0x0f;Lcd_Write_Data[8] |= 0x01;\
							Lcd_Write_Data[9]	|= 0x01;Lcd_Write_Data[6] |= 0x08	//��ʾ������־
#define	WIND_OFF			Lcd_Write_Data[0]	&= 0x00;Lcd_Write_Data[8] &= 0x0e;\
							Lcd_Write_Data[9]	&= 0x0e;Lcd_Write_Data[6] &= 0x07	//�رշ�����־
#define PHST_ON				Lcd_Write_Data[3]	|= 0x02														//��ʾ�ⴥý��־
#define PHST_OFF			Lcd_Write_Data[3]	&= 0xfd														//�رչⴥý��־
#define DISI_ON				Lcd_Write_Data[14]	|= 0x08;Lcd_Write_Data[3] |= 0x01	//��ʾ������־
#define DISI_OFF			Lcd_Write_Data[14]	&= 0xf7;Lcd_Write_Data[3] &= 0xfe	//�ر�������־
#define MODE_ON				Lcd_Write_Data[2]	|= 0X07;Lcd_Write_Data[1] |= 0x0f	//��ʾģʽ��־
#define MODE_OFF			Lcd_Write_Data[2]	&= 0xf0;Lcd_Write_Data[1] &= 0x00  //�ر�ģʽ��־
#define MANUAL_ON			Lcd_Write_Data[2]	= 0x04;Lcd_Write_Data[1] &= 0xf0	//��ʾ�ֶ�ģʽ��־
//#define MANUAL_OFF		Lcd_Write_Data[2]	&= 0xf0;													//�ر��ֶ�ģʽ��־
#define AUTO_ON				Lcd_Write_Data[1]	|= 0x0f;Lcd_Write_Data[2] &= 0xf8	//��ʾ�Զ�ģʽ��־
//#define AUTO_OFF			Lcd_Write_Data[1]	&=	0x00;													//�ر��Զ�ģʽ��־
#define TIME_ON				Lcd_Write_Data[24]	|= 0x08;Lcd_Write_Data[26]|= 0x08	//��ʾʣ��ʱ���־
#define TIME_OFF			Lcd_Write_Data[24]	&= 0xf7;Lcd_Write_Data[26]&= 0xf7	//�ر�ʣ��ʱ���־
#define TEMP_ON				Lcd_Write_Data[12]	|= 0x08														//��ʾ�¶ȱ�־
#define TEMP_OFF			Lcd_Write_Data[12]	&= 0xf7														//�ر��¶ȱ�־
#define HUMI_ON				Lcd_Write_Data[18]	|= 0x08														//��ʾʪ�ȱ�־
#define	HUMI_OFF			Lcd_Write_Data[18]	&= 0xf7														//�ر�ʪ�ȱ�־
#define PM_ON				Lcd_Write_Data[9]	|= 0x02														//��ʾPM2.5��־
#define	PM_OFF				Lcd_Write_Data[9]	&= 0x0d														//�ر�PM2.5��־
#define	TVOC_ON				Lcd_Write_Data[8]	|= 0x02														//��ʾ�۳���־
#define	TVOC_OFF			Lcd_Write_Data[8]	&= 0x0d														//�رշ۳���־
/*�ر�ĳһλ�����*/
#define	LED1_OFF			Lcd_Write_Data[24]	&= 0x08;Lcd_Write_Data[25] &= 0x00 	//�ر������1
#define	LED2_OFF			Lcd_Write_Data[26]	&= 0x08;Lcd_Write_Data[27] &= 0x00 	//�ر������2
#define	LED3_OFF			Lcd_Write_Data[28]	&= 0x00;Lcd_Write_Data[7 ] &= 0x00 	//�ر������3
#define	LED4_OFF			Lcd_Write_Data[5 ]	&= 0x00;Lcd_Write_Data[6 ] &= 0x08 	//�ر������4
#define	LED5_OFF			Lcd_Write_Data[12]	&= 0x08;Lcd_Write_Data[13] &= 0x00 	//�ر������5
#define	LED6_OFF			Lcd_Write_Data[14]	&= 0x08;Lcd_Write_Data[15] &= 0x00 	//�ر������6
#define	LED7_OFF			Lcd_Write_Data[16]	&= 0x00;Lcd_Write_Data[17] &= 0x00 	//�ر������7
#define	LED8_OFF			Lcd_Write_Data[18]	&= 0x08;Lcd_Write_Data[19] &= 0x00 	//�ر������8
#define	LED9_OFF			Lcd_Write_Data[20]	&= 0x08;Lcd_Write_Data[21] &= 0x00 	//�ر������9
#define	LED10_OFF			Lcd_Write_Data[22]	&= 0x00;Lcd_Write_Data[23] &= 0x00 	//�ر������10

/*��ʾĳһλ�����*/
#define LED1_ON				Lcd_Write_Data[24]	|= 0x07;Lcd_Write_Data[25] |= 0x0f 	//��ʾ�����1
#define LED2_ON				Lcd_Write_Data[26]	|= 0x07;Lcd_Write_Data[27] |= 0x0f 	//��ʾ�����2
#define	LED3_ON				Lcd_Write_Data[28]	|= 0x07;Lcd_Write_Data[7 ] |= 0x0f 	//��ʾ�����3
#define LED4_ON				Lcd_Write_Data[5 ]	|= 0x0f;Lcd_Write_Data[6 ] |= 0x07 	//��ʾ�����1
#define LED5_ON				Lcd_Write_Data[12]	|= 0x07;Lcd_Write_Data[13] |= 0x0f 	//��ʾ�����2
#define	LED6_ON				Lcd_Write_Data[14]	|= 0x07;Lcd_Write_Data[15] |= 0x0f 	//��ʾ�����3
#define LED7_ON				Lcd_Write_Data[16]	|= 0x07;Lcd_Write_Data[17] |= 0x0f 	//��ʾ�����1
#define LED8_ON				Lcd_Write_Data[18]	|= 0x07;Lcd_Write_Data[19] |= 0x0f 	//��ʾ�����2
#define	LED9_ON				Lcd_Write_Data[20]	|= 0x07;Lcd_Write_Data[21] |= 0x0f 	//��ʾ�����3
#define	LED10_ON			Lcd_Write_Data[22]	|= 0x07;Lcd_Write_Data[23] |= 0x0f 	//�ر������10
/*��ʾ��xƬ��Ҷ�͹���*/
#define WIND1				Lcd_Write_Data[0] 	= 0x03;Lcd_Write_Data[6] |= 0x08	//��ʾ��1Ƭ��Ҷ��������ķ���
#define WIND2				Lcd_Write_Data[0] 	= 0x05;Lcd_Write_Data[9] |= 0x01	//��ʾ��2Ƭ��Ҷ�͵ڶ�������
#define WIND3				Lcd_Write_Data[0] 	= 0x09;Lcd_Write_Data[8] |= 0x01	//��ʾ��3Ƭ��Ҷ�͵���������
#define	CLR_WIND			Lcd_Write_Data[9]  &= 0x0e;Lcd_Write_Data[8] &= 0x0e	//�������������ʾ

/*��ʾ��xƬ��Ҷ*/
#define FAN1				Lcd_Write_Data[0] 	= 0x03														//ֻ��ʾ��һƬ��Ҷ
#define FAN2				Lcd_Write_Data[0] 	= 0x05														//ֻ��ʾ�ڶ�Ƭ��Ҷ
#define FAN3				Lcd_Write_Data[0] 	= 0x09														//ֻ��ʾ����Ƭ��Ҷ
/*��ʾ��x������*/
#define FANPOST1			Lcd_Write_Data[6] |= 0x08;Lcd_Write_Data[9]  &= 0x0e;\
							Lcd_Write_Data[8] &= 0x0e								//��ʾ������ķ���
#define FANPOST2			Lcd_Write_Data[9] |= 0x01;Lcd_Write_Data[8] &= 0x0e		//��ʾ�ڶ�������
#define FANPOST3			Lcd_Write_Data[8] |= 0x01								//ֻ��ʾ����Ƭ��Ҷ
/*��ʾ����������PM2.5��TVOC*/
#define	PM0_CLR				Lcd_Write_Data[9]  &= 0x03;Lcd_Write_Data[11]  = 0x00	//���PM2.5��ʾ
#define	PM1_ON				Lcd_Write_Data[9]  |= 0x0c;Lcd_Write_Data[11]  = 0x00	//��ʾPM2.5������
#define	PM2_ON				Lcd_Write_Data[9]  &= 0x03;Lcd_Write_Data[11]  = 0x0c 	//��ʾPM2.5������
#define	PM3_ON				Lcd_Write_Data[9]  &= 0x03;Lcd_Write_Data[11]  = 0x03 	//��ʾPM2.5������
#define PMPOST_ON			Lcd_Write_Data[9]  |= 0x0c;Lcd_Write_Data[11]  = 0x0f	//��ʾPM2.5���й���
#define PMPOST_OFF			Lcd_Write_Data[9]  &= 0x03;Lcd_Write_Data[11]  = 0x00	//�ر�PM2.5���й���
// #define	PM4_ON			Lcd_Write_Data[9]  |= 0x0c;Lcd_Write_Data[11]  = 0x0c 	//��ʾPM2.5������-
// #define	PM5_ON			Lcd_Write_Data[9]  |= 0x0c;Lcd_Write_Data[11]  = 0x0e	//��ʾPM2.5������+
// #define	PM6_ON			Lcd_Write_Data[9]  |= 0x0c;Lcd_Write_Data[11]  = 0x0f	//��ʾPM2.5������-
#define	TVOC0_CLR			Lcd_Write_Data[8]  &= 0x03;Lcd_Write_Data[10]  = 0x00	//���TVOC��ʾ
#define	TVOC1_ON			Lcd_Write_Data[8]  |= 0x0c;Lcd_Write_Data[10]  = 0x00	//��ʾTVOC������
#define	TVOC2_ON			Lcd_Write_Data[8]  &= 0x03;Lcd_Write_Data[10]  = 0x0c	//��ʾTVOC������
#define	TVOC3_ON			Lcd_Write_Data[8]  &= 0x03;Lcd_Write_Data[10]  = 0x03 	//��ʾTVOC������
#define TVOCPOST_ON			Lcd_Write_Data[8]  |= 0x0c;Lcd_Write_Data[10]  = 0x0f	//��ʾTVOC���й���
#define TVOCPOST_OFF		Lcd_Write_Data[8]  &= 0x03;Lcd_Write_Data[10]  = 0x00	//�ر�TVOC���й���
// #define	TVOC4_ON		Lcd_Write_Data[8]  |= 0x0c;Lcd_Write_Data[10]  = 0x0c	//��ʾTVOC������-
// #define	TVOC5_ON		Lcd_Write_Data[8]  |= 0x0c;Lcd_Write_Data[10]  = 0x0e	//��ʾTVOC������+
// #define	TVOC6_ON		Lcd_Write_Data[8]  |= 0x0c;Lcd_Write_Data[10]  = 0x0f;	//��ʾTVOC������-

/*��ʾ�ڼ����Զ�ģʽԲ��*/
#define AutoMode1			Lcd_Write_Data[1] = 0x08;Lcd_Write_Data[2] = 0x03		//��ʾ��1��Բ��
#define AutoMode2			Lcd_Write_Data[1] = 0x04;Lcd_Write_Data[2] = 0x03		//��ʾ��2��Բ��
#define AutoMode3			Lcd_Write_Data[1] = 0x02;Lcd_Write_Data[2] = 0x03		//��ʾ��3��Բ��
#define AutoMode4			Lcd_Write_Data[1] = 0x01;Lcd_Write_Data[2] = 0x03		//��ʾ��4��Բ��

#define AutoMode12			Lcd_Write_Data[1] = 0x0c;Lcd_Write_Data[2] = 0x03		//��ʾ��1��2��3��Բ��
#define AutoMode23			Lcd_Write_Data[1] = 0x06;Lcd_Write_Data[2] = 0x03  	//��ʾ��2��3��4��Բ��
#define AutoMode34			Lcd_Write_Data[1] = 0x03;Lcd_Write_Data[2] = 0x03		//��ʾ��3��4��1��Բ��
#define AutoMode41			Lcd_Write_Data[1] = 0x09;Lcd_Write_Data[2] = 0x03		//��ʾ��4��1��2��Բ��

#define AutoMode123			Lcd_Write_Data[1] = 0x0e;Lcd_Write_Data[2] = 0x03		//��ʾ��1��2��3��Բ��
#define AutoMode234			Lcd_Write_Data[1] = 0x07;Lcd_Write_Data[2] = 0x03  	//��ʾ��2��3��4��Բ��
#define AutoMode341			Lcd_Write_Data[1] = 0x0b;Lcd_Write_Data[2] = 0x03		//��ʾ��3��4��1��Բ��
#define AutoMode412			Lcd_Write_Data[1] = 0x0d;Lcd_Write_Data[2] = 0x03		//��ʾ��4��1��2��Բ��
/*PM2.5������ֵ*/
#define	PM1_LIMI			13
#define	PM2_LIMI			25
/*TVOC������ֵ*/
#define	TVOC1_LIMI		35
#define	TVOC2_LIMI		29
/*Һ������*/
#define  BIAS     0x52             //0b1000 0101 0010  1/3duty 4com
#define  SYSDIS   0X00             //0b1000 0000 0000  ����ϵͳ������LCDƫѹ������
#define  SYSEN    0X02             //0b1000 0000 0010 ��ϵͳ����
#define  LCDOFF   0X04             //0b1000 0000 0100  ��LCDƫѹ
#define  LCDON    0X06             //0b1000 0000 0110  ��LCDƫѹ
#define  XTAL     0x28             //0b1000 0010 1000 �ⲿ��ʱ��
#define  RC256    0X30             //0b1000 0011 0000  �ڲ�ʱ��
#define  TONEON   0X12             //0b1000 0001 0010  ���������
#define  TONEOFF  0X10             //0b1000 0001 0000 �ر��������
#define  WDTDIS1   0X0A        			//0b1000 0000 1010  ��ֹ���Ź�


/*
 * ======== variables declaration ========
 */


extern unsigned char Lcd_Write_Data[32]; 
extern unsigned int Count_Down;
extern unsigned char Minute_Flag0;
extern unsigned char CountDown_Flag;
extern unsigned char Second_Flag0;
extern unsigned char Disi_Flag;
extern unsigned char Key_Power_State;	//��Դ����״̬λ 0���ر� 1������
extern volatile char PM_State;				//pm2.5����������ȼ�״̬λ 1.�� 2.�� 3.��
extern volatile char TVOC_State;			//TVOC����������ȼ�״̬λ 1.�� 2.�� 3.��
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
//void Col_Disp(void);			//��ʾ����ʱ��ð��

extern void Oper_Wind_High(void);
extern void Oper_Wind_Mid(void);
extern void Oper_Wind_Low(void);
#endif		/*__LCD_H__*/
/********end of file******************************************/
