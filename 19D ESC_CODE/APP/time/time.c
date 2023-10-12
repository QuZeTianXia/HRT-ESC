#include "time.h"
uint8_t lock=0;
void time_init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	 //����һ���ṹ�������������ʼ����ʱ��

	NVIC_InitTypeDef NVIC_InitStructure;

	/* ������ʱ��5ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

	
	//��ʱ��5�ж�Ƶ��100Hz
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//���TIMx���жϴ�����λ:TIM �ж�Դ
	TIM_TimeBaseInitStructure.TIM_Period =999;//�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 719;//����������ΪTIMxʱ��Ƶ��Ԥ��Ƶֵ��100Khz����Ƶ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);	
	
	/* �����жϲ����������ж� */
	
	
	/* ����NVIC���� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;  //��Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE );	//ʹ�ܻ���ʧ��ָ����TIM�ж�
	TIM_Cmd(TIM5,ENABLE); //ʹ�ܻ���ʧ��TIMx����
}

void adjust()
{
	/***��ǰ������ΪLOCK��ǰ��λ**/
	
	if((RF_PRE>(PRE_LVE*50+120))||(lock&0x01))//�ƶ��ȼ���ʵ����ѹ��0-800���Ĺ�ϵΪ100-500
	{
		RF_TRA=3*1000/14;
		RF_STAB=5*1000/14;
		lock=lock|0x01;
		RF_IN=0;
		RF_OUT=0;
		LB_IN=0;
		LB_OUT=0;
		if(!SERVO&&!SERVO)
			SERCONT=100;
	}
	else
	{
		lock=lock&0xfe;
	}
	
	if((RF_PRE<(PRE_LVE*50+80))||(lock&0x02))
	{
		RF_IN=5*1000/14;
		RF_OUT=5*1000/14;
		LB_IN=5*1000/14;
		LB_OUT=5*1000/14;
		lock=lock|0x02;
		RF_TRA=0;
		RF_STAB=0;
		if(!SERVO&&!SERVO)
			SERCONT=100;
	}
	else
	{
		lock=lock&0xfd;
	}
	
	/****/
	
	
	/***��ǰ������Ϊ�������λ**/
	if((LF_PRE>(PRE_LVE*50+120))||(lock&0x10))
	{
		LF_TRA=3*1000/14;
		LF_STAB=5*1000/14;
		lock=lock|0x10;
		LF_IN=0;
		LF_OUT=0;
		RB_IN=0;
		RB_OUT=0;
		if(!SERVO&&!SERVO)
			SERCONT=100;
	}
	else
	{
		lock=lock&0xef;
	}
	
	if((LF_PRE<(PRE_LVE*50+80))||(lock&0x20))
	{
		LF_IN=5*1000/14;
		LF_OUT=5*1000/14;
		RB_IN=5*1000/14;
		RB_OUT=5*1000/14;
		lock=lock|0x20;
		LF_TRA=0;
		LF_STAB=0;
		if(!SERVO&&!SERVO)
			SERCONT=100;
	}
	else
	{
		lock=lock&0xdf;
	}	
	/*****/
	
	
	
	if(lock)
	{
		if(RF_PRE>=(PRE_LVE*50+100))
			lock=lock&0xfd;
		if(RF_PRE<=(PRE_LVE*50+100))
			lock=lock&0xfe;
		if(LF_PRE>=(PRE_LVE*50+120))
			lock=lock&0xdf;
		if(LF_PRE<=(PRE_LVE*50+120))
			lock=lock&0xef;
	}
	else
	{
		SERVO=0;
	}
}


void volchge()
{
	//if(RF_STAB)
		RF_STAB=RF_STAB*voltage/1400;
	//if(LB_IN)
		LB_IN=LB_IN*voltage/1400;
	//if(RF_IN)
		RF_IN=RF_IN*voltage/1400;
	//if(LF_TRA)
		LF_TRA=LF_TRA*voltage/1400;
	//if(LF_OUT)
		LF_OUT=LF_OUT*voltage/1400;
	//if(RB_OUT)
		RB_OUT=RB_OUT*voltage/1400;
	//if(LF_IN)
		LF_IN=LF_IN*voltage/1400;
	//if(SERVO)
		SERVO=SERVO*voltage/1400;
	//if(RF_TRA)
		RF_TRA=RF_TRA*voltage/1400;
	//if(LB_OUT)
		LB_OUT=LB_OUT*voltage/1400;
	//if(RF_OUT)
		RF_OUT=RF_OUT*voltage/1400;
	//if(LF_STAB)
		LF_STAB=LF_STAB*voltage/1400;
	//if(RB_IN)
		RB_IN=RB_IN*voltage/1400;
}


void TIM5_IRQHandler()	  //��ʱ��5�жϺ���
{
	static int timcount=0;
	timcount++;
	if(SERCONT)
			SERCONT--;
	if(timcount>=100)
		timcount=0;
	if(SERCONT==99)
	{
		if(SERVO!=0)
			SERVO=RISELVE*(uint16_t)((HIG_MOT-LOW_MOT)*(Timscale+1)/16.0)+SERVO;
		else
		{
			SERVO=RISELVE*(uint16_t)((HIG_MOT-LOW_MOT)*(Timscale+1)/16.0)+(uint16_t)(LOW_MOT*1000);
		}
	}
	else if(!SERCONT)
		SERVO=PRE_LVE*(uint16_t)((HIG_MOT-LOW_MOT)*(Timscale+1)/8.0)+(uint16_t)(LOW_MOT*1000);
	adjust();
	volchge();
	if(!PRE_LVE)
		SERVO=0;
	
	
	if(!timcount%10)    //����PWM״̬
	{
		TIM_SetCompare1(TIM1,RF_STAB);
		TIM_SetCompare2(TIM1,LB_IN);
		TIM_SetCompare3(TIM1,RF_IN);
	
		TIM_SetCompare1(TIM2,LF_TRA);
		TIM_SetCompare2(TIM2,LF_OUT);
		TIM_SetCompare3(TIM2,RB_OUT);
		TIM_SetCompare4(TIM2,LF_IN);
		
		TIM_SetCompare1(TIM4,SERVO);
		TIM_SetCompare2(TIM4,RF_TRA);
		TIM_SetCompare3(TIM4,RF_OUT);
		TIM_SetCompare4(TIM4,LB_OUT);
	
		TIM_SetCompare2(TIM3,LF_STAB);
		TIM_SetCompare3(TIM3,RB_IN);
		TIM_SetCompare1(TIM1,RF_STAB);
//		TIM_SetCompare2(TIM1,800);
//		TIM_SetCompare3(TIM1,500);
//	
//		TIM_SetCompare1(TIM2,500);
//		TIM_SetCompare2(TIM2,500);
//		TIM_SetCompare3(TIM2,800);
//		TIM_SetCompare4(TIM2,800);
//		
//		TIM_SetCompare1(TIM4,200);
//		TIM_SetCompare2(TIM4,800);
//		TIM_SetCompare3(TIM4,800);
//		TIM_SetCompare4(TIM4,800);
//	
//		TIM_SetCompare2(TIM3,800);
//		TIM_SetCompare3(TIM3,800);
		LED1=!LED1;
		LED2=!LED2;
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
}

