#include "public.h"
#include "pwm.h"
uint16_t RF_IN=0; //���ڵ�ŷ�
uint16_t RF_OUT=0;//���ڵ�ŷ�
uint16_t LF_IN=0;
uint16_t LF_OUT=0;
uint16_t RB_IN=0;
uint16_t RB_OUT=0;
uint16_t LB_IN=0;
uint16_t LB_OUT=0;
uint16_t SERVO=0; //�ŷ����
uint16_t LF_TRA=0;
uint16_t RF_TRA=0;//ǣ�����ȶ���ŷ�
uint16_t LF_STAB=0;//�ȶ����Ƶ�ŷ����������ӿ�
uint16_t RF_STAB=0;

uint16_t voltage=1375;
uint16_t RB_PRE=0;
uint16_t RF_PRE=0;
uint16_t LF_PRE=0;
uint16_t LB_PRE=0;
uint16_t LVE_PRE=0;

uint8_t SERCONT=0;
uint8_t RISELVE=0;
uint8_t PRE_LVE=1;

void ALL_Init()
{
	//SysTick_Init(72);
	//JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	GPIO_init();
	time_init();
	pwm_init();
	
	CAN1_Config(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);

}
