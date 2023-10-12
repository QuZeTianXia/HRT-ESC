#ifndef _public_H
#define _public_H

#include "stm32f10x.h"
#include "misc.h"
#include "SysTick.h"
#include "system.h"
#include "led.h"
#include "time.h"
#include "CAN.h"
#include "pwm.h"

#define LED1 PBout(12)
#define LED2 PBout(13)
#define SERVOEN PDout(3)
#define L9348EN PDout(6)
#define VALVEEN PEout(11)


#define DR1STA PEin(9)
#define DR2STA PEin(15)
#define DR3STA PEin(7)
#define DR4STA PBin(11)
#define VALVESTA PEin(13)
#define Timscale 999
#define LOW_MOT 0.5
#define HIG_MOT 0.8
//对应1000Khz
extern uint16_t RF_IN;
extern uint16_t RF_OUT;
extern uint16_t LF_IN;
extern uint16_t LF_OUT;
extern uint16_t RB_IN;
extern uint16_t RB_OUT;
extern uint16_t LB_IN;
extern uint16_t LB_OUT;
extern uint16_t SERVO;
extern uint16_t LF_TRA;
extern uint16_t RF_TRA;//牵引
extern uint16_t LF_STAB;//稳定，有三个接口
extern uint16_t RF_STAB;

extern uint16_t voltage;
extern uint16_t RB_PRE;
extern uint16_t RF_PRE;
extern uint16_t LF_PRE;
extern uint16_t LB_PRE;
extern uint16_t LVE_PRE;

extern uint8_t SERCONT;//?
extern uint8_t RISELVE;//?
extern uint8_t PRE_LVE;
void ALL_Init(void);


#endif
