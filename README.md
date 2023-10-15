# 更新日志      
[![O1jPRb.png](https://ooo.0x0.ooo/2023/10/13/O1jPRb.png)](https://img.tg/image/O1jPRb)  
+ 2023/10/15/9:35  
梳理了逻辑，增加了使用CAN改变加压力度配制参数的功能。  
	+ Data[0] = 0xAA ---> Data[1] = Quick_Accelerate;  
	+ Data[0] = 0xBB ---> Data[1] = Slow_Accelerate;   
下一步计划增加P控制
+ 2023/10/13/21:34  
更新了23D四轮制动的版本，但未经测试。
+ 2023/10/13/21:25   
更新了23D刚刚可用的只有前轮制动的版本。