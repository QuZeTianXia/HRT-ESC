#include "can.h"


void CAN1_Config(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* 初始化IO口 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;        //PA12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;        //PA11

	GPIO_Init(GPIOA, &GPIO_InitStructure);



    /* 初始化CAN的参数 */

	CAN_DeInit(CAN1);
	

	/* CAN 参数初始化 */
	CAN_InitStructure.CAN_TTCM = DISABLE;    //失能时间触发模式
	CAN_InitStructure.CAN_ABOM = DISABLE;    //失能自动离线管理
	CAN_InitStructure.CAN_AWUM = DISABLE;    //失能睡眠模式通过软件唤醒
	CAN_InitStructure.CAN_NART = ENABLE;    //失能非自动重传输模式（也就是会自动重传输）
	CAN_InitStructure.CAN_RFLM = DISABLE;    //失能接收FIFO锁定模式，新数据会覆盖旧数据
	CAN_InitStructure.CAN_TXFP = DISABLE;    //优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode = mode;       //有普通模式和拓展模式
	CAN_InitStructure.CAN_SJW = tsjw; //重新同步跳跃宽度 1 个时间单位
	
	CAN_InitStructure.CAN_BS1 = tbs1; //时间段 1 为9 个时间单位 
	CAN_InitStructure.CAN_BS2 = tbs2; //时间段 2 为8 个时间单位
	CAN_InitStructure.CAN_Prescaler = brp;	 

		CAN_Init(CAN1, &CAN_InitStructure);
 
		CAN_FilterInitStructure.CAN_FilterNumber=0;      //????0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //???????????
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //????????32??

//    CAN_FilterInitStructure.CAN_FilterIdHigh= ((u32)0x564<<5);    //????ID??
//    CAN_FilterInitStructure.CAN_FilterIdLow= ((u32)0x234<<5); //????ID??
    CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;    //????ID??
    CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000; //????ID??
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;   //????16???????
    CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;   //????16???????
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;    //???????FIFO0
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;   //?????
		CAN_FilterInit(&CAN_FilterInitStructure);
 
		CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);		    

//		CAN_FilterInitStructure.CAN_FilterNumber=1;      //????0
//    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //???????????
//		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //????????32??

//    CAN_FilterInitStructure.CAN_FilterIdHigh= (((u32)0x186040f3<<21)&0xffff0000)>>16;    //????ID??
//    CAN_FilterInitStructure.CAN_FilterIdLow= (((u32)0x186040f3<<21)|CAN_ID_EXT|CAN_RTR_DATA)&0xffff;; //????ID??
//    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;   //????16???????
//    CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;   //????16???????
//		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;    //???????FIFO0
//		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;   //?????
//		CAN_FilterInit(&CAN_FilterInitStructure);
// 
		CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 主优先级为1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure); 
		
//		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX1_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 主优先级为1
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为0
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//		NVIC_Init(&NVIC_InitStructure);
  

}


void CAN1_SendMesg(uint32_t id, uint8_t len, uint8_t *dat)
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
//    if((id & 0x7FF) == 0x7FF)       //检测是标准帧还是拓展帧（拓展帧大于11位）
//    {
//    	TxMessage.IDE = CAN_ID_STD;	//拓展ID   
//    }
//    else
//    {
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
//	uint8_t i;
	static uint8_t OLD_LVE;
	CanRxMsg RxMessage;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	 if(RxMessage.StdId==0x134&&RxMessage.IDE==0)
	 {
		 RB_PRE=(RxMessage.Data[0]*550/256-50)*2;
		 RF_PRE=(RxMessage.Data[1]*550/256-50)*2;
		 LF_PRE=(RxMessage.Data[2]*550/256-50)*2;
		 LB_PRE=(RxMessage.Data[3]*550/256-50)*2;//转化为实际油压0到1000，但是因为最大电压在500，所以最大油压800
		 voltage=RxMessage.Data[4]*1430/256;//实际电压值*100
		 
	 }
		if(RxMessage.StdId==0x564&&RxMessage.IDE==0)
		{
			  PRE_LVE=RxMessage.Data[0];
				if(PRE_LVE)
				{
					SERVOEN=1;
					L9348EN=1;
					VALVEEN=1;
				}
				else
				{
					SERVOEN=0;
					L9348EN=0;
					VALVEEN=0;
				}
				if(PRE_LVE>OLD_LVE)
				{
					SERCONT=100;
					RISELVE=PRE_LVE-OLD_LVE;
				}
				else if(PRE_LVE<OLD_LVE)
				{
					SERCONT=0;
					RISELVE=0;
				}
				OLD_LVE=PRE_LVE;
		}	
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
} 
