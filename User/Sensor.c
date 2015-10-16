#include "Sensor.h"

// int samplingTime = 280;			//采样延时单位us
// int deltaTime = 40;					//采样延时单位us
//int sleepTime = 9680;				//采样延时单位us
volatile char PM_State = 1;				//pm2.5传感器输出等级状态位 1.优 2.中 3.差
volatile char TVOC_State = 1;			//TVOC传感器输出等级状态位 1.优 2.中 3.差
unsigned int i32ConversionData = 0;
// unsigned int averpm_prev = 0;
/*------------------------------------ADC--------------------------------------------------*/
void ADC_Config(void)
{
	/* Set the ADC operation mode as continuous scan, input mode as single-end and
		enable the analog input channel 5and 6*/
 	ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_CONTINUOUS, 0x60);
//	ADC_Open(ADC, ADC_ADCR_DIFFEN_SINGLE_END, ADC_ADCR_ADMD_BURST, 0x60);
	/* Power on ADC module */
	ADC_POWER_ON(ADC);
	/* clear the A/D interrupt flag for safe */
	ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
	

}
/*******************************PM2.5传感器信号转换*********************************************/
int ADC_Convert_PM(void)		//ADC电压转换  
{
	unsigned int  Convert_Value = 0;
	
	/* start A/D conversion */
	ADC_START_CONV(ADC);

	/* Wait conversion done */
	while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));

	/* clear the A/D interrupt flag for safe */
	ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
	
	i32ConversionData = ADC_GET_CONVERSION_DATA(ADC,5);
 	Convert_Value = (int)(i32ConversionData * AREF_VOLTAGE * 10 / 4096);
	
	//Disp_Temp(Convert_Value);						//显示值是实际电压的10倍  使用温度数码管显示供参考
	return Convert_Value;		
}

