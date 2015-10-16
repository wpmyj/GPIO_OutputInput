#include "Sensor.h"

// int samplingTime = 280;			//������ʱ��λus
// int deltaTime = 40;					//������ʱ��λus
//int sleepTime = 9680;				//������ʱ��λus
volatile char PM_State = 1;				//pm2.5����������ȼ�״̬λ 1.�� 2.�� 3.��
volatile char TVOC_State = 1;			//TVOC����������ȼ�״̬λ 1.�� 2.�� 3.��
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
/*******************************PM2.5�������ź�ת��*********************************************/
int ADC_Convert_PM(void)		//ADC��ѹת��  
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
	
	//Disp_Temp(Convert_Value);						//��ʾֵ��ʵ�ʵ�ѹ��10��  ʹ���¶��������ʾ���ο�
	return Convert_Value;		
}

/*********************��ζ�������ź�ת��***************************************************/
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
	
	//Disp_Temp(Convert_Value);						//��ʾֵ��ʵ�ʵ�ѹ��10�� ʹ���¶��������ʾ���ο�
	return Convert_Value;
}
void Tvoc(void)				//��ζ����������ת������ʾ
{
	int tvoc = 40;			//��ʼ����һ��Ϊ����
	int	tvoc_sum = 0,tvoc_aver = 0,i,State;//
	TVOC_ON;
	//TVOC_Ctrl = 1;
// 	if(1 == TVOC_Ctrl)
// 	{
	for(i = 0;i < 2;i++)
	{
		tvoc =	ADC_Convert_TVOC();	//��ζADת��
		tvoc_sum = tvoc_sum + tvoc;
	}
	tvoc_aver = (int)(tvoc_sum / 2);
	
	if(tvoc_aver >= TVOC1_LIMI) //����TVOC������
	{
		State = 1;
	}
		else if((tvoc_aver < (TVOC1_LIMI - 1))&&(tvoc_aver >= TVOC2_LIMI))		//����TVOC������
		{
			State = 2;
		}
		else if(tvoc_aver < (TVOC2_LIMI - 1))		//����TVOC������
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
//	Disp_TVOC(tvoc_aver);			//��ʾ��ζ�ȼ�
// 	}
// 	TVOC_Ctrl ^= 1;
	
}
void loop(void)				//PM2.5���������ݶ�ȡ����ʾ
{
	int pm = 0,i,sumpm = 0,averpm = 0,State;	//,averpm_prev = 0
	int samplingTime = 280;				//������ʱ��λus
	int deltaTime = 40;					//������ʱ��λus
	for(i = 0;i < 100;i++)
	{
		PMLEDPOWER_ON;				//power on the LED
		Delay_us(samplingTime);		
		pm = ADC_Convert_PM();		//ADCת��ͨ���¶������������ʾ
		Delay_us(deltaTime);
		PMLEDPOWER_OFF;
		sumpm = sumpm + pm;
		//Delay_us(sleepTime);
		os_dly_wait(10);
	}
	averpm = (int)(sumpm / 100);
	if(averpm <= PM1_LIMI) //����PM2.5������
	{
		State = 1;
	}
		else if((averpm > PM1_LIMI)&&(averpm <= (PM2_LIMI - 1)))	//����PM2.5������
		{
			State = 2;
		}
		else if(averpm > PM2_LIMI)		//����PM2.5������
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
//	Disp_PM(averpm);				//��ʾPM2.5��״̬
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
void Delay_us(unsigned n)			//1us����ȷ��ʱ
{
	unsigned int i,j;
	for(i = 0;i < n;i++)
		{
			for(j = 0;j < 10;j++);
		}
}
/*�Զ�ģʽ�� ����ͨ����������(��������õ�����)�ı�*/
void Auto_Wind(void)
{
	if(0 == ManualMode_Flag)		//�Զ�ģʽ��
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
 * ��������DHT11_GPIO_Config
 * ����  ������DHT11�õ���I/O��
 * ����  ����
 * ���  ����
 */
void DHT11_GPIO_Config(void)
{		 
	/* Configure P2.5 as Output mode */
	GPIO_SetMode(P2, BIT5, GPIO_PMD_OUTPUT);
	/* ����DHT11�ܽ�	*/
	DHT11_HIGH;
}

/*
 * ��������DHT11_Mode_IPU
 * ����  ��ʹDHT11-DATA���ű�Ϊ����ģʽ
 * ����  ����
 * ���  ����
 */
static void DHT11_Mode_IPU(void)
{
	/* Configure P2.7 as Output mode */
	GPIO_SetMode(P2, BIT5, GPIO_PMD_INPUT);
}
/*
 * ��������DHT11_Mode_Out_PP
 * ����  ��ʹDHT11-DATA���ű�Ϊ���ģʽ
 * ����  ����
 * ���  ����
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
	 
	   /*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
    	while(DHT11_DATA_IN() == LOW);
		 
		 /*DHT11 ��27~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
	 		 ͨ�����60us��ĵ�ƽ��������������״̬*/

		Delay_us(35); //��ʱ30us		   	  
	
		  if(DHT11_DATA_IN()==HIGH)//60us����Ϊ�ߵ�ƽ��ʾ���ݡ�1��
		   {
		   	/*��ѯֱ���ӻ�������ʣ��� 30us �ߵ�ƽ����*/
   				 while(DHT11_DATA_IN()==HIGH);

				 temp|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1 
			
		   }
		   else	 //60us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		   {			   
		   	  temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0
		   }
	 }
	  return temp;
}

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	/*���ģʽ*/
   DHT11_Mode_Out_PP();
   /*��������*/
   DHT11_DATA_OUT(LOW);
   /*��ʱ18ms*/
   //Delay_ms(18);
	 os_dly_wait(18);
   /*�������� ������ʱ30us*/
   DHT11_DATA_OUT(HIGH); 

   Delay_us(25);   //��ʱ25us
   
 	/*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 
   DHT11_Mode_IPU();

 /*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
   if(DHT11_DATA_IN()==LOW)   //T !   
    {
  
	
	  /*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
	    while(DHT11_DATA_IN()==LOW);
	  	 
	  /*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/
	    while(DHT11_DATA_IN()==HIGH);

	  /*��ʼ��������*/   
	 	 DHT11_Data->humi_int= Read_Byte();
								
		 DHT11_Data->humi_deci= Read_Byte();
				
		 DHT11_Data->temp_int= Read_Byte();

		 DHT11_Data->temp_deci= Read_Byte();
				 		
		 DHT11_Data->check_sum= Read_Byte();
		 								  

		 	/*��ȡ���������Ÿ�Ϊ���ģʽ*/
  		 DHT11_Mode_Out_PP();
		   /*��������*/
		 DHT11_DATA_OUT(HIGH);

		   /*����ȡ�������Ƿ���ȷ*/
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
