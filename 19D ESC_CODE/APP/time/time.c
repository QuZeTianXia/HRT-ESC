#include "time.h"
uint8_t lock=0;
void time_init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	 //声明一个结构体变量，用来初始化定时器

	NVIC_InitTypeDef NVIC_InitStructure;

	/* 开启定时器5时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

	
	//定时器5中断频率100Hz
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//清除TIMx的中断待处理位:TIM 中断源
	TIM_TimeBaseInitStructure.TIM_Period =999;//设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 719;//设置用来作为TIMx时钟频率预分频值，100Khz计数频率
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);	
	
	/* 设置中断参数，并打开中断 */
	
	
	/* 设置NVIC参数 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;  //响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能
	NVIC_Init(&NVIC_InitStructure);	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE );	//使能或者失能指定的TIM中断
	TIM_Cmd(TIM5,ENABLE); //使能或者失能TIMx外设
}

void adjust()
{
	/***右前，锁存为LOCK的前两位**/
	
	if((RF_PRE>(PRE_LVE*50+120))||(lock&0x01))//制动等级和实际油压（0-800）的关系为100-500
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
	
	
	/***左前，锁存为第五第六位**/
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


void TIM5_IRQHandler()	  //定时器5中断函数
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
	
	
	if(!timcount%10)    //更新PWM状态
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

