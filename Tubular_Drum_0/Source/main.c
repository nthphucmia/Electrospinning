/********************************************************************************
 1. Use time1 with 4 channels to control 2 motors for PWM are TIM2_CH3 with frequency 2kHz. 
 2. Two encoders combined in motors have 2 phase A,B read by TIM3 with interrupt configuration.
 3. TIM4 is interrupted every 10ms to calculate the velocity.
 4. The calculated velocity is printed in PC by UART with baud rate: 115200
 
 The connected diagram
 STM32F103C8:
 PA2, PA3: Module UART2
 PA6, PA7: phase A, B encoder (motor2) - TIM3				
 PA0, PA1: upper and lower bridge driver motor1		
 PA2: TIM2-CH3
 ----------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "stdio.h"
#include "Usart_serial.h"
#include "User_interrupt.h"
#include "GPIO_Configuration.h"
#include "Encoder_Configuration.h"
#include "DC_Motor_Control.h"
#include "stm32f10x_type.h"
#include "stdio.h"
#include "LiquidCrystal_I2C.h"
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
NVIC_InitTypeDef 		NVIC_InitStructure;
GPIO_InitTypeDef  	GPIO_InitStructure;
EXTI_InitTypeDef  	EXTI_InitStructure;
TIM_OCInitTypeDef 	TIM_OCInitStructure;
TIM_ICInitTypeDef 	TIM_ICInitStructure;
TIM_ICInitTypeDef 	TIM_ICInitStructure;
I2C_InitTypeDef		  I2C_InitStructure;       // khai bao bien de su dung thu vien chuan I2C cua ST.
/*----------------------------------------------------------------------------------------*/
	char buffer[40];
	char buffer1[40];
	volatile int32_t actual_speed;
	volatile uint8_t sample_count;
	int varEncoderInput=0;
	int varEncoderMotor=0;
int main(void)
{ 
	GPIO_USART_Configuration();
	USARTx_Configuration(USART3);
	Motor_pin_Configuration();
	Encoder_pin_Configuraton();
	
	EXTI9_5_Configuration();
		//Encoder Activating
	Encoder_System_Configuration();
	Encoder_Reading_Configuration();
  NVIC_Configuration();
	
	//Activate motor
	PWM_Configuration(2);								// PWM Init
  PWM_update(2);
	//PWM_update(15000);											// test motor
	
	// LCD Initialization
	SysTick_Config(SystemCoreClock/1000);
	I2C_LCD_Configuration();
	lcd_init ();                                 // ham khoi dong LCD16x2
	lcd_goto_XY(1,0);
	lcd_send_string ("SET SPEED:");
	lcd_goto_XY(2,0);
	lcd_send_string("REAL SPEED:");
		USARTx_SendString(USART3,"\n\rCCW\n\r");
	while (1)
  {	
		if (sample_count>=10) //hien thi moi 250ms
			{
					/*---------------------------------SET-SPEED-----------------------------*/
				sprintf(buffer,"%d",varEncoderInput);
				lcd_goto_XY(1,10);
				lcd_send_string("      ");
				lcd_goto_XY(1,10);
				lcd_send_string(buffer);
				
				/*--------------------------------RUN-SPEED----------------------------------*/
				sprintf(buffer1,"%d",actual_speed);
				lcd_goto_XY(2,10);
				lcd_send_string("      ");
				lcd_goto_XY(2,11);
				lcd_send_string(buffer1);
				Delay_msST(1000);
			}	
	}
}

/***********************************************************************/	