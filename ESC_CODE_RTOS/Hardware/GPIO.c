#include "stm32f10x.h"                  // Device header
#include "Global_variable.h"

void GPIOS_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SERVO_EN_Pin |L9348_EN_Pin |VALVES_EN_Pin; //SERVO_EN  //L9348_EN //VALVES+_EN
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SERVO_EN_Port ,&GPIO_InitStructure);
	
	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Pin = DR1_STA_Pin;//PE7:DR3_STA, //PE9:DR1_STA  //PE15:DR2_STA
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(DR1_STA_Port ,&GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Pin = DR4_STA_Pin|DR2_STA_Pin |DR3_STA_Pin ;             //PB11:DR4_STA
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(DR4_STA_Port ,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED1_Pin|LED2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED1_Port ,&GPIO_InitStructure);	
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED3_Pin; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED3_Port ,&GPIO_InitStructure);	
	
	
}


void ESC_ENABLE(void)//驱动使能
{
	GPIO_SetBits(SERVO_EN_Port ,SERVO_EN_Pin );//电机使能
	GPIO_SetBits(L9348_EN_Port ,L9348_EN_Pin );//芯片使能
	GPIO_SetBits(VALVES_EN_Port ,VALVES_EN_Pin);//主电源使能
	ESC_Drive_flag = 1;
}

void ESC_DISABLE(void)//驱动失能
{
	GPIO_ResetBits(SERVO_EN_Port ,SERVO_EN_Pin );//电机失能
	GPIO_ResetBits(L9348_EN_Port ,L9348_EN_Pin );//芯片失能
	GPIO_ResetBits(VALVES_EN_Port ,VALVES_EN_Pin);//主电源失能
	ESC_Drive_flag = 0;
}

void ESC_Drive_State(uint8_t ESC_Drive_flag)//ESC驱动状态
{
	GPIO_WriteBit(SERVO_EN_Port ,SERVO_EN_Pin ,(BitAction)ESC_Drive_flag);
	GPIO_WriteBit(L9348_EN_Port ,L9348_EN_Pin ,(BitAction)ESC_Drive_flag);
	GPIO_WriteBit(VALVES_EN_Port ,VALVES_EN_Pin ,(BitAction)ESC_Drive_flag);
}


void LED1_ON(void)
{
	GPIO_ResetBits(LED1_Port ,LED1_Pin );
}

void LED1_OFF(void)
{
	GPIO_SetBits(LED1_Port ,LED1_Pin );
}

void LED1_FLASH(void)
{
	if(GPIO_ReadOutputDataBit(LED1_Port ,LED1_Pin )==1)
	{
		GPIO_ResetBits(LED1_Port ,LED1_Pin );		
	}
	else
	{
		GPIO_SetBits(LED1_Port ,LED1_Pin );		
	}
}


void LED2_ON(void)
{
	GPIO_ResetBits(LED2_Port ,LED2_Pin );
}

void LED2_OFF(void)
{
	GPIO_SetBits(LED2_Port ,LED2_Pin );
}

void LED2_FLASH(void)
{
	if(GPIO_ReadOutputDataBit(LED2_Port ,LED2_Pin )==1)
	{
		GPIO_ResetBits(LED2_Port ,LED2_Pin );		
	}
	else
	{
		GPIO_SetBits(LED2_Port ,LED2_Pin );		
	}
}


void LED3_ON(void)
{
	GPIO_ResetBits(LED3_Port ,LED3_Pin );
}

void LED3_OFF(void)
{
	GPIO_SetBits(LED3_Port ,LED3_Pin );
}

void LED3_FLASH(void)
{
	if(GPIO_ReadOutputDataBit(LED3_Port ,LED3_Pin )==1)
	{
		GPIO_ResetBits(LED3_Port ,LED3_Pin );		
	}
	else
	{
		GPIO_SetBits(LED3_Port ,LED3_Pin );		
	}
}

void ESC_Status_Flash(void)
{
	if(ESC_Drive_flag ==0)
	{
		ESC_ENABLE();
	}
	else
	{
		ESC_DISABLE();
	}
}

uint16_t Positive_Difference(uint16_t big_pre,uint16_t small_pre)
{
	uint16_t difference;
	difference = big_pre - small_pre;
	return difference;
}

uint16_t Absolute_Difference(uint16_t pre1,uint16_t pre2)
{
	uint16_t difference;
	if(pre1 > pre2)   difference = pre1 - pre2;
	if(pre1 < pre2)   difference = pre2 - pre1;
	return difference;
}








