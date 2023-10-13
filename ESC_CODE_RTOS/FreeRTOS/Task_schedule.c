#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"


//@开始任务
//任务优先级
#define START_TASK_PRIO		 1
//任务堆栈大小	
#define START_STK_SIZE 		32 
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void Start_Task(void *pvParameters);
 




//@CAN发送任务 
//任务优先级
#define CAN_Send_TASK_PRIO		 1
//任务堆栈大小	
#define CAN_Send_STK_SIZE 		128  
//任务句柄
TaskHandle_t CAN_SendTask_Handler;
//任务函数
void CAN_Send_Task(void *pvParameters);




//开机Timng控制任务 
//任务优先级
#define Turn_on_Timing_TASK_PRIO		 4
//任务堆栈大小	
#define Turn_on_Timing_STK_SIZE 		64  
//任务句柄
TaskHandle_t Turn_on_TimingTask_Handler;
//任务函数
void Turn_on_Timing_Task(void *pvParameters);




//@PWM任务 
//任务优先级
#define PWM_TASK_PRIO		 2
//任务堆栈大小	
#define PWM_STK_SIZE 		128  
//任务句柄
TaskHandle_t PWMTask_Handler;
//任务函数
void PWM_Task(void *pvParameters);



//@ESC的REST任务函数
//任务优先级
#define ESC_Rest_TASK_PRIO		 1
//任务堆栈大小	
#define ESC_Rest_STK_SIZE 		32  
//任务句柄
TaskHandle_t ESC_RestTask_Handler;
//任务函数
void ESC_Rest_Task(void *pvParameters);






//开始任务任务函数
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL();//进入临界区
          					
		
	 //创建CAN发送任务
    xTaskCreate((TaskFunction_t )CAN_Send_Task,     
                (const char*    )"CAN_Send_Task",   
                (uint16_t       )CAN_Send_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )CAN_Send_TASK_PRIO,
                (TaskHandle_t*  )&CAN_SendTask_Handler);
				
	 //创建开机Timing任务
    xTaskCreate((TaskFunction_t )Turn_on_Timing_Task,     
                (const char*    )"Turn_on_Timing_Task",   
                (uint16_t       )Turn_on_Timing_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Turn_on_Timing_TASK_PRIO,
                (TaskHandle_t*  )&Turn_on_TimingTask_Handler);    				
  
				
	 //创建PWM任务
    xTaskCreate((TaskFunction_t )PWM_Task,     
                (const char*    )"PWM_Task",   
                (uint16_t       )PWM_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )PWM_TASK_PRIO,
                (TaskHandle_t*  )&PWMTask_Handler);  
				

				
				
	//ESC的REST任务函数	
    xTaskCreate((TaskFunction_t )ESC_Rest_Task,     
                (const char*    )"ESC_Rest_Task",   
                (uint16_t       )ESC_Rest_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )ESC_Rest_TASK_PRIO,
                (TaskHandle_t*  )&ESC_RestTask_Handler); 
				
   				
				
    vTaskDelete(StartTask_Handler); //删除开始任务
                 
    taskEXIT_CRITICAL();            //退出临界区
}

void First_Task(void)
{
	  //创建开始任务
  xTaskCreate((TaskFunction_t )Start_Task,            //任务函数
              (const char*    )"Start_Task",          //任务名称
              (uint16_t       )START_STK_SIZE,        //任务堆栈大小
              (void*          )NULL,                  //传递给任务函数的参数
              (UBaseType_t    )START_TASK_PRIO,       //任务优先级
              (TaskHandle_t*  )&StartTask_Handler);   //任务句柄  
              
  vTaskStartScheduler();          //开启任务调度
  
}


