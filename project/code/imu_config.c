#include "YJX.h"
#include "rtthread.h"
#include "math.h"

rt_event_t gyro_event;

GyroPara GyroOffset;
AccPara AccOffset;
ICMdara icm_data; //处理后的六轴数据
//extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z; // ICM20602采集的六轴数值
//extern int16 imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z;

extern int16 icm20602_gyro_x, icm20602_gyro_y , icm20602_gyro_z;            // 三轴陀螺仪数据      gyro (陀螺仪)
extern int16 icm20602_acc_x, icm20602_acc_y, icm20602_acc_z;               // 三轴加速度计数据    acc (accelerometer 加速度计)

//陀螺仪零飘初始化
void GyroOffset_Init(void)
{
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
    uint16 i = 0;
    system_delay_ms(100);
    for (i = 0; i < 205; i++) {
//        imu660ra_get_gyro();
			icm20602_get_gyro();

        // GyroOffset.Xdata += icm_gyro_x;
        // GyroOffset.Ydata += icm_gyro_y;
        if (i > 4) {
            GyroOffset.Zdata += icm20602_gyro_z;
            GyroOffset.gyroz[i - 5] = icm20602_gyro_z;
        }

        // printf("%d\n", icm_gyro_z);

        system_delay_ms(5);
    }

    // GyroOffset.Xdata /= 5;
    // GyroOffset.Ydata /= 5;
    GyroOffset.Zdata /= 200;
    for (i = 0; i < 200; i++) {
        icm_data.fangcha += pow(GyroOffset.gyroz[i] - GyroOffset.Zdata, 2);
    }
    icm_data.fangcha /= 200;
    icm_data.yaw = 0; //开机一段时间后给他置0
		
}

//加速度计零飘初始化
void AccOffset_Init(void)
{
    AccOffset.Xdata = 0;
    AccOffset.Ydata = 0;
    uint16 i = 0;
    system_delay_ms(100);
    for (i = 0; i < 205; i++) {
//        imu660ra_get_acc();
			icm20602_get_acc();

        if (i > 4) {
            AccOffset.Xdata += icm20602_acc_x;
            AccOffset.Ydata += icm20602_acc_y;
            AccOffset.accx[i - 5] = icm20602_acc_x;
            AccOffset.accy[i - 5] = icm20602_acc_y;
        }

        // printf("%d\n", icm_gyro_z);

        system_delay_ms(5);
    }

    AccOffset.Xdata /= 200;
    AccOffset.Ydata /= 200;
    AccOffset.Xdata -= 2;
    AccOffset.Ydata -= 2;
    for (i = 0; i < 200; i++) {
        AccOffset.fangchaX += pow(AccOffset.accx[i] - AccOffset.Xdata, 2);
        AccOffset.fangchaY += pow(AccOffset.accy[i] - AccOffset.Ydata, 2);
    }
    AccOffset.fangchaX /= 200;
    AccOffset.fangchaY /= 200;
//    INS_para.Now_Vx = 0; //开机一段时间后给他置0
//    INS_para.Now_Vy = 0;
}

//void imu_entry (void *parameter)
//{
//	while(1){
//		if(rt_event_recv
//		(gyro_event,													// 事件控制块
//		(ICM20602gyro_ReadOver | FirstHubu_InitOver),							// 事件标志3和事件标志5
//		(RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR),				// 事件与触发，接收完成后清除事件标志位
//		RT_WAITING_FOREVER,										// 一直等待
//		RT_NULL) == RT_EOK)
//		{
//			
//		}
//	}

//}

//imu相关线程创建以及事件集创建
//void imu_config_init()
//{
//	gyro_event = rt_event_create("gyro_event", RT_IPC_FLAG_FIFO);
//	
//	rt_thread_t tid;
//	
//		// 创建动态线程
//	tid = rt_thread_create("imu",							// 线程名称
//		imu_entry,											// 线程入口函数
//		RT_NULL,												// 线程参数
//		1024,													// 1024 个字节的栈空间
//		12,														// 线程优先级为8，数值越小，优先级越高，0为最高优先级。
//																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
//		5);														// 时间片为5

//	if(tid != RT_NULL)											// 线程创建成功
//	{
//		rt_kprintf("create thread1 OK\n");
//		//运行该线程
//		rt_thread_startup(tid);
//	}
//}
