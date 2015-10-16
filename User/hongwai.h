#ifndef		__HONGWAI_H__
#define		__HONGWAI_H__

#include "M051Series.h"
#include "RTL.h"

#define HONGWAI		P32

void Tim_Config(void);	//定时器配置  用于测量红外高电平的时间
void HWGPIO_Config(void);	//红外接收GPIO配置
#endif		/*END __HONGWAI_H__*/

