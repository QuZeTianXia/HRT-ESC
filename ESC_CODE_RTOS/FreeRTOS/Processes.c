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
				
				Front_State = Quick_UP;
				Back_State = Quick_UP;
				
				vTaskDelay(7000);
				
				ESC_DISABLE();
				
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
					
					Front_State = Quick_UP;
					Back_State = Quick_UP;
					
					vTaskDelay(5000);
					
					ESC_DISABLE();
					
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
					
					Front_State = Finish;
					Back_State = Finish;
					
					vTaskDelay(3000);
					
					ESC_DISABLE();
					
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




//PWM任务函数
void PWM_Task(void *pvParameters)
{
	
    while(1)
    {	
		LED2_FLASH();
		
		OLD_Front_State = Front_State;
		OLD_Back_State = Back_State;
		
		if(!Emergency_Flag)
		{
			
			//前轮快速增压
			if(LF_Target - LF_PRE > 500 || RF_Target - RF_PRE > 500) 
			{
				Front_State	= Quick_UP;		
			}
			
			
			//前轮缓慢增压
			if((LF_Target - LF_PRE < 500 && LF_Target - LF_PRE > 50 ) || (RF_Target - RF_PRE < 500 && RF_Target - RF_PRE > 50 )) 
			{
				Front_State	= Slow_UP;	
			}

							
			//前轮快速降压
			if(LF_PRE - LF_Target > 300 || RF_PRE - RF_Target > 300)
			{
				Front_State	= Quick_DOWN;
			}

			
			//后轮快速增压
			if(LB_Target - LB_PRE > 500 || RB_Target - RB_PRE > 500)
			{
				Back_State = Quick_UP ;
			}
			

			//后轮缓慢增压
			if((LB_Target - LB_PRE < 500 && LB_Target - LB_PRE > 50) || (RB_Target - RB_PRE < 500 && RB_Target - RB_PRE > 50))  
			{
				Back_State = Slow_UP ;
			}
			
				
			//后轮快速降压
			if(LB_PRE - LB_Target > 300 || RB_PRE - RB_Target > 300) 
			{
				Back_State = Quick_DOWN ;
			}
			
			
			if(OLD_Front_State != Front_State)
			{
				Front_Count = 0;
			}
		}
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

		}

    }    
}

 


