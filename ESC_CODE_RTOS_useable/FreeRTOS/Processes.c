#include "stm32f10x.h"                  // Device header
#include "Global_variable.h"


//初始化函数
void Hardware_Init(void)
{	
	GPIOS_Init();
	PWM_Init();
	KEY_Init();
	CAN1_Init();
	Store_Init();
	
	Quick_Accelerate = 0x3C;
	Slow_Accelerate = 0X14;
	Keep_Accelerate = 0x08;
	Store_Save();
	
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
		CAN_Message[5] = Quick_Accelerate;
		CAN_Message[6] = Slow_Accelerate;
		
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
				ESC_Drive_flag = 0;
				
				ESC_Release();
			}
		
					
			if(ECU_State == ECU_State_AS_Slow_Stop)
			{
				normal_mode = 0;
								
				ESC_ENABLE();
				ESC_Drive_flag = 1;
								
				LF_Target = 3000;
				RF_Target = 3000;
				LB_Target = 3000;
				RB_Target = 3000;
				
				vTaskDelay(7000);
				
				ESC_DISABLE();
				ESC_Drive_flag = 0;
				
				LF_Target = 0;
				RF_Target = 0;
				LB_Target = 0;
				RB_Target = 0;
				
				Front_State = Release;
				Back_State = Release;	
								
				normal_mode = 1;
			}
			
			if(ECU_State == ECU_State_AS_Emergency)
			{
				normal_mode = 0;
				vTaskDelay(1000);
				if(LF_PRE < 2000 || RF_PRE < 2000 || LB_PRE < 2000 ||RB_PRE < 2000)
				{				
					ESC_ENABLE();
					ESC_Drive_flag = 1;
										
					LF_Target = 3000;
					RF_Target = 3000;
					LB_Target = 3000;
					RB_Target = 3000;
					
					vTaskDelay(5000);
					
					ESC_DISABLE();
					ESC_Drive_flag = 0;
					
					LF_Target = 0;
					RF_Target = 0;
					LB_Target = 0;
					RB_Target = 0;
					
					Front_State = Release;
					Back_State = Release;	
										
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
								
					ESC_ENABLE();
					ESC_Drive_flag = 1;
										
					LF_Target = 3000;
					RF_Target = 3000;
					LB_Target = 3000;
					RB_Target = 3000;
					
					vTaskDelay(7000);
					
					ESC_DISABLE();
					ESC_Drive_flag = 0;
				
					LF_Target = 0;
					RF_Target = 0;
					LB_Target = 0;
					RB_Target = 0;
					
					Front_State = Release;
					Back_State = Release;	
					
					normal_mode = 1;
			}
		Ros_State_Switch_Flag = 0;
		}
		
		vTaskDelay(75);
    }    
}


