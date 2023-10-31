#ifndef GPIO_H
#define GPIO_H
#include "stm32f10x.h"                  // Device header


void GPIOS_Init(void);

void ESC_ENABLE(void);//驱动使能
void ESC_DISABLE(void);//驱动失能
void ESC_Status_Flash(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED1_FLASH(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED2_FLASH(void);
void ESC_Drive_State(uint8_t ESC_Drive_flag);//ESC驱动状态
void LED3_FLASH(void);
void LED3_ON(void);
void LED3_OFF(void);

uint16_t Absolute_Difference(uint16_t pre1,uint16_t pre2);
uint16_t Positive_Difference(uint16_t big_pre,uint16_t small_pre);

#endif