/*********************异味传感器信号转换***************************************************/
int ADC_Convert_TVOC(void)
{
	unsigned int i32ConversionData = 0, Convert_Value = 0;
	
	/* start A/D conversion */
	ADC_START_CONV(ADC);

	/* Wait conversion done */
	while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));

	/* clear the A/D interrupt flag for safe */
	ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
	i32ConversionData = ADC_GET_CONVERSION_DATA(ADC,6);
 	Convert_Value = (int)(i32ConversionData * AREF_VOLTAGE * 10 / 4096);
	
	//Disp_Temp(Convert_Value);						//显示值是实际电压的10倍 使用温度数码管显示供参考
	return Convert_Value;
}
void Tvoc(void)				//异味传感器数据转换并显示
{
	int tvoc = 40;			//初始化第一次为优秀
	int	tvoc_sum = 0,tvoc_aver = 0,i,State;//
	TVOC_ON;
	//TVOC_Ctrl = 1;
// 	if(1 == TVOC_Ctrl)
// 	{
	for(i = 0;i < 2;i++)
	{
		tvoc =	ADC_Convert_TVOC();	//异味AD转化
		tvoc_sum = tvoc_sum + tvoc;
	}
	tvoc_aver = (int)(tvoc_sum / 2);
	
	if(tvoc_aver >= TVOC1_LIMI) //空气TVOC质量优
	{
		State = 1;
	}
		else if((tvoc_aver < (TVOC1_LIMI - 1))&&(tvoc_aver >= TVOC2_LIMI))		//空气TVOC质量中
		{
			State = 2;
		}
		else if(tvoc_aver < (TVOC2_LIMI - 1))		//空气TVOC质量差
		{
			State = 3;
		}	
		else
		{
			State = 0;
		}
	if(TVOC_State < State)
	{
		Count5 = 1;
		Count5_Flag = 0;
		TVOC_State = State;
		TVOC_Disp(TVOC_State);
	}
		else if(TVOC_State > State)
		{
			Count5_Flag = 1;
			if(1 == FiveMinute5_Flag)
			{
				FiveMinute5_Flag = 0;
				if(TVOC_State <= 1)
					TVOC_State = 1;
					else	
					TVOC_State = TVOC_State - 1;
				TVOC_Disp(TVOC_State);
			}
		}
// 		else if((TVOC_State > State)&&(2 == TVOCSample_Flag))
// 		{
// 			TVOCSample_Flag = 0;
// 			TVOC_State = TVOC_State - 1;
// 		}
		else if(TVOC_State == State)
		{
			Count5 = 1;
			Count5_Flag = 0;
			TVOC_Disp(TVOC_State);
		}
		else
		{}
		TVOC_Ctrl = 1;
//	Disp_TVOC(tvoc_aver);			//显示异味等级
// 	}
// 	TVOC_Ctrl ^= 1;
	
}
void loop(void)				//PM2.5传感器数据读取并显示
{
	int pm = 0,i,sumpm = 0,averpm = 0,State;	//,averpm_prev = 0
	int samplingTime = 280;				//采样延时单位us
	int deltaTime = 40;					//采样延时单位us
	for(i = 0;i < 100;i++)
	{
		PMLEDPOWER_ON;				//power on the LED
		Delay_us(samplingTime);		
		pm = ADC_Convert_PM();		//ADC转换通过温度三个数码管显示
		Delay_us(deltaTime);
		PMLEDPOWER_OFF;
		sumpm = sumpm + pm;
		//Delay_us(sleepTime);
		os_dly_wait(10);
	}
	averpm = (int)(sumpm / 100);
	if(averpm <= PM1_LIMI) //空气PM2.5质量优
	{
		State = 1;
	}
		else if((averpm > PM1_LIMI)&&(averpm <= (PM2_LIMI - 1)))	//空气PM2.5质量中
		{
			State = 2;
		}
		else if(averpm > PM2_LIMI)		//空气PM2.5质量差
		{
			State = 3;
		}
		else
		{
			State = 0;
		}
	if(PM_State < State)
	{
		Count2 = 1;
		Count2_Flag = 0;
		PM_State = State;
		//PM_CWind(PM_State);
		PM_Disp(PM_State);
	}
		else if(PM_State > State)
		{
			Count2_Flag = 1;
			if(1 == FiveMinute_Flag)
			{
				FiveMinute_Flag = 0;
				if(PM_State <= 1)
					PM_State = 1;
					else	
					PM_State = PM_State - 1;
			//	PM_CWind(PM_State);
				PM_Disp(PM_State);
			}
		}
		else if(PM_State == State)
		{
			//PM_CWind(PM_State);
			Count2 = 1;
			Count2_Flag = 0;
			PM_Disp(PM_State);
		}
		else
		{}
//	Disp_PM(averpm);				//显示PM2.5的状态
}
// void PM_CWind(char State)
// {
// 	if(1 == State)
// 	{
// 		PM1_ON;
// 		Oper_Wind_Low();
// 	}
// 		else if(2 == State)
// 		{
// 			PM2_ON;
// 			Oper_Wind_Mid();
// 		}
// 		else if(3 == State)
// 		{
// 			PM3_ON;
// 			Oper_Wind_High();
// 		}
// 		else
// 		{}
// }
void PM_Disp(char State)
{
	if(1 == State)
	{
		PM1_ON;
	}
		else if(2 == State)
		{
			PM2_ON;
		}
		else if(3 == State)
		{
			PM3_ON;
		}
		else
		{}
}
void TVOC_Disp(char State)
{
	if(1 == State)
	{
		TVOC1_ON;
	}
		else if(2 == State)
		{
			TVOC2_ON;
		}
		else if(3 == State)
		{
			TVOC3_ON;
		}
		else
		{}
}
void Delay_us(unsigned n)			//1us不精确延时
{
	unsigned int i,j;
	for(i = 0;i < n;i++)
		{
			for(j = 0;j < 10;j++);
		}
}
/*自动模式下 风速通过空气质量(传感器获得的数据)改变*/
void Auto_Wind(void)
{
	if(0 == ManualMode_Flag)		//自动模式下
	{
		if((1 == PM_State )&&(1 == TVOC_State))
			Oper_Wind_Low();
		else if(((2 == PM_State)&&(TVOC_State < 3))||((2 == TVOC_State)&&(PM_State < 3)))
			Oper_Wind_Mid();
		else if((3 == PM_State)||(3 == TVOC_State))
			Oper_Wind_High();
		else
		{}
	}
	else
	{}
}
/************************************DHT11***********************************/

