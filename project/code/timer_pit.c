#include "YJX.h"

rt_sem_t speedRead_sem = RT_NULL;							// 创建指向速度读取结束的信号量的指针
rt_sem_t imu660raGet_sem = RT_NULL;							// 创建指向陀螺仪和加速度计读取结束的信号量的指针
rt_sem_t binary_control_sem = RT_NULL;
rt_sem_t me_control_sem = RT_NULL;
rt_sem_t tianoo_control_sem = RT_NULL;

extern PositionalPIDpara PlanXpara;
extern PositionalPIDpara PlanYpara;
extern float XXstopgo;
extern float YXstopgo;
extern float XVgo;
extern float YVgo;
extern INS2DPara INS_para;

extern int16 binary_control;
extern rt_sem_t putPhoto_control_sem;

void timer1_pit_entry(void *parameter)
{
	//采集编码器数据 给四轮赋值 逆解算车模速度
  SpeedRead();
	rt_sem_release(speedRead_sem);
	
	if(binary_control == 1)
		rt_sem_release(binary_control_sem);
	else if(binary_control == 0)
		rt_sem_release(me_control_sem);
	else if(binary_control == 2)
		rt_sem_release(tianoo_control_sem);
	else if(binary_control == 3){
		rt_sem_release(binary_control_sem);
		rt_sem_release(tianoo_control_sem);
	}else if(binary_control == 4){
		rt_sem_release(putPhoto_control_sem);
	}else if(binary_control == 5){
		rt_sem_release(BackCarport_control_sem);
	}
	
//	if(text_flag)
//		rt_sem_release(text_sem);
	
	if(!binary_control){
		XVgo = -PositionalPID(&PlanXpara, INS_para.Now_X, XXstopgo, planPIDMax, 1);
		YVgo = -PositionalPID(&PlanYpara, INS_para.Now_Y, YXstopgo, planPIDMax, 1);
	}
}

extern float expect_w;
extern INS2DPara INS_para;
extern float Anglewant;
extern int anglePidMax;
extern ICMdara icm_data; 
extern PositionalPIDpara anglePIDpara;

void timer2_pit_entry(void *parameter)
{
	
//	imu660ra_get_gyro();
//	imu660ra_get_acc();
	icm20602_get_gyro();
	icm20602_get_acc();
	
	FristHubu_Filer();
	expect_w = PositionalPID(&anglePIDpara, icm_data.yaw, Anglewant, anglePidMax, 0);
	
//	rt_sem_release(imu660raGet_sem);
}

void timer_pit_init(void)
{
	speedRead_sem = rt_sem_create("speedRead_sem", 0 ,RT_IPC_FLAG_FIFO);
	imu660raGet_sem = rt_sem_create("imu660raGet_sem", 0 ,RT_IPC_FLAG_FIFO);
	binary_control_sem = rt_sem_create("binary_control_sem", 0 ,RT_IPC_FLAG_FIFO);
	me_control_sem = rt_sem_create("rt_sem_create", 0, RT_IPC_FLAG_FIFO);
	tianoo_control_sem = rt_sem_create("tianoo_control_sem", 0, RT_IPC_FLAG_FIFO);
	
    rt_timer_t timer;
    
    //创建一个定时器 周期运行
    timer = rt_timer_create("timer1", timer1_pit_entry, RT_NULL, 2, RT_TIMER_FLAG_PERIODIC);
    //启动定时器
    if(RT_NULL != timer)
    {
        rt_timer_start(timer);
    }

		timer = rt_timer_create("timer2", timer2_pit_entry, RT_NULL, 5, RT_TIMER_FLAG_PERIODIC);
		if(RT_NULL != timer)
    {
        rt_timer_start(timer);
    }
    
}