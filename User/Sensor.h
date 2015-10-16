#ifndef		__SENSOR_H__
#define		__SENSOR_H__

#include "M051Series.h"
#include "lcd.h"
#include "RTL.h"

#define AREF_VOLTAGE		5			//AD�ο���ѹ
#define FREQUENCY100		3749		//Ƶ��Ϊ100HZʱ�������Ĵ������õ�ֵ
#define DUTY32				119			//ռ�ձ�Ϊ3.2%ʱ�ȽϼĴ������õ�ֵ

#define PMLEDPOWER_ON		P20 = 0		//����PM2.5LED
#define	PMLEDPOWER_OFF		P20 = 1		//�ر�PM2.5LED

#define TVOC_Ctrl			P17

#define HIGH  1
#define LOW   0

#define SUCCESS 1
#define	ERROR		0

typedef struct
{
	uint8_t  humi_int;		//ʪ�ȵ���������
	uint8_t  humi_deci;	 	//ʪ�ȵ�С������
	uint8_t  temp_int;	 	//�¶ȵ���������
	uint8_t  temp_deci;	 	//�¶ȵ�С������
	uint8_t  check_sum;	 	//У���
		                 
}DHT11_Data_TypeDef;

//���κ꣬��������������һ��ʹ��,����ߵ�ƽ��͵�ƽ
#define DHT11_DATA_OUT(a)	if (a)	\
					P25 = 1;\
					else		\
					P25 = 0;
 //��ȡ���ŵĵ�ƽ
#define  DHT11_DATA_IN()	 P25
#define  DHT11_HIGH			 P25 = 1;		//����dht11�ܽ�
					
extern unsigned char FiveMinute_Flag; //5���Ӽ�ʱ����
extern unsigned char Count2_Flag;	//5���Ӽ�ʱ��ʼ
extern unsigned char FiveMinute5_Flag;
extern unsigned char Count5_Flag;
void ADC_Config(void);			//ADC��ʼ��
int ADC_Convert_PM(void);		//ACת������ʾ
int ADC_Convert_TVOC(void);		//ACת������ʾ
void Delay_us(unsigned n); 		//��ʱ����
void loop(void);
void Tvoc(void);				//��ζ����������ת������ʾ

void DHT11_GPIO_Config(void);
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
static uint8_t Read_Byte(void);
void PWM_Config(void);
// void PM_CWind(char State);
void PM_Disp(char State);
void TVOC_Disp(char State);
/*�Զ�ģʽ�� ����ͨ����������(��������õ�����)�ı�*/
void Auto_Wind(void);

extern void Disp_Temp(unsigned int Temp);
extern void Oper_Wind_High(void);
extern void Oper_Wind_Mid(void);
extern void Oper_Wind_Low(void);

#endif  /*__SENSOR_H__*/

