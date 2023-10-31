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

uint16_t LF_PRE = 0;//������·��ѹ����
uint16_t LB_PRE = 0;
uint16_t RF_PRE = 0;
uint16_t RB_PRE = 0;

uint16_t LF_Target = 0;//������·Ŀ����ѹ
uint16_t LB_Target = 0;
uint16_t RF_Target = 0;
uint16_t RB_Target = 0;

uint8_t normal_mode = 1;


void CAN1_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
		//��ʼ��IO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//TX//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;//RX//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��ʼ��CAN
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_Prescaler = 9;   //Ԥ��Ƶ
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //CANģʽ
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;    //����ͬ����Ծ���Ϊ��ʱ�䵥λ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;    //ʱ���1ռ��5��ʱ�䵥λ
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;    //ʱ���2ռ��2��ʱ�䵥λ
	CAN_InitStructure.CAN_TTCM = DISABLE;       // ��ʱ�䴥��ͨ��ģʽ
	CAN_InitStructure.CAN_ABOM= ENABLE;         //����Զ����߹���
	CAN_InitStructure.CAN_AWUM= ENABLE;         //˯��ģʽͨ���������
	CAN_InitStructure.CAN_NART = ENABLE;        //��ֹ�����Զ�����
	CAN_InitStructure.CAN_RFLM = DISABLE;       //���Ĳ�����,�µĸ��Ǿɵ�
	CAN_InitStructure.CAN_TXFP = DISABLE;       //���ȼ��ɱ��ı�ʶ������
	
	CAN_Init(CAN1,&CAN_InitStructure);
	//��ʼ��������
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;  //������ʹ��
	CAN_FilterInitStructure.CAN_FilterNumber = 0;            //������ 0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; //����ģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;     //32/16λ
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //FIFO0
	
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0;              //���ù�����ID��16λ//��֤��
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0;               //���ù�����ID��16λ
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0;          //���ù����������16λ//������
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0;           //���ù����������16λ
	
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);                    //ʹ��FIFO0�ж� 
	
	//NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	
}


void CAN1_SendMesg(uint32_t id, uint8_t len, uint8_t *dat)//CAN���ͺ���
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
	CanRxMsg RxMessage;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	LED1_FLASH();
	
	//From:EBS//content:current oil pressure//Fre:20Hz
	if(RxMessage.StdId==0x402&&RxMessage.IDE==0)  //��ǰ��ѹ
	{
//		LF_PRE = RxMessage.Data[0] + RxMessage.Data[1]*256 ;
		RF_PRE = RxMessage.Data[2] + RxMessage.Data[3]*256 ;
		LB_PRE = RxMessage.Data[4] + RxMessage.Data[5]*256 ;
		RB_PRE = RxMessage.Data[6] + RxMessage.Data[7]*256 ;
	 }
	
	//From:ros//content:ros State//Fre:20Hz 
	 else if(RxMessage.StdId==0x20&&RxMessage.IDE==0)//EBS�ź�
	 {
		 Old_Ros_State = Ros_State;
		 Ros_State = RxMessage.Data[0];
		 if(Old_Ros_State != Ros_State)  Ros_State_Switch_Flag = 1;
	 }
	 
	 //From:����//content:
	 else if(RxMessage.StdId==0x666&&RxMessage.IDE==0)//Ŀ����ѹ
	 {
		 if(normal_mode)
		 {
			RB_Target = RxMessage.Data[0] + RxMessage.Data[1]*256 ;
			RF_Target = RxMessage.Data[2] + RxMessage.Data[3]*256 ;
			LB_Target = RxMessage.Data[4] + RxMessage.Data[5]*256 ;
//			LF_Target = RxMessage.Data[6] + RxMessage.Data[7]*256 ;
		 }
	 }
	 
	 //From:ECU//content:ECU״̬
	 else if(RxMessage.StdId==0x156&&RxMessage.IDE==0)//ECU״̬
	 {
		 Old_ECU_State = ECU_State;
		 ECU_State = RxMessage.Data[0] & 0x7;//ֻ��ECU��AS_Driving(4)��AS_Emergency(6)��Slow_stop(7)����״̬�²Ÿ�ESCʹ��
		 if(Old_ECU_State != ECU_State)  ECU_State_Switch_Flag = 1;

	 }
	 
	  //From:USER//content:��������
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
	 
	 
	  
	 
//	 else if(RxMessage.StdId==0x156&&RxMessage.IDE==0)//ECU�ź�
//	 {
//		ESC_Drive_flag = RxMessage.Data[0];
//		ESC_ENABLE();
//		
//			  PRE_LVE_ecu=(RxMessage.Data[6]>>2)&0x07;//0-5    ����Ϊ2��
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
//				if((RxMessage.Data[0]&0x07)==4)   //�Ƿ��ƶ�ģʽ
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
//				if((PRE_LVE>OLD_LVE)&&(OLD_LVE==0))				//���ƶ��ȼ�Ϊ0��ʼ
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


	
