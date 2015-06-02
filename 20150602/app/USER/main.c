#include "delay.h"
#include "sys.h"
#include "includes.h" 	
#include "Task.h"

int main(void)
{	 
	NVIC_Configuration(); 	// ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	// ��ʱ������ʼ��	  
	
	OSInit();  	 			// ��ʼ��UCOSII		 			  
 
	OSTaskCreate(start_task, 
				(void *)0, 
				(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],
				START_TASK_PRIO);			// ������ʼ����
	
	OSStart();	    
}

