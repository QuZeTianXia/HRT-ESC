#include "can.h"


void CAN1_Config(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* ��ʼ��IO�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;        //PA12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;        //PA11

	GPIO_Init(GPIOA, &GPIO_InitStructure);



    /* ��ʼ��CAN�Ĳ��� */

	CAN_DeInit(CAN1);
	

	/* CAN ������ʼ�� */
	CAN_InitStructure.CAN_TTCM = DISABLE;    //ʧ��ʱ�䴥��ģʽ
	CAN_InitStructure.CAN_ABOM = DISABLE;    //ʧ���Զ����߹���
	CAN_InitStructure.CAN_AWUM = DISABLE;    //ʧ��˯��ģʽͨ���������
	CAN_InitStructure.CAN_NART = ENABLE;    //ʧ�ܷ��Զ��ش���ģʽ��Ҳ���ǻ��Զ��ش��䣩
	CAN_InitStructure.CAN_RFLM = DISABLE;    //ʧ�ܽ���FIFO����ģʽ�������ݻḲ�Ǿ�����
	CAN_InitStructure.CAN_TXFP = DISABLE;    //���ȼ��ɱ��ı�ʶ������ 
	CAN_InitStructure.CAN_Mode = mode;       //����ͨģʽ����չģʽ
	CAN_InitStructure.CAN_SJW = tsjw; //����ͬ����Ծ��� 1 ��ʱ�䵥λ
	
	CAN_InitStructure.CAN_BS1 = tbs1; //ʱ��� 1 Ϊ9 ��ʱ�䵥λ 
	CAN_InitStructure.CAN_BS2 = tbs2; //ʱ��� 2 Ϊ8 ��ʱ�䵥λ
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
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // �����ȼ�Ϊ1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure); 
		
//		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX1_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // �����ȼ�Ϊ1
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ0
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
	TxMessage.StdId = id & 0x7FF; //��׼֡ID11λ
	//TxMessage.ExtId = (id);   //������չ��ʾ������չ��ʾ����29λ��
	TxMessage.RTR = CAN_RTR_DATA;   //����Ϊ����֡����Զ��֡ΪCAN_RTR_Remote��
//    if((id & 0x7FF) == 0x7FF)       //����Ǳ�׼֡������չ֡����չ֡����11λ��
//    {
//    	TxMessage.IDE = CAN_ID_STD;	//��չID   
//    }
//    else
//    {
    	TxMessage.IDE = CAN_ID_STD;	//��׼ID
   // }
	TxMessage.DLC = len;	        //���͵����ݳ���

	/* �����ݷ��뵽������ */
	for(i=0; i<len; i++)	         
	{
		TxMessage.Data[i] = *dat;
		dat++;	
	}
    
    /* ��ʼ�������� */
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
		 LB_PRE=(RxMessage.Data[3]*550/256-50)*2;//ת��Ϊʵ����ѹ0��1000��������Ϊ����ѹ��500�����������ѹ800
		 voltage=RxMessage.Data[4]*1430/256;//ʵ�ʵ�ѹֵ*100
		 
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
