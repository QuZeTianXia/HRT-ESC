#include "stm32f10x.h"                  // Device header

#ifndef CAN_H
#define CAN_H

void CAN1_Init(void);
void CAN1_SendMesg(uint32_t id, uint8_t len, uint8_t *dat);//CAN·¢ËÍº¯Êý

#endif

