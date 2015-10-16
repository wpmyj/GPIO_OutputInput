#ifndef __TIME_H__
#define __TIME_H__

#include "M051Series.h"
#include "lcd.h"
#include "Sensor.h"

// #define TIME3_INIT Count3 = 1;Minute_Flag3 = 0		//用于待机计时 有按键按下则重新计数 
/**********variables***********************/
extern unsigned char Minute_Flag0;	//1分钟到达标志位 用于倒计时
extern unsigned char Second_Flag0; //1秒钟到达标志位 用于倒计时
extern unsigned char CountDown_Flag;//倒计时执行标志位
extern unsigned int Count0;

extern unsigned char SelfTest_flag;//自检标志位 0、为正常状态1、为自检模式 2、为手动自动模式选择 3、运行自动模式
extern unsigned int Count1;			//计数0.1s自加一次 用于开机自检和模式选择时间
extern unsigned char Second_Flag1; //1秒钟到达标志位 用于开机自检和模式选择时间

extern unsigned int Count2;		  //用于自动5分钟变化计时
extern unsigned char FiveMinute_Flag; //5分钟计时到达
extern unsigned char Count2_Flag;

extern unsigned char Minute_Flag3;	  //1分钟时间到达 用于待机操作
extern unsigned int Count3;		  //用于1分钟计时
extern unsigned char TVOCSample_Flag;	//异味传感器采样标志 0、等待 1、预热 2、 采样


extern unsigned int Count4;		  //用于光触媒运行1小时后自动关闭
extern unsigned int Count5;		  //用于5分钟计时下降档位
extern unsigned char FiveMinute5_Flag;
extern unsigned char Count5_Flag;	//
extern unsigned char OPER_Flag;
extern unsigned char ManualMode_Flag;

extern unsigned char Disi_Flag;			//消毒工作状态位 0、关闭 1、工作	用于温第二个数码管和等离子标志位的冲突
extern unsigned char Phst_Flag;			//光触媒的工作状态 0、不工作 1、工作

extern unsigned char Wind_High_Flag;		//风速高速模式标志位
extern unsigned char Wind_Mid_Flag;		//风速中速模式标志位
extern unsigned char Wind_Low_Flag;		//风速低速模式标志位
/**********fuctions***************/
void Time_Count0(void);						//倒计时的计时函数
void Time_Count1(void);						//自检和模式选择计时
void Time_Count2(void);						//自动5分钟变化风量状态
void Time_Count3(void);						//用于待机计时
void Time_Count4(void);						//光触媒运行1小时后关闭
void Time_Count5(void);						//消毒运行1小时后关闭
// void Time_Count6(void);						//1分钟采集数据一次
extern void Oper_Mode_Auto(void);	//运行自动模式
extern void Oper_Mode_Manual(void);//运行手动模式
#endif /*__TIME_H__*/
/******************end of file**********************/
