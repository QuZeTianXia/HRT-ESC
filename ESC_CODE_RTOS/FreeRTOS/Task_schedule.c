#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"


//@��ʼ����
//�������ȼ�
#define START_TASK_PRIO		 1
//�����ջ��С	
#define START_STK_SIZE 		32 
//������
TaskHandle_t StartTask_Handler;
//������
void Start_Task(void *pvParameters);
 




//@CAN�������� 
//�������ȼ�
#define CAN_Send_TASK_PRIO		 1
//�����ջ��С	
#define CAN_Send_STK_SIZE 		128  
//������
TaskHandle_t CAN_SendTask_Handler;
//������
void CAN_Send_Task(void *pvParameters);




//����Timng�������� 
//�������ȼ�
#define Turn_on_Timing_TASK_PRIO		 4
//�����ջ��С	
#define Turn_on_Timing_STK_SIZE 		64  
//������
TaskHandle_t Turn_on_TimingTask_Handler;
//������
void Turn_on_Timing_Task(void *pvParameters);




//@PWM���� 
//�������ȼ�
#define PWM_TASK_PRIO		 2
//�����ջ��С	
#define PWM_STK_SIZE 		128  
//������
TaskHandle_t PWMTask_Handler;
//������
void PWM_Task(void *pvParameters);



//@ESC��REST������
//�������ȼ�
#define ESC_Rest_TASK_PRIO		 1
//�����ջ��С	
#define ESC_Rest_STK_SIZE 		32  
//������
TaskHandle_t ESC_RestTask_Handler;
//������
void ESC_Rest_Task(void *pvParameters);






//��ʼ����������
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL();//�����ٽ���
          					
		
	 //����CAN��������
    xTaskCreate((TaskFunction_t )CAN_Send_Task,     
                (const char*    )"CAN_Send_Task",   
                (uint16_t       )CAN_Send_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )CAN_Send_TASK_PRIO,
                (TaskHandle_t*  )&CAN_SendTask_Handler);
				
	 //��������Timing����
    xTaskCreate((TaskFunction_t )Turn_on_Timing_Task,     
                (const char*    )"Turn_on_Timing_Task",   
                (uint16_t       )Turn_on_Timing_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Turn_on_Timing_TASK_PRIO,
                (TaskHandle_t*  )&Turn_on_TimingTask_Handler);    				
  
				
	 //����PWM����
    xTaskCreate((TaskFunction_t )PWM_Task,     
                (const char*    )"PWM_Task",   
                (uint16_t       )PWM_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )PWM_TASK_PRIO,
                (TaskHandle_t*  )&PWMTask_Handler);  
				

				
				
	//ESC��REST������	
    xTaskCreate((TaskFunction_t )ESC_Rest_Task,     
                (const char*    )"ESC_Rest_Task",   
                (uint16_t       )ESC_Rest_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )ESC_Rest_TASK_PRIO,
                (TaskHandle_t*  )&ESC_RestTask_Handler); 
				
   				
				
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
                 
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

void First_Task(void)
{
	  //������ʼ����
  xTaskCreate((TaskFunction_t )Start_Task,            //������
              (const char*    )"Start_Task",          //��������
              (uint16_t       )START_STK_SIZE,        //�����ջ��С
              (void*          )NULL,                  //���ݸ��������Ĳ���
              (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
              (TaskHandle_t*  )&StartTask_Handler);   //������  
              
  vTaskStartScheduler();          //�����������
  
}