//PWM任务函数
void PWM_Task(void *pvParameters)
{
	
    while(1)
    {	
		LED2_FLASH();
		//前轮快速增压
		if(LF_Target - LF_PRE > 500 || RF_Target - RF_PRE > 500) 
		{
			Front_State	= Quick_UP;	
			Front_PWM_Value = Quick_Accelerate;			
		}
				
		//前轮缓慢增压
		if((LF_Target - LF_PRE < 500 && LF_Target - LF_PRE > 100 ) || (RF_Target - RF_PRE < 500 && RF_Target - RF_PRE > 100 )) 
		{
			Front_State	= Slow_UP;
			Front_PWM_Value = Slow_Accelerate;			
		}

		//前轮快速降压
		if(LF_PRE - LF_Target > 300 || RF_PRE - RF_Target > 300)
		{
			Front_State	= Quick_DOWN;
			Front_PWM_Value = 0;
		}
		
		//后轮快速增压
		if(LB_Target - LB_PRE > 500 || RB_Target - RB_PRE > 500)
		{
			Back_State = Quick_UP ;
			Front_PWM_Value = Quick_Accelerate;
		}
		
		//后轮缓慢增压
		if((LB_Target - LB_PRE < 500 && LB_Target - LB_PRE > 100) || (RB_Target - RB_PRE < 500 && RB_Target - RB_PRE > 100))  
		{
			Back_State = Slow_UP ;
			Back_PWM_Value = Slow_Accelerate;
		}	

		//后轮快速降压
		if(LB_PRE - LB_Target > 300 || RB_PRE - RB_Target > 300) 
		{
			Back_State = Quick_DOWN ;
			Back_PWM_Value = 0;
		}
		
		//前轮保持压力
		if(LF_PRE > 600 &&
		   RF_PRE > 600 &&
		   ((Positive_Difference(LF_PRE , LF_Target) < 200 && Positive_Difference(LF_PRE , LF_Target) > 0 ) ||
		      
		   (Positive_Difference(RF_PRE , RF_Target) < 200 && Positive_Difference(RF_PRE , RF_Target) > 0))
		   )
		{
			Front_State = Keep;
			Front_PWM_Value = Keep_Accelerate;
		}
		
		//后轮保持压力
		if(LB_PRE > 600 &&
		   RB_PRE > 600 &&
		   ((Positive_Difference(LB_PRE , LB_Target) < 200 && Positive_Difference(LB_PRE , LB_Target) > 0 ) ||
		      
		   (Positive_Difference(RB_PRE , RB_Target) < 200 && Positive_Difference(RB_PRE , RB_Target) > 0))
		   )
		{
			Back_State = Keep;
			Back_PWM_Value = Keep_Accelerate;
		}
		
		//后轮保持压力
		if(LB_PRE < 500 &&
		   RB_PRE < 500 &&
		   ((Absolute_Difference(LB_PRE , LB_Target) < 200  ) && (Absolute_Difference(RB_PRE , RB_Target) < 200 ))
		   )
		{
			Back_State = Release;
			Back_PWM_Value = 0;
		}

		//前轮保持压力
		if(LF_PRE < 500 &&
		   RF_PRE < 500 &&
		   ((Absolute_Difference(LF_PRE , LF_Target) < 200 ) && (Absolute_Difference(RF_PRE , RF_Target) < 200 ))
		   )
		{
			Front_State = Release;
			Front_PWM_Value = 0;
		}		
		
		if(LF_PRE>3800||RF_PRE>3800||LB_PRE>3800||RB_PRE>3800)
		{
			ESC_Release();
		}
		
		
		
//		//后轮保持压力
//		if(LB_PRE > 600 &&
//		   RB_PRE > 600 &&
//		   Positive_Difference(LB_PRE , LB_Target) < 200 && 
//		   Positive_Difference(LB_PRE , LB_Target) > 0   && 
//		   Positive_Difference(RB_PRE , RB_Target) < 200 &&
//		   Positive_Difference(RB_PRE , RB_Target) > 0     )
//		{
//			Back_State = Keep;
//		}

		//前轮缓慢降压
//		if((LF_PRE - LF_Target > 100  && LF_PRE - LF_Target < 500) ||	(RF_PRE - RF_Target > 50 && RF_PRE - RF_Target < 500))
//		{
////			Front_Pressure_Down_Slow();
//			
////			OLD_Front_State = Front_State;
////			Front_Count = 0;
//			Front_State	= Slow_DOWN;
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
		
		
	 vTaskDelay(75);	
		
    } 
   
}



//ESC的REST任务函数
void ESC_Rest_Task(void *pvParameters)
{
    while(1)
    {

		switch (Back_State)
		{
			case Quick_UP :
				Back_Pressure_Up_Quick();
				break;
 			case Slow_UP :
				Back_Pressure_Up_Slow();
				break;
			case Quick_DOWN :
				Back_Pressure_Down_Quick();
				break;
			case Slow_DOWN :
				Back_Pressure_Down_Slow();
				break;
			case Release :
				Back_Release();
				break;
			case Keep :
				Back_Keep();
				break;			
		}
		
		
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
			case Keep :
				Front_Keep();
				break;			

		}
		
		if(Front_PWM_Value > Back_PWM_Value)  Set_PUMP_PWM(Front_PWM_Value)
		else Set_PUMP_PWM(Back_PWM_Value)

    }    
}

 


