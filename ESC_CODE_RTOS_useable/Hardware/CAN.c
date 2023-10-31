#include "stm32f10x.h"                  // Device header 
#include "Global_variable.h"  

uint16_t PWM = 0;
uint8_t ESC_Drive_flag = 0;
uint16_t Front_PWM_Value = 0;
uint16_t Back_PWM_Value = 0;

int timenum = 0;
//int PUMPnum = 0;
uint8_t CANSend[8];
uint8_t EBS_flag = 0;

uint8_t Emergency_Flag = 0;

uint8_t Front_State = 0;
uint8_t Back_State = 0;
uint8_t Front_Count = 0;
uint8_t Back_Count = 0;

uint8_t ESC_Front_Switch_Flag = 0;
uint8_t ESC_Back_Switch_Flag = 0;

uint8_t OLD_Front_State = 0;
uint8_t OLD_Back_State = 0;

uint8_t ECU_State = 0;
uint8_t Old_ECU_State = 0;
uint8_t ECU_State_Switch_Flag = 0;
uint8_t Ros_State = 0;
uint8_t Old_Ros_State = 0;
uint8_t Ros_State_Switch_Flag = 0;

uint8_t CAN_Message[8];

uint16_t LF_PRE = 0;//定义四路油压参数
uint16_t LB_PRE = 0;
uint16_t RF_PRE = 0;
uint16_t RB_PRE = 0;

uint16_t LF_Target = 0;//定义四路目标油压
uint16_t LB_Target = 0;
uint16_t RF_Target = 0;
uint16_t RB_Target = 0;

uint8_t normal_mode = 1;


void CAN1_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
		//初始化IO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//TX//复用推挽
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;//RX//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//初始化CAN
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_Prescaler = 9;   //预分频
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //CAN模式
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;    //重新同步跳跃宽度为个时间单位
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;    //时间段1占用5个时间单位
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;    //时间段2占用2个时间单位
	CAN_InitStructure.CAN_TTCM = DISABLE;       // 非时间触发通信模式
	CAN_InitStructure.CAN_ABOM= ENABLE;         //软件自动离线管理
	CAN_InitStructure.CAN_AWUM= ENABLE;         //睡眠模式通过软件唤醒
	CAN_InitStructure.CAN_NART = ENABLE;        //禁止报文自动传送
	CAN_InitStructure.CAN_RFLM = DISABLE;       //报文不锁定,新的覆盖旧的
	CAN_InitStructure.CAN_TXFP = DISABLE;       //优先级由报文标识符决定
	
	CAN_Init(CAN1,&CAN_InitStructure);
	//初始化过滤器
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;  //过滤器使能
	CAN_FilterInitStructure.CAN_FilterNumber = 0;            //过滤器 0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; //掩码模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;     //32/16位
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //FIFO0
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0;              //设置过滤器ID高16位//验证码
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0;               //设置过滤器ID低16位
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0;          //设置过滤器掩码高16位//屏蔽码
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0;           //设置过滤器掩码低16位
	
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                    //使能FIFO0中断 
	
	//NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	
}


void CAN1_SendMesg(uint32_t id, uint8_t len, uint8_t *dat)//CAN发送函数
{
 	uint16_t i = 0;
	CanTxMsg TxMessage;
  if(len > 8)
   {
      return ;
   }
	TxMessage.StdId = id & 0x7FF; //标准帧ID11位
	 //TxMessage.ExtId = (id);   //设置扩展标示符（拓展标示符有29位）
	TxMessage.RTR = CAN_RTR_DATA;   //设置为数据帧（或远程帧为CAN_RTR_Remote）
  TxMessage.IDE = CAN_ID_STD;	//标准ID
   // }
	TxMessage.DLC = len;	        //发送的数据长度

	/* 将数据放入到邮箱中 */
	for(i=0; i<len; i++)	         
	{
		TxMessage.Data[i] = *dat;
		dat++;	
	}
    
    /* 开始传送数据 */
	CAN_Transmit(CAN1, &TxMessage); 
}




