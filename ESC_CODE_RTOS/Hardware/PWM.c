#include "stm32f10x.h"                  // Device header
#include "Global_variable.h"

//int PWM_Value = 555;
//int PWMscale = 999;

//uint8_t ESC_Drive_flag = 0;


void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIMBaseInitStructure;
	TIM_OCInitTypeDef TIMOCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=PWMscale;
	TIMBaseInitStructure.TIM_Prescaler=35;
	TIMBaseInitStructure.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM1,&TIMBaseInitStructure);
	
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=PWMscale;
	TIMBaseInitStructure.TIM_Prescaler=35;	
	TIMBaseInitStructure.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM2,&TIMBaseInitStructure);
	
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=PWMscale;
	TIMBaseInitStructure.TIM_Prescaler=35;
	TIMBaseInitStructure.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM3,&TIMBaseInitStructure);
	
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=PWMscale;
	TIMBaseInitStructure.TIM_Prescaler=35;
	TIMBaseInitStructure.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM4,&TIMBaseInitStructure);
	
	TIMBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIMBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMBaseInitStructure.TIM_Period=PWMscale;
	TIMBaseInitStructure.TIM_Prescaler=35;
	TIMBaseInitStructure.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM8,&TIMBaseInitStructure);
	
	
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
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
	
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
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
	
	TIMOCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
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
	TIM_OC4Init(TIM4,&TIMOCInitStructure);//初始化15路pwm输出（1+14）
	
	TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;       
	TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIMOCInitStructure.TIM_Pulse=50;
	TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //????:TIM???????
	TIMOCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;        //互补输出使能关
	TIMOCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIMOCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIMOCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIM1, &TIMOCInitStructure);  //??T??????????TIM1 4OC1         LFO     RSTB1
	TIM_OC2Init(TIM1, &TIMOCInitStructure);  //??T??????????TIM1 4OC1         RBO     LBO
	TIM_OC3Init(TIM1, &TIMOCInitStructure);  //??T??????????TIM1 4OC1         LSTB1   RSTB2
	
	TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;       
	TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIMOCInitStructure.TIM_Pulse=50;
	TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //????:TIM???????
	TIMOCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;        //互补输出使能关
	TIMOCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIMOCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIMOCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIM8, &TIMOCInitStructure);  //??T??????????TIM1 4OC1         LFO     RSTB1
	TIM_OC2Init(TIM8, &TIMOCInitStructure);  //??T??????????TIM1 4OC1         RBO     LBO
	TIM_OC3Init(TIM8, &TIMOCInitStructure);  //??T??????????TIM1 4OC1         LSTB1   RSTB2
	TIM_OC4Init(TIM8, &TIMOCInitStructure);
	
//	TIM_SetCompare1(TIM1,100);		//PA8//LSC
//	TIM_SetCompare2(TIM1,100);		//PA9//RBO
//	TIM_SetCompare3(TIM1,100);		//PA10//LFO
//	TIM_SetCompare1(TIM2,100);		//PA0//RTC
//	TIM_SetCompare2(TIM2,100);		//PA1//RFI
//	TIM_SetCompare3(TIM2,100);		//PA2//LBI
//	TIM_SetCompare4(TIM2,100);		//PA3//RFO
//	TIM_SetCompare1(TIM3,100);		//PA6
//	TIM_SetCompare2(TIM3,100);		//PA7//RSC1
//	TIM_SetCompare3(TIM3,100);		//PB0//LBO
//	TIM_SetCompare4(TIM3,100);		//PB1
//	TIM_SetCompare1(TIM4,50.);		//PB6//PUMP
//	TIM_SetCompare2(TIM4,100);		//PB7//LTC
//	TIM_SetCompare3(TIM4,100);		//PB8//LFI
//	TIM_SetCompare4(TIM4,100);		//PB9//RBI
	
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
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	TIM_Cmd(TIM8,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
	TIM_CtrlPWMOutputs(TIM8, ENABLE); 
	
	TIM_SetCompare1(TIM1,PWM_Value);		//PA8//LSC
	TIM_SetCompare2(TIM1,PWM_Value);		//PA9//RBO
	TIM_SetCompare3(TIM1,PWM_Value);		//PA10//LFO
	TIM_SetCompare1(TIM2,PWM_Value);		//PA0//RTC
	TIM_SetCompare2(TIM2,PWM_Value);		//PA1//RFI
	TIM_SetCompare3(TIM2,PWM_Value);		//PA2//LBI
	TIM_SetCompare4(TIM2,PWM_Value);		//PA3//RFO
	TIM_SetCompare1(TIM3,PWM_Value);		//PA6
	TIM_SetCompare2(TIM3,PWM_Value);		//PA7//RSC1
	TIM_SetCompare3(TIM3,PWM_Value);		//PB0//LBO
	TIM_SetCompare4(TIM3,PWM_Value);		//PB1//R2TB2
	TIM_SetCompare1(TIM4,PWM_Value);		//PB6//PUMP
	TIM_SetCompare2(TIM4,PWM_Value);		//PB7//LTC
	TIM_SetCompare3(TIM4,PWM_Value);		//PB8//LFI
	TIM_SetCompare4(TIM4,PWM_Value);		//PB9//RBI
	TIM_SetCompare1(TIM8,PWM_Value);		
	TIM_SetCompare2(TIM8,PWM_Value);		
	TIM_SetCompare3(TIM8,PWM_Value);		
	TIM_SetCompare4(TIM8,PWM_Value);
	
}


