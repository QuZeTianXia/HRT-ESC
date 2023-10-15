#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "GPIO.h" 
 
#include "Task_schedule.h" 
#include "Processes.h" 

#include "PWM.h"
#include "GPIO.h"
#include "KEY.h"
#include "CAN.h"
#include "oled.h"
#include "Store.h"

#define LED1_Pin           GPIO_Pin_14 
#define LED1_Port          GPIOB

#define LED2_Pin           GPIO_Pin_12
#define LED2_Port          GPIOB

#define LED3_Pin           GPIO_Pin_9
#define LED3_Port          GPIOD

#define SERVO_EN_Port      GPIOE
#define SERVO_EN_Pin       GPIO_Pin_14

#define L9348_EN_Port      GPIOE
#define L9348_EN_Pin       GPIO_Pin_13

#define VALVES_EN_Port     GPIOE
#define VALVES_EN_Pin      GPIO_Pin_12

#define DR1_STA_Port       GPIOB
#define DR1_STA_Pin        GPIO_Pin_3

#define DR2_STA_Port       GPIOD
#define DR2_STA_Pin        GPIO_Pin_5

#define DR3_STA_Port       GPIOD
#define DR3_STA_Pin        GPIO_Pin_3

#define DR4_STA_Port       GPIOD
#define DR4_STA_Pin        GPIO_Pin_0




#define PWMscale   249//2K



extern uint8_t ESC_Drive_flag ;



extern int PWM_Value;

extern int timenum;//定义一个时间参数
//extern int PUMPnum;//电机运行时间
extern uint16_t OUTPUT;

extern uint16_t LF_PRE;//定义四路油压参数
extern uint16_t LB_PRE;
extern uint16_t RF_PRE;
extern uint16_t RB_PRE;


extern uint16_t LF_Target;//定义四路目标油压
extern uint16_t LB_Target;
extern uint16_t RF_Target;
extern uint16_t RB_Target;


extern uint8_t Front_State ;
extern uint8_t Back_State ;
extern uint8_t OLD_Front_State ;
extern uint8_t OLD_Back_State ;

extern uint8_t Front_Count;
extern uint8_t Back_Count;

extern uint8_t CAN_Message[8];


extern uint8_t Emergency_Flag;

extern uint8_t ECU_State;
extern uint8_t Ros_State;
extern uint8_t Old_ECU_State;
extern uint8_t ECU_State_Switch_Flag;
extern uint8_t Ros_State_Switch_Flag;

extern uint8_t EBS_flag;
extern uint8_t ESC_Drive_flag;

extern uint8_t ESC_Front_Switch_Flag;
extern uint8_t ESC_Back_Switch_Flag;

extern uint16_t LFOUT,RFOUT,LBOUT,RBOUT;

extern uint8_t normal_mode;

#define Quick_Accelerate  Store_Data[1]
#define Slow_Accelerate  Store_Data[2]


#define Quick_UP          0x0A
#define Slow_UP           0x0B
#define Quick_DOWN        0x0C
#define Slow_DOWN         0x0D
#define Release           0x0E
#define Finish            0xAB


#define ECU_State_AS_Driving      0x4
#define ECU_State_AS_Emergency    0x6
#define ECU_State_AS_Slow_Stop    0x7
#define ECU_State_AS_Ready    0x3
#define ECU_State_AS_Finish    0x5
#define ROS_Finish            0x5


#define Set_LTC_PWM(x)	    TIM_SetCompare1(TIM2,x);		//PA0//LTC-	
#define Set_LFO_PWM(x)		TIM_SetCompare2(TIM2,x);		//PA1//LFO-
#define Set_RTC_PWM(x)		TIM_SetCompare3(TIM2,x);		//PA2//RTC-
#define Set_RFI_PWM(x)		TIM_SetCompare1(TIM3,x);		//PA6//RFI-
#define Set_LBI_PWM(x)		TIM_SetCompare2(TIM3,x);		//PA7//LBI-
#define Set_RBI_PWM(x)		TIM_SetCompare3(TIM3,x);		//PB0//RBI-
#define Set_LFI_PWM(x)		TIM_SetCompare4(TIM3,x);		//PB1//LFI-
#define Set_LBO_PWM(x)		TIM_SetCompare1(TIM4,x);		//PB6//LBO-
#define Set_RSTB2_PWM(x)	TIM_SetCompare2(TIM4,x);		//PB7//RSTB2-
#define Set_RSTB1_PWM(x)	TIM_SetCompare3(TIM4,x);		//PB8//RSTB1-
#define Set_PUMP_PWM(x)		TIM_SetCompare4(TIM4,x);		//PB9//PUMP-
#define Set_RFO_PWM(x)		TIM_SetCompare1(TIM8,x);	    //PC6//RFO-
#define Set_RBO_PWM(x)		TIM_SetCompare2(TIM8,x);	    //PC7//RBO-	
#define Set_LSTB2_PWM(x)	TIM_SetCompare3(TIM8,x);		//PC8//LSTB2-
#define Set_LSTB1_PWM(x)	TIM_SetCompare4(TIM8,x);	    //PC9//LSTB1-
