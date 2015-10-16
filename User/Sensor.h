#ifndef		__SENSOR_H__
#define		__SENSOR_H__

#include "M051Series.h"
#include "lcd.h"
#include "RTL.h"

#define AREF_VOLTAGE		5			//AD参考电压
#define FREQUENCY100		3749		//频率为100HZ时计数器寄存器配置的值
#define DUTY32				119			//占空比为3.2%时比较寄存器配置的值

#define PMLEDPOWER_ON		P20 = 0		//点亮PM2.5LED
#define	PMLEDPOWER_OFF		P20 = 1		//关闭PM2.5LED

#define TVOC_Ctrl			P17

#define HIGH  1
#define LOW   0

#define SUCCESS 1
#define	ERROR		0

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
}DHT11_Data_TypeDef;

//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define DHT11_DATA_OUT(a)	if (a)	\
					P25 = 1;\
					else		\
					P25 = 0;
 //读取引脚的电平
#define  DHT11_DATA_IN()	 P25
#define  DHT11_HIGH			 P25 = 1;		//拉高dht11管脚
					
extern unsigned char FiveMinute_Flag; //5分钟计时到达
extern unsigned char Count2_Flag;	//5分钟计时开始
extern unsigned char FiveMinute5_Flag;
extern unsigned char Count5_Flag;
void ADC_Config(void);			//ADC初始化
int ADC_Convert_PM(void);		//AC转化并显示
int ADC_Convert_TVOC(void);		//AC转化并显示
void Delay_us(unsigned n); 		//延时函数
void loop(void);
void Tvoc(void);				//异味传感器数据转换并显示

void DHT11_GPIO_Config(void);
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
static uint8_t Read_Byte(void);
void PWM_Config(void);
// void PM_CWind(char State);
void PM_Disp(char State);
void TVOC_Disp(char State);
/*自动模式下 风速通过空气质量(传感器获得的数据)改变*/
void Auto_Wind(void);

extern void Disp_Temp(unsigned int Temp);
extern void Oper_Wind_High(void);
extern void Oper_Wind_Mid(void);
extern void Oper_Wind_Low(void);

#endif  /*__SENSOR_H__*/