void Back_Pressure_Up_Slow(void)
{
//	Back_State = Slow_UP;

	Set_LBI_PWM(0)
	Set_LBO_PWM(0)
	Set_RBO_PWM(0)
	Set_RBI_PWM(0)
	
	Set_RFI_PWM(150)//-//RTC*********
	Set_RSTB2_PWM(150)//-*如开//RSTB*******
	Set_PUMP_PWM(Slow_Accelerate)	
}

void Front_Pressure_Up_Slow(void)
{
//	Front_State = Slow_UP;
	
	Set_LFI_PWM(0)
	Set_LFO_PWM(0)
	Set_RFO_PWM(0)
	Set_RFI_PWM(0)

	Set_LSTB2_PWM(150)//--//LSTB1******
	Set_LFI_PWM(125)//-//LTC*******	
	Set_PUMP_PWM(Slow_Accelerate)	
}

void Back_Pressure_Up_Quick(void)
{


	Set_LBI_PWM(0)
	Set_LBO_PWM(0)
	Set_RBO_PWM(0)
	Set_RBI_PWM(0)
	
	Set_RFI_PWM(150)//-//RTC*********
	Set_RSTB2_PWM(150)//-*如开//RSTB*******
	Set_PUMP_PWM(Quick_Accelerate)	
}

  void Front_Pressure_Up_Quick(void)
{

	
	Set_LFI_PWM(0)
	Set_LFO_PWM(0)
	Set_RFO_PWM(0)
	Set_RFI_PWM(0)


	Set_LSTB2_PWM(150)//--//LSTB1******
	Set_LFI_PWM(125)//-//LTC*******	
	Set_PUMP_PWM(Quick_Accelerate)	
}


void Front_Pressure_Down_Quick(void)
{

	
	Set_RSTB1_PWM(0)
	Set_RTC_PWM(0)
	Set_LSTB2_PWM(0)//--//LSTB1******
	Set_LFI_PWM(0)//-//LTC*******	
	Set_PUMP_PWM(0)
//				Set_LTC_PWM(150)//-//LFI
//		Set_LBO_PWM(150)//--LFO
	
//	Set_LFI_PWM(150)//前轮降压
//	Set_LFO_PWM(150)
//	Set_RFO_PWM(150)
////	Set_RFI_PWM(150)

	
//	ESC_Release();
}

void Back_Pressure_Down_Quick(void)
{
//	Back_State = Quick_DOWN;
	
	Set_LSTB1_PWM(0)
	Set_LTC_PWM(0)
	
	Set_LBI_PWM(0)
	Set_LBO_PWM(0)
	
	Set_RFI_PWM(0)//-//RTC*********
	Set_RSTB2_PWM(0)//-*如开//RSTB*******
	
//	Set_RBO_PWM(100)
//	Set_RBI_PWM(100)
	Set_PUMP_PWM(0)
}

void Front_Pressure_Down_Slow(void)
{
//	Front_State = Slow_DOWN;
	
	Set_RSTB1_PWM(0)
	Set_RTC_PWM(0)
	Set_LSTB2_PWM(0)//--//LSTB1******
	Set_LFI_PWM(0)//-//LTC*******	
	Set_PUMP_PWM(0)
	
//				Set_LTC_PWM(150)//-//LFI
//		Set_LBO_PWM(150)//--LFO
	
//	Set_LFI_PWM(200)
//	Set_LFO_PWM(0)
//	Set_RFO_PWM(150)
//	Set_RFI_PWM(150)
//	Set_PUMP_PWM(50)
	//ESC_Release();
}

void Back_Pressure_Down_Slow(void)
{
	Set_LBI_PWM(0)
	Set_LBO_PWM(0)
	Set_RBO_PWM(0)
	Set_RBI_PWM(0)
	Set_PUMP_PWM(0)
	
	Set_RFI_PWM(0)//-//RTC*********
	Set_RSTB2_PWM(0)//-*如开//RSTB*******
	Set_PUMP_PWM(0)
}




void Front_Release(void)
{
//	Front_State = Release;
	
//	Set_RSTB1_PWM(0)
//	Set_RTC_PWM(0)
//	
//	Set_LFI_PWM(0)
//	Set_LFO_PWM(0)
//	Set_RFO_PWM(0)
//	Set_RFI_PWM(0)
//	Set_PUMP_PWM(0)
	Set_LSTB2_PWM(0)//--//LSTB1******
	Set_LFI_PWM(0)//-//LTC*******
//			Set_LTC_PWM(150)//-//LFI
//		Set_LBO_PWM(150)//--LFO	
	Set_PUMP_PWM(0)

}

void Back_Release(void)
{
	Set_RFI_PWM(0)//-//RTC*********
	Set_RSTB2_PWM(0)//-*如开//RSTB*******
	
	Set_LBI_PWM(0)
	Set_LBO_PWM(0)
	Set_RBO_PWM(0)
	Set_RBI_PWM(0)
	
	Set_PUMP_PWM(0)
}

void ESC_Release(void)
{		
	Front_Release();
	Back_Release();
}



