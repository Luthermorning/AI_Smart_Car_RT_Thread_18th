#include "YJX.h"

int aaa = -1;
rt_sem_t text_sem = RT_NULL;
int text_flag = 0;
extern float SpeedLoopWant_Lf;
extern float SpeedLoopWant_Rf;
extern float SpeedLoopWant_Lb;
extern float SpeedLoopWant_Rb;
void text_debug (void *parameter){
	gpio_init(B16, GPO, 0, GPO_PUSH_PULL);//右
	gpio_init(B17, GPO, 0, GPO_PUSH_PULL);//前
	gpio_init(B18, GPO, 0, GPO_PUSH_PULL);//左
	gpio_init(B19, GPO, 0, GPO_PUSH_PULL);//后
	while(1){
		if((rt_sem_take(text_sem, RT_WAITING_FOREVER)) == RT_EOK){
			putPhoto_motion(aaa);
			if(tdong==80){
				SpeedLoopWant_Lf = 0;
				SpeedLoopWant_Rf = 0;
				SpeedLoopWant_Lb = 0;
				SpeedLoopWant_Rb = 0;
				text_flag = 0;
			}
		}
	}
}

void text_init(void)
{
	rt_thread_t tid;
	text_sem = rt_sem_create("text_sem", 0, RT_IPC_FLAG_FIFO);

	tid = rt_thread_create("text_debug",							// 线程名称
		text_debug,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 1024 个字节的栈空间
		20,														// 线程优先级为12，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		5);														// 时间片为5

	if(tid != RT_NULL)											// 线程创建成功
	{
		//运行该线程
		rt_thread_startup(tid);
	}
}