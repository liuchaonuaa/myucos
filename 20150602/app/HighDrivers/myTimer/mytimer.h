#ifndef __MYTIMER_H
#define __MYTIMER_H

#define TIMER3_TASK_20MS	 	((u32)(20/10))
#define TIMER3_TASK_50MS	 	((u32)(50/10))
#define TIMER3_TASK_70MS		((u32)(70/10))
#define TIMER3_TASK_100MS	 	((u32)(100/10))
#define TIMER3_TASK_200MS	 	((u32)(200/10))
#define TIMER3_TASK_500MS	 	((u32)(500/10))
#define TIMER3_TASK_1_SECOND 	((u32)(1000/10*1))
#define TIMER3_TASK_2_SECOND 	((u32)(1000/10*2))
#define TIMER3_TASK_3_SECOND 	((u32)(1000/10*3))
#define TIMER3_TASK_5_SECOND 	((u32)(1000/10*5))
#define TIMER3_TASK_10_SECOND 	((u32)(1000/10*10))
#define TIMER3_TASK_30_SECOND 	((u32)(1000/10*30))
#define TIMER3_TASK_2_MINUTE 	((u32)(1000/10*60*2))
#define TIMER3_TASK_10_MINUTE	((u32)(1000/10*60*10))

void Time2_Init(void);
void Time3_Init(void);
u32 timer3_GetCount(void);
void timer3_ResetCount(void);

#endif
