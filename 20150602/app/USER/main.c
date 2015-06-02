#include "delay.h"
#include "sys.h"
#include "includes.h" 	
#include "Task.h"

int main(void)
{	 
	NVIC_Configuration(); 	// 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    	// 延时函数初始化	  
	
	OSInit();  	 			// 初始化UCOSII		 			  
 
	OSTaskCreate(start_task, 
				(void *)0, 
				(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],
				START_TASK_PRIO);			// 创建起始任务
	
	OSStart();	    
}

