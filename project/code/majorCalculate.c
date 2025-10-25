#include "YJX.h"

extern float expect_w;

extern PositionalPIDpara anglePIDpara;
extern PositionalPIDpara PlanXpara;
extern PositionalPIDpara PlanYpara;
extern INS2DPara INS_para;
extern float Anglewant;
extern int anglePidMax;
extern ICMdara icm_data; 

extern float XXstopgo;
extern float YXstopgo;

extern rt_sem_t speedRead_sem;
extern rt_sem_t imu660raGet_sem;

extern int16 binary_control;

float XVgo = 0;
float YVgo = 0;

void speedLoop_and_INS_Calculate_entry (void *parameter)
{
	while(1)
	{
		if((rt_sem_take(speedRead_sem, RT_WAITING_FOREVER)) == RT_EOK){
			SpeedPIDdutySet_AND_Get4wheels();
			Update_INS(0.002);
		}
	}
}

void angleLoop_Calculate_entry (void *parameter)
{
	while(1)
	{
		if((rt_sem_take(imu660raGet_sem, RT_WAITING_FOREVER)) == RT_EOK){
			FristHubu_Filer();
			expect_w = PositionalPID(&anglePIDpara, icm_data.yaw, Anglewant, anglePidMax, 0);
		}
	}
}

void majorCalculate_init()
{
	
	rt_thread_t tid;
	
		// 创建速度环和2D惯导更新线程
	tid = rt_thread_create("speedLoop_and_INS_Calculate",							// 线程名称
		speedLoop_and_INS_Calculate_entry,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 1024 个字节的栈空间
		5,														// 线程优先级为12，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		5);														// 时间片为5

	if(tid != RT_NULL)											// 线程创建成功
	{
//		rt_kprintf("create thread1 OK\n");
		//运行该线程
		rt_thread_startup(tid);
	}
	
	
	// 创建角度环线程
	tid = rt_thread_create("angleLoop_Calculate",							// 线程名称
		angleLoop_Calculate_entry,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 1024 个字节的栈空间
		7,														// 线程优先级为12，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		10);														// 时间片为5

	if(tid != RT_NULL)											// 线程创建成功
	{
//		rt_kprintf("create thread1 OK\n");
		//运行该线程
		rt_thread_startup(tid);
	}
}