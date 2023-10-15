#include "stm32f10x.h"                  // Device header
#include "Global_variable.h"


//初始化函数
void Hardware_Init(void)
{	
	GPIOS_Init();
	PWM_Init();
	KEY_Init();
	CAN1_Init();
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //禁用JTAG	
}



//CAN发送任务函数
void CAN_Send_Task(void *pvParameters)
{

    while(1)
    {
		CAN_Message[0] = ESC_Drive_flag;
		CAN_Message[1] = Front_State;
		CAN_Message[2] = Back_State;
		CAN_Message[3] = ESC_Front_Switch_Flag;
		CAN_Message[4] = ESC_Back_Switch_Flag;
		
		LED3_FLASH();
		CAN1_SendMesg(0x407,8,CAN_Message);
		vTaskDelay(50);
    }    
}


//开机Timng控制任务函数
void Turn_on_Timing_Task(void *pvParameters)
{
    while(1)
    {
		LED1_FLASH();
		
		if(ECU_State_Switch_Flag)
		{			
			if(ECU_State == ECU_State_AS_Driving)
			{
				normal_mode = 1;
				ESC_ENABLE();
				ESC_Drive_flag = 1;
			}
			else	
			{
				normal_mode = 0;
				ESC_DISABLE();
				ESC_Release();
				ESC_Drive_flag = 0;
				
			}
		
					
			if(/*ECU_State == ECU_State_AS_Emergency || */ECU_State == ECU_State_AS_Slow_Stop)
			{
				normal_mode = 0;
				
				Emergency_Flag = 1;
				
				ESC_ENABLE();
				ESC_Drive_flag = 1;
				
				ESC_Front_Switch_Flag = 0;
				ESC_Back_Switch_Flag = 0;
				
//				LF_Target = 3000;
				RF_Target = 3000;
				
				vTaskDelay(7000);
				
				ESC_DISABLE();
				
//				LF_Target = 0;
				RF_Target = 0;
				
				Front_State = Release;
				Back_State = Release;	
				
				ESC_Drive_flag = 0;
				
				Emergency_Flag = 0;
				normal_mode = 1;
			}
			
			if(ECU_State == ECU_State_AS_Emergency)
			{
				normal_mode = 0;
				vTaskDelay(1000);
				if(LF_PRE < 2000 || RF_PRE < 2000 || LB_PRE < 2000 ||RB_PRE < 2000)
				{
					Emergency_Flag = 1;
				
					ESC_ENABLE();
					ESC_Drive_flag = 1;
					
					ESC_Front_Switch_Flag = 0;
					ESC_Back_Switch_Flag = 0;
					
//					LF_Target = 3000;
					RF_Target = 3000;
					
					vTaskDelay(5000);
					
					ESC_DISABLE();
					
//					LF_Target = 0;
					RF_Target = 0;
					
					Front_State = Release;
					Back_State = Release;	
					
					ESC_Drive_flag = 0;
					
					Emergency_Flag = 0;
					
					normal_mode = 1;
				}
			}
			
		
			ECU_State_Switch_Flag = 0;	
			
		}
		
		if(Ros_State_Switch_Flag)
		{
			if(Ros_State == ROS_Finish)
			{
					ECU_State_Switch_Flag = 0;
				
					normal_mode = 0;
				
					Emergency_Flag = 1; 
				
					ESC_ENABLE();
					ESC_Drive_flag = 1;
					
					ESC_Front_Switch_Flag = 0;
					ESC_Back_Switch_Flag = 0;
					
//					LF_Target = 3000;
					RF_Target = 3000;
					
					vTaskDelay(7000);
					
					ESC_DISABLE();
				
//					LF_Target = 0;
					RF_Target = 0;
					
					Front_State = Release;
					Back_State = Release;	
					
					ESC_Drive_flag = 0;
					
					Emergency_Flag = 0;
					
					normal_mode = 1;
			}
		Ros_State_Switch_Flag = 0;
		}
		
		vTaskDelay(75);
    }    
}


//状态切换控制任务函数
void State_Switch_Control_Task(void *pvParameters)
{
    while(1)
    {
		LED1_FLASH ();
		
		if(Front_Count == 0)//刚刚切换了状态
		{
			ESC_Front_Switch_Flag = 0;//前轮不可切换状态
			Front_Count	= 2;		//开始计数
		}

		
		if(Back_Count == 0)
		{
			ESC_Back_Switch_Flag = 0;	
			Front_Count	= 2;			
		}


		
		
		if(Front_Count >= 2)        Front_Count++;
		if(Front_Count >= 12)
		{
			Front_Count = 1;
			ESC_Front_Switch_Flag = 1;
			
		}

		
		if(Back_Count >= 2)        Back_Count++;
		if(Back_Count >= 12)
		{
			Back_Count = 1;
			ESC_Back_Switch_Flag = 1;
		}


		
			
		vTaskDelay(100);	
    }    
}

//PWM任务函数
void PWM_Task(void *pvParameters)
{
	
    while(1)
    {	
		LED2_FLASH();
		
		OLD_Front_State = Front_State;
		OLD_Back_State = Back_State;
		
//		if(!Emergency_Flag)
//		{
		//前轮快速增压
		if(LF_Target - LF_PRE > 500 || RF_Target - RF_PRE > 500) 
		{
//			Front_Pressure_Up_Quick();
			
//			OLD_Front_State = Front_State;
//			Front_Count = 0;
			Front_State	= Quick_UP;		
		}
		
		
		//前轮缓慢增压
		if((LF_Target - LF_PRE < 500 && LF_Target - LF_PRE > 50 ) || (RF_Target - RF_PRE < 500 && RF_Target - RF_PRE > 50 )) 
		{
//			Front_Pressure_Up_Slow();
			
//			OLD_Front_State = Front_State;
//			Front_Count = 0;
			Front_State	= Slow_UP;	
		}

		
		//前轮缓慢降压
//		if((LF_PRE - LF_Target > 100  && LF_PRE - LF_Target < 500) ||	(RF_PRE - RF_Target > 50 && RF_PRE - RF_Target < 500))
//		{
////			Front_Pressure_Down_Slow();
//			
////			OLD_Front_State = Front_State;
////			Front_Count = 0;
//			Front_State	= Slow_DOWN;
//		}
		
		
		//前轮快速降压
		if(LF_PRE - LF_Target > 500 || RF_PRE - RF_Target > 500)
		{
//			Front_Pressure_Down_Quick();
			
//			OLD_Front_State = Front_State;
//			Front_Count = 0;
			Front_State	= Quick_DOWN;
		}

		
//		//后轮快速增压
//		if(LB_Target - LB_PRE > 500 || RB_Target - RB_PRE > 500)
//		{
////			Back_Pressure_Up_Quick();
//			
////			OLD_Back_State = Back_State;
////			Back_Count = 0;
//			Back_State = Quick_UP ;
//		}
		

//		//后轮缓慢增压
//		if((LB_Target - LB_PRE < 500 && LB_Target - LB_PRE > 100) || (RB_Target - RB_PRE < 500 && RB_Target - RB_PRE > 50))  
//		{
////			Back_Pressure_Up_Slow();
//			
////			OLD_Back_State = Back_State;
////			Back_Count = 0;
//			Back_State = Slow_UP ;
//		}
		
		
//		//后轮缓慢降压
//		if((LB_PRE - LB_Target > 50 && LB_PRE - LB_Target < 500) ||	(RB_PRE - RB_Target > 50 && RB_PRE - RB_Target < 500)) 
//		{
////			Back_Pressure_Down_Slow();
//			
////			OLD_Back_State = Back_State;
////			Back_Count = 0;
//			Back_State = Slow_DOWN ;
//		}
		
		
//		//后轮快速降压
//		if(LB_PRE - LB_Target > 500 || RB_PRE - RB_Target > 500) 
//		{
////			Back_Pressure_Down_Quick();
//			
////			OLD_Back_State = Back_State;
////			Back_Count = 0;
//			Back_State = Quick_DOWN ;
//		}
		
		//前轮释放压力
//		if(Absolute_Difference(LF_PRE , LF_Target) < 100 && Absolute_Difference(RF_PRE , RF_Target) < 100)
//		{
////			Front_Release();
//			Front_State = Release;
////			Front_Count = 0;
//		}
		
//		//后轮释放压力
//		if(Absolute_Difference(LB_PRE , LB_Target) < 100 && Absolute_Difference(RB_PRE , RB_Target) < 100)
//		{
////			Back_Release();
//			Back_State = Release;
////			Back_Count = 0;
//		}
		
		
		
		if(OLD_Front_State != Front_State)
		{
			Front_Count = 0;
		}
//	}
	 vTaskDelay(75);	
		
    } 
   
}



//状态切换任务函数
void State_Switch_Task(void *pvParameters)
{
    while(1)
    {
//		Set_PUMP_PWM(50)
//		LED3_FLASH();
//		if(ESC_Front_Switch_Flag)
//		{
//			if(Front_State == Quick_UP) 		Front_Pressure_Up_Quick();
//			if(Front_State == Slow_UP) 			Front_Pressure_Up_Slow();
//			if(Front_State == Quick_DOWN)	    Front_Pressure_Down_Quick();
//			if(Front_State == Slow_DOWN) 		Front_Pressure_Down_Slow();
//		}
//		
//		if(ESC_Back_Switch_Flag)
//		{
//			if(Back_State == Quick_UP)       Back_Pressure_Up_Quick();
//			if(Back_State == Slow_UP) 		 Back_Pressure_Up_Slow();
//			if(Back_State == Quick_DOWN)	 Back_Pressure_Down_Quick();
//			if(Back_State == Slow_DOWN)		 Back_Pressure_Down_Slow();
//		}
//		vTaskDelay(75);
//	vTaskDelay(75);
      

    }    
}



//ESC的REST任务函数
void ESC_Rest_Task(void *pvParameters)
{
    while(1)
    {
//		Set_PUMP_PWM(500)
//		switch (Back_State)
//		{
//			case Quick_UP :
//				Back_Pressure_Up_Quick();
//				break;
// 			case Slow_UP :
//				Back_Pressure_Up_Slow();
//				break;
//			case Quick_DOWN :
//				Back_Pressure_Down_Quick();
//				break;
//			case Slow_DOWN :
//				Back_Pressure_Down_Slow();
//				break;
//			case Release :
//				Back_Release();
//				break;			
//		}
		
		
		switch (Front_State)
		{
			case Quick_UP :
				Front_Pressure_Up_Quick();
				break;
 			case Slow_UP :
				Front_Pressure_Up_Slow();
				break;
			case Quick_DOWN :
				Front_Pressure_Down_Quick();
				break;
			case Slow_DOWN :
				Front_Pressure_Down_Slow();
				break;
			case Release :
				Front_Release();
				break;	
			case Finish :
				Front_Finish();
				break;
		}

    }    
}

 


