#include "pwm.h"

void pwm_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIMBaseInitStructure;
	TIM_OCInitTypeDef TIMOCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOB, &GPIO_InitStructure);                //15个PWM口
	
	
	//四个定时器的配置
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //PWM频率为(1/Timescale)MHz
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=Timscale;
	TIMBaseInitStructure.TIM_Prescaler=71;
	TIMBaseInitStructure.TIM_RepetitionCounter=0X00;
	TIM_TimeBaseInit(TIM1,&TIMBaseInitStructure);
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=Timscale;
	TIMBaseInitStructure.TIM_Prescaler=71;
	TIMBaseInitStructure.TIM_RepetitionCounter=0X00;
	TIM_TimeBaseInit(TIM2,&TIMBaseInitStructure);
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=Timscale;
	TIMBaseInitStructure.TIM_Prescaler=71;
	TIMBaseInitStructure.TIM_RepetitionCounter=0X00;
	TIM_TimeBaseInit(TIM3,&TIMBaseInitStructure);
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=Timscale;
	TIMBaseInitStructure.TIM_Prescaler=71;
	TIMBaseInitStructure.TIM_RepetitionCounter=0X00;
	TIM_TimeBaseInit(TIM4,&TIMBaseInitStructure);
	
//	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
//	TIMOCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
//	TIMOCInitStructure.TIM_OutputState=ENABLE;
//	TIMOCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
//	TIM_OC1Init(TIM1,&TIMOCInitStructure);
//	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
//	TIMOCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
//	TIMOCInitStructure.TIM_OutputState=ENABLE;
//	TIMOCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
//	TIM_OC2Init(TIM1,&TIMOCInitStructure);
//	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
//	TIMOCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
//	TIMOCInitStructure.TIM_OutputState=ENABLE;
//	TIMOCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
//	TIM_OC3Init(TIM1,&TIMOCInitStructure);
	
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;        //定时器2的四个PWM通道
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC1Init(TIM2,&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC2Init(TIM2,&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC3Init(TIM2,&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC4Init(TIM2,&TIMOCInitStructure);
	
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;         //定时器3的四个PWM通道
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC1Init(TIM3,&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC2Init(TIM3,&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC3Init(TIM3,&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC4Init(TIM3,&TIMOCInitStructure);
	
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;        //定时器4的四个PWM通道
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC1Init(TIM4,&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC2Init(TIM4,&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC3Init(TIM4,&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMOCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMOCInitStructure.TIM_OutputState=ENABLE;
	TIM_OC4Init(TIM4,&TIMOCInitStructure);
	TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
       
    TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIMOCInitStructure.TIM_Pulse=50;
    TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIMOCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIMOCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIMOCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIMOCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIM1, &TIMOCInitStructure);  //TIM1 4OC1
	TIM_OC2Init(TIM1, &TIMOCInitStructure);
	TIM_OC3Init(TIM1, &TIMOCInitStructure);
	                                                                 //初始化15路pwm输出
	
	
	TIM_SetCompare1(TIM1,0);  //所有PWM置零
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);
	TIM_SetCompare1(TIM2,0);
	TIM_SetCompare2(TIM2,0);
	TIM_SetCompare3(TIM2,0);
	TIM_SetCompare4(TIM2,0);
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare4(TIM3,0);
	TIM_SetCompare1(TIM4,0);
	TIM_SetCompare2(TIM4,0);
	TIM_SetCompare3(TIM4,0);
	TIM_SetCompare4(TIM4,0);
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
}
