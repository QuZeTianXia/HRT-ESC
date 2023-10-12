#ifndef __CAN_H
#define __CAN_H
#include "public.h"
#define CAN_RX0_INT_ENABLE 1

void CAN1_Config(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);
void CAN1_SendMesg(uint32_t id, uint8_t len, uint8_t *dat);
void CAN1_Config16BitFilter(uint16_t id1, uint16_t id2);
void CAN1_Config32BitFilter(uint32_t id);
void CAN1_ReceiveMesg(uint8_t *receiveBuff);

#endif
