/******************** (C) COPYRIGHT *********************************************
* Project Name       : MyUSB_hid
* Author             : DZ561
* Version            : V1.0
* Date               : Jun-17-2010
* Description        : 	
用于学习STM32 USB HID
源码已测试通
欢迎访问我的博客：http://blog.ednchina.com/itspy/  
PC上位机程序可以到我的博客去下载  
********************************************************************************/
#include "stm32f10x_lib.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "stm32f10x_it.h"

#define nReportCnt 22

extern u8 Receive_Buffer[22];
extern u8 Transi_Buffer[22];
extern u8 USB_ReceiveFlg;

void DebugApp(void);
void NVIC_Configuration(void);
extern void RCC_Configuration(void);
extern void USB_SendString(u8 *str);
void GPIO_Toggle(GPIO_TypeDef* GPIOx, vu16 GPIO_Pin);
void usb_gpio_init(void);
void led_gpio_init(void);
vu8 MsgCmd;
extern u8 TimeCount;

vu32 time_cnt=0;

//此程序没加LCD屏显示，使用说明请看程序文件目录下有介绍！！
int main(void)
{
#ifdef USER_DEBUG
    // DBGMCU->CR |= DBGMCU_CR_DBG_STOP;  //for debug purpose
#endif  
     RCC_Configuration();
     NVIC_Configuration();
     Set_System();	 

#ifdef STM32F103VC
     RCC->APB2ENR |= (1 << 3);                 // enable clock for GPIOB  
     GPIOB->CRL &= ~0x00F00000;                // clear port PB5  
     GPIOB->CRL |=  0x00700000;                // PB5 General purpose output open-drain, max speed 50 MHz  
     GPIOB->BRR  = 0x0020;                     // reset PD2  (set to low)  
#endif
	usb_gpio_init();
	led_gpio_init();
     USB_Interrupts_Config();
     Set_USBClock();    
     USB_Init();
     USB_ReceiveFlg = FALSE;
     while(1) //
     {
         if (TimeCount && USB_ReceiveFlg == TRUE) //收到后上位机的数据后，将1S发送一次数据给PC
         {
             USB_SendString("Hi,PC! I'm STM32-ARM");
             TimeCount = 0x00;
         }
				 
				time_cnt++;
				if(time_cnt>0xFFFFF)				 
				{
				time_cnt=0;
				GPIO_Toggle(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);

				}
				 
				 
				 
     }
}

void GPIO_Toggle(GPIO_TypeDef* GPIOx, vu16 GPIO_Pin)
{
  GPIO_WriteBit(GPIOx, GPIO_Pin, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin)));
}


void USB_SendString(u8 *str)
{
     u8 ii=0;   
     while(*str)
     {
         Transi_Buffer[ii++]=*(str++);
         if (ii == nReportCnt) break;
     }
     UserToPMABufferCopy(Transi_Buffer, ENDP2_TXADDR, nReportCnt);
     SetEPTxValid(ENDP2);
}
/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{    
  //NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
     NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
     NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif 
}
/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nCount)
{
  for(; nCount!= 0;nCount--);
}

void usb_gpio_init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
}
void led_gpio_init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOA端口

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
}



#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while(1)
  {
  }
  
}
#endif
