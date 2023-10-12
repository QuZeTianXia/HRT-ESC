#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "wdg.h"
 
 
/************************************************
 ALIENTEK战舰STM32开发板实验6
 独立看门狗实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

 int main(void)
 {		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();		  	 //初始化与LED连接的硬件接口
	KEY_Init();          //按键初始化	 
	delay_ms(500);   	 //让人看得到灭
	IWDG_Init(4,625);    //与分频数为64,重载值为625,溢出时间为1s	   
	LED0=0;				 //点亮LED0
	while(1)
	{
		if(KEY_Scan(0)==WKUP_PRES)
		{
			IWDG_Feed();//如果WK_UP按下,则喂狗
		}
		delay_ms(10);
	};	 
}
  