void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	LED1_FLASH();
	
	//From:EBS//content:current oil pressure//Fre:20Hz
	if(RxMessage.StdId==0x402&&RxMessage.IDE==0)  //当前油压
	{
//		LF_PRE = RxMessage.Data[0] + RxMessage.Data[1]*256 ;
		RF_PRE = RxMessage.Data[2] + RxMessage.Data[3]*256 ;
		LB_PRE = RxMessage.Data[4] + RxMessage.Data[5]*256 ;
		RB_PRE = RxMessage.Data[6] + RxMessage.Data[7]*256 ;
	 }
	
	//From:ros//content:ros State//Fre:20Hz 
	 else if(RxMessage.StdId==0x20&&RxMessage.IDE==0)//EBS信号
	 {
		 Old_Ros_State = Ros_State;
		 Ros_State = RxMessage.Data[0];
		 if(Old_Ros_State != Ros_State)  Ros_State_Switch_Flag = 1;
	 }
	 
	 //From:华海//content:
	 else if(RxMessage.StdId==0x666&&RxMessage.IDE==0)//目标油压
	 {
		 if(normal_mode)
		 {
			RB_Target = RxMessage.Data[0] + RxMessage.Data[1]*256 ;
			RF_Target = RxMessage.Data[2] + RxMessage.Data[3]*256 ;
			LB_Target = RxMessage.Data[4] + RxMessage.Data[5]*256 ;
//			LF_Target = RxMessage.Data[6] + RxMessage.Data[7]*256 ;
		 }
	 }
	 
	 //From:ECU//content:ECU状态
	 else if(RxMessage.StdId==0x156&&RxMessage.IDE==0)//ECU状态
	 {
		 Old_ECU_State = ECU_State;
		 ECU_State = RxMessage.Data[0] & 0x7;//只在ECU在AS_Driving(4)、AS_Emergency(6)、Slow_stop(7)三种状态下才给ESC使能
		 if(Old_ECU_State != ECU_State)  ECU_State_Switch_Flag = 1;

	 }
	 
	  //From:USER//content:更新配置
	 else if(RxMessage.StdId==0x317&&RxMessage.IDE==0)
	 {
		 if(RxMessage.Data[0] == 0xAA)
		 {
			 Quick_Accelerate = RxMessage.Data[1];
		 }
		 if(RxMessage.Data[0] == 0xBB)
		 {
			 Slow_Accelerate = RxMessage.Data[1];
		 }
		 Store_Save();
		 if(RxMessage.Data[0] == 0xCC)
		 {
			 Keep_Accelerate = RxMessage.Data[1];
		 }
	 }
	 
	 
	  
	 
//	 else if(RxMessage.StdId==0x156&&RxMessage.IDE==0)//ECU信号
//	 {
//		ESC_Drive_flag = RxMessage.Data[0];
//		ESC_ENABLE();
//		
//			  PRE_LVE_ecu=(RxMessage.Data[6]>>2)&0x07;//0-5    限制为2级
//			  PRE_MOD_ecu=RxMessage.Data[3];
//			  PRE_SERVO_ecu=RxMessage.Data[2];
//			  PRE_SET_ecu=RxMessage.Data[1];
//			  PRE_S  ET_ecu*=2;
//			  if(PRE_LVE_ebs)
//				  PRE_LVE=2;
//			  else 
//				  PRE_LVE = PRE_LVE_ecu;
//			PRE_LVE=5;
//			  if(PRE_LVE>2/*||braktime==1*/)
//					PRE_LVE=2;
//				if((RxMessage.Data[0]&0x07)==4)   //是否制动模式
//				{
//					if(!braktime)	
//						braktime=300;
//				}
//				else
//					braktime=0;
//				if(PRE_LVE)
//				{
//					SERVOEN=1;
//					L9348EN=1;
//					VALVEEN=1;
//				}
//				else
//				{
//					SERVOEN=0;
//					L9348EN=0;
//					VALVEEN=0;
//				} 
//				if((PRE_LVE>OLD_LVE)&&(OLD_LVE==0))				//从制动等级为0开始
//				{
//					SERCONT=100;
//					RISELVE=PRE_LVE-OLD_LVE;
//				}
//				else if(PRE_LVE<OLD_LVE)
//				{
//					SERCONT=0;
//					RISELVE=0;
//				}
//				OLD_LVE=PRE_LVE;
//	}	
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
}


	