/*
 * 函数名：DHT11_GPIO_Config
 * 描述  ：配置DHT11用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void DHT11_GPIO_Config(void)
{		 
	/* Configure P2.5 as Output mode */
	GPIO_SetMode(P2, BIT5, GPIO_PMD_OUTPUT);
	/* 拉高DHT11管脚	*/
	DHT11_HIGH;
}

/*
 * 函数名：DHT11_Mode_IPU
 * 描述  ：使DHT11-DATA引脚变为输入模式
 * 输入  ：无
 * 输出  ：无
 */
static void DHT11_Mode_IPU(void)
{
	/* Configure P2.7 as Output mode */
	GPIO_SetMode(P2, BIT5, GPIO_PMD_INPUT);
}
/*
 * 函数名：DHT11_Mode_Out_PP
 * 描述  ：使DHT11-DATA引脚变为输出模式
 * 输入  ：无
 * 输出  ：无
 */
static void DHT11_Mode_Out_PP(void)
{
	/* Configure P2.5 as Output mode */
	GPIO_SetMode(P2, BIT5, GPIO_PMD_OUTPUT);
}

static uint8_t Read_Byte(void)
{	  

   	 uint8_t i, temp=0;

	 for(i=0;i<8;i++)    
	 {
	 
	   /*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
    	while(DHT11_DATA_IN() == LOW);
		 
		 /*DHT11 以27~28us的高电平表示“0”，以70us高电平表示“1”，
	 		 通过检测60us后的电平即可区别这两个状态*/

		Delay_us(35); //延时30us		   	  
	
		  if(DHT11_DATA_IN()==HIGH)//60us后仍为高电平表示数据“1”
		   {
		   	/*轮询直到从机发出的剩余的 30us 高电平结束*/
   				 while(DHT11_DATA_IN()==HIGH);

				 temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1 
			
		   }
		   else	 //60us后为低电平表示数据“0”
		   {			   
		   	  temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0
		   }
	 }
	  return temp;
}

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	/*输出模式*/
   DHT11_Mode_Out_PP();
   /*主机拉低*/
   DHT11_DATA_OUT(LOW);
   /*延时18ms*/
   //Delay_ms(18);
	 os_dly_wait(18);
   /*总线拉高 主机延时30us*/
   DHT11_DATA_OUT(HIGH); 

   Delay_us(25);   //延时25us
   
 	/*主机设为输入 判断从机响应信号*/ 
   DHT11_Mode_IPU();

 /*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
   if(DHT11_DATA_IN()==LOW)   //T !   
    {
  
	
	  /*轮询直到从机发出 的80us 低电平 响应信号结束*/  
	    while(DHT11_DATA_IN()==LOW);
	  	 
	  /*轮询直到从机发出的 80us 高电平 标置信号结束*/
	    while(DHT11_DATA_IN()==HIGH);

	  /*开始接收数据*/   
	 	 DHT11_Data->humi_int= Read_Byte();
								
		 DHT11_Data->humi_deci= Read_Byte();
				
		 DHT11_Data->temp_int= Read_Byte();

		 DHT11_Data->temp_deci= Read_Byte();
				 		
		 DHT11_Data->check_sum= Read_Byte();
		 								  

		 	/*读取结束，引脚改为输出模式*/
  		 DHT11_Mode_Out_PP();
		   /*主机拉高*/
		 DHT11_DATA_OUT(HIGH);

		   /*检查读取的数据是否正确*/
		 if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
		  	return SUCCESS;
		  else 
		  	return ERROR;
   }
   else
   	{		
			return ERROR;
	}
   
}
/*******end of file***************/
