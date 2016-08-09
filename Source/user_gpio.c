#include "user_gpio.h"


void exit_wake_up(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;    
	EXTI_InitTypeDef EXTI_InitStructure;   
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE); 
 
	/* EXTI line gpio config(PA0) */       
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);   
   
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);    
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;   
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;   
	EXTI_Init(&EXTI_InitStructure);	
}

//  toggle gpio
void GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIO_WriteBit(GPIOx, GPIO_Pin, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin)));
}

void BSP_GPIO_IO1_Set(uint8_t enabled)
{
	if (enabled)		
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		
	else
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
}


void BSP_GPIO_IO2_Set(uint8_t enabled)
{
	if (enabled)
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	else
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
}


void BSP_GPIO_IO3_Set(uint8_t enabled)
{
	if (enabled)
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	else
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
}

void BSP_GPIO_IO4_Set(uint8_t enabled)
{
	if (enabled)
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	else
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
}


/********************************************************************************************************
**������Ϣ ��void BSP_GPIO_Init(void)                         // WAN.CG // 2010.12.8 
**�������� ��GPIO��ʼ����������ʼ��ʵ���õ�������GPIO��
**������� ����
**������� ����
**������ʾ ��RCC_APB2PeriphClockCmd������GPIO_Init����
********************************************************************************************************/
void User_GpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOB�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //����GPIOC�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //����GPIOD�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  

	
	// CHRG_STATUS -PA1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_1|GPIO_Pin_2);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);//��C��ʱ�ӣ�����ʱ�ӡ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //��Ϊ�����ע�⣺2MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	PWR_BackupAccessCmd(ENABLE);//�����޸�RTC �ͺ󱸼Ĵ���
	RCC_LSEConfig(RCC_LSE_OFF);//�ر��ⲿ�����ⲿʱ���źŹ��� ��PC13 PC14 PC15 �ſ��Ե���ͨIO�á�
	BKP_TamperPinCmd(DISABLE);//�ر����ּ�⹦�ܣ�Ҳ���� PC13��Ҳ���Ե���ͨIO ʹ��
	PWR_BackupAccessCmd(DISABLE);//��ֹ�޸ĺ󱸼Ĵ���
//	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	//GPIO_SetBits(GPIOC, GPIO_Pin_13);



}

// LEDϵͳ����ָʾ
void LED_SystemRun(void)
{
	uint8_t num = 3;
// 	LED_RED_ON;

	while(num--)
	{
		GPIO_Toggle(GPIOC,LED_GREEN);
		GPIO_Toggle(GPIOA,LED_RED);
		GPIO_Toggle(GPIOC,LED_BLUE);
		DelayMS(100);
	}
	LED_RED_OFF;
	LED_GREEN_OFF;
	LED_BLUE_OFF;
	
}

void DetectPortInput(void)
{
	ExInput.Bit.In0_0 = I0_0_STATUS;
	ExInput.Bit.In0_1 = I0_1_STATUS;
	ExInput.Bit.In0_2 = I0_2_STATUS;
	
	
	ExInput.Bit.In0_3 = 1^I0_3_STATUS;
	ExInput.Bit.In0_4 = 1^I0_4_STATUS;
	ExInput.Bit.In0_5 = 1^I0_5_STATUS;
	ExInput.Bit.In0_6 = 1^I0_6_STATUS;
	ExInput.Bit.In0_7 = 1^I0_7_STATUS;
	ExInput.Bit.In0_8 = 1^I0_8_STATUS;
	
	ExInput.Bit.In1_0 = I1_0_STATUS;
	ExOutput.Value=0;

}

// Q0.0 -- PA3 @PWM TIM5_CH1
// Q0.1 -- PA2 @PWM TIM5_CH2  
// Q0.2 -- PA1 @PWM TIM5_CH3
// Q0.3 -- PA0 @PWM TIM5_CH4
// Q0.4 -- PB0
// Q0.5 -- PB1
// Q0.6 -- PC13
// Q0.7 -- PD0

void OutputControl(PORT_OUTPUT Output)
{

			if(Output.Bit.Q0) 
				GPIO_ResetBits(LED_PORT, BEEP);
			else 
				GPIO_SetBits(LED_PORT, BEEP);

			if(ExInput.Bit.In1_0==0)		
				GPIO_ResetBits(LED_PORT, LED_BLUE);  // open LED
			else 
				GPIO_SetBits(LED_PORT, LED_BLUE);    // close LED
		
}


