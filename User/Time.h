#ifndef __TIME_H__
#define __TIME_H__

#include "M051Series.h"
#include "lcd.h"
#include "Sensor.h"

// #define TIME3_INIT Count3 = 1;Minute_Flag3 = 0		//���ڴ�����ʱ �а������������¼��� 
/**********variables***********************/
extern unsigned char Minute_Flag0;	//1���ӵ����־λ ���ڵ���ʱ
extern unsigned char Second_Flag0; //1���ӵ����־λ ���ڵ���ʱ
extern unsigned char CountDown_Flag;//����ʱִ�б�־λ
extern unsigned int Count0;

extern unsigned char SelfTest_flag;//�Լ��־λ 0��Ϊ����״̬1��Ϊ�Լ�ģʽ 2��Ϊ�ֶ��Զ�ģʽѡ�� 3�������Զ�ģʽ
extern unsigned int Count1;			//����0.1s�Լ�һ�� ���ڿ����Լ��ģʽѡ��ʱ��
extern unsigned char Second_Flag1; //1���ӵ����־λ ���ڿ����Լ��ģʽѡ��ʱ��

extern unsigned int Count2;		  //�����Զ�5���ӱ仯��ʱ
extern unsigned char FiveMinute_Flag; //5���Ӽ�ʱ����
extern unsigned char Count2_Flag;

extern unsigned char Minute_Flag3;	  //1����ʱ�䵽�� ���ڴ�������
extern unsigned int Count3;		  //����1���Ӽ�ʱ
extern unsigned char TVOCSample_Flag;	//��ζ������������־ 0���ȴ� 1��Ԥ�� 2�� ����


extern unsigned int Count4;		  //���ڹⴥý����1Сʱ���Զ��ر�
extern unsigned int Count5;		  //����5���Ӽ�ʱ�½���λ
extern unsigned char FiveMinute5_Flag;
extern unsigned char Count5_Flag;	//
extern unsigned char OPER_Flag;
extern unsigned char ManualMode_Flag;

extern unsigned char Disi_Flag;			//��������״̬λ 0���ر� 1������	�����µڶ�������ܺ͵����ӱ�־λ�ĳ�ͻ
extern unsigned char Phst_Flag;			//�ⴥý�Ĺ���״̬ 0�������� 1������

extern unsigned char Wind_High_Flag;		//���ٸ���ģʽ��־λ
extern unsigned char Wind_Mid_Flag;		//��������ģʽ��־λ
extern unsigned char Wind_Low_Flag;		//���ٵ���ģʽ��־λ
/**********fuctions***************/
void Time_Count0(void);						//����ʱ�ļ�ʱ����
void Time_Count1(void);						//�Լ��ģʽѡ���ʱ
void Time_Count2(void);						//�Զ�5���ӱ仯����״̬
void Time_Count3(void);						//���ڴ�����ʱ
void Time_Count4(void);						//�ⴥý����1Сʱ��ر�
void Time_Count5(void);						//��������1Сʱ��ر�
// void Time_Count6(void);						//1���Ӳɼ�����һ��
extern void Oper_Mode_Auto(void);	//�����Զ�ģʽ
extern void Oper_Mode_Manual(void);//�����ֶ�ģʽ
#endif /*__TIME_H__*/
/******************end of file**********************/
