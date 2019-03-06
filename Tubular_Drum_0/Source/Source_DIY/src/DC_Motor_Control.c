#include "stm32f10x.h"
#include "DC_Motor_Control.h"
#include "delay.h"

extern TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
extern NVIC_InitTypeDef 		NVIC_InitStructure;
extern GPIO_InitTypeDef  	GPIO_InitStructure;
extern EXTI_InitTypeDef  	EXTI_InitStructure;
extern TIM_OCInitTypeDef 	TIM_OCInitStructure;


void PWM_Configuration(uint16_t pulse)
{	
	/* Enable TIM3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 4.8-1;														
  TIM_TimeBaseInitStructure.TIM_Period = 15000-1;															
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
																									
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// clear on compare match
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
	
	TIM_OCInitStructure.TIM_Pulse=pulse-1;		//3599
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, DISABLE);	
}	

void PWM_update(uint16_t pulse)
{	
	TIM_Cmd(TIM3,	DISABLE);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
	
	TIM_OCInitStructure.TIM_Pulse=pulse-1;		
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);								// TIM2_CH3		
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
}	

	/*
	 1. The motor run when
		BRA: Low
		PWM: High
		DIR: High or Low
	  
		2. The motor stop (without brake) when
		PWM: Low
		BRA: Low
		DIR: X
	
		3. The motor stop (with brake) when
		PWM, DIR, BRA: High
		PWM, BRA: High		and DIR: Low
		PWM: Low and DIR (X), Brake: High
		-------------------------------------
		|				PB0: PWM										|			
		|				PA0:	BRAKE									|
		|				PB1: 	DIR									  |
		-------------------------------------
	*/
	
void Run_Motor (uint16_t pulse)
	{
			// turn on the motor
		  GPIO_SetBits(GPIOB, GPIO_Pin_0);
			//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		
			// direction determination
			GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		
			//set speed
			PWM_update(pulse);
	}

void Stop_Motor_no_BRA(void)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_0| GPIO_Pin_1);
	}