#include "YJX.h"
#include "math.h"

bool yellowLinesend = 0;
int putPointX[6] = {left_manyPut, behind_manyPut};
int putPointY[6] = {10, 10};
uint8 count = 0;
float Xputgo;
float Yputgo;
float XVVVgo;
float YVVVgo;
rt_sem_t putPhoto_control_sem = RT_NULL;
rt_sem_t BackCarport_control_sem = RT_NULL;
bool countOver = 0;

int Xcarportgo;
float Ycarportgo;
float XVCgo;
float YVCgo;

extern int16 binary_control;
extern INS2DPara INS_para;
extern PositionalPIDpara PlanXpara;
extern PositionalPIDpara PlanYpara;
extern float expect_w;
extern float SpeedLoopWant_Lf;
extern float SpeedLoopWant_Rf;
extern float SpeedLoopWant_Lb;
extern float SpeedLoopWant_Rb;
extern GetV getv;

int put_mode = 0;
int put_dong;

void putPhoto_entry(void *parameter)
{
	while(1){
		if((rt_sem_take(putPhoto_control_sem, RT_WAITING_FOREVER)) == RT_EOK){
			pwm_set_duty(PWM1_MODULE3_CHA_D0, 400);
			pwm_set_duty(PWM1_MODULE3_CHB_D1, 750);
			if(put_mode==0){
				if(count>1){
					SpeedLoopWant_Lf = -(expect_w * (a + b));
					SpeedLoopWant_Rf = -(expect_w * (a + b));
					SpeedLoopWant_Lb = -(expect_w * (a + b));
					SpeedLoopWant_Rb = -(expect_w * (a + b));
					INS_para.Now_X = 0;
					INS_para.Now_Y = 0;
					
					XVVVgo = 0;
					YVVVgo = 0;
					Xputgo = 0;
					Yputgo = 0;
					binary_control = 5;
				}
				if((!countOver)&&(count<2)){
					Xputgo = coordinateTransformationX(putPointX[count], 0) - INS_para.all_X;
					Yputgo = coordinateTransformationY(putPointY[count], 0) - INS_para.all_Y;
					countOver = 1;
				}

				XVVVgo = -PositionalPID(&PlanXpara, INS_para.Now_X, Xputgo, planPIDMax, 1);
				YVVVgo = -PositionalPID(&PlanYpara, INS_para.Now_Y, Yputgo, planPIDMax, 1);
				
				SpeedLoopWant_Lf = -(XVVVgo + YVVVgo + expect_w * (a + b));
				SpeedLoopWant_Rf = -(XVVVgo - YVVVgo + expect_w * (a + b));
				SpeedLoopWant_Lb = (XVVVgo - YVVVgo - expect_w * (a + b));
				SpeedLoopWant_Rb = (XVVVgo + YVVVgo - expect_w * (a + b));
				
				if ((fabs(INS_para.Now_X) > (0.1 / 5.0) *fabs(Xputgo)) && (fabs(INS_para.Now_Y) > (0.1 / 5.0) *fabs(Yputgo))){
					if(count == 1){
						if(yellowLinesend==0){
							rt_mb_send(buzzer_mailbox, 400);
							printf("%d", 1);//该openart干活辣
							yellowLinesend = 1;
						}
					}
//					if(count == 0) {gpio_set_level(B16, 1);}
//					else if(count == 1) {gpio_set_level(B17, 1);}
//					else if(count == 2) {gpio_set_level(B18, 1);}

					if((fabs(getv.get_Vy) <= 0.5) && (fabs(getv.get_Vx) <= 0.5)){
						SpeedLoopWant_Lf = -(expect_w * (a + b));
						SpeedLoopWant_Rf = -(expect_w * (a + b));
						SpeedLoopWant_Lb = -(expect_w * (a + b));
						SpeedLoopWant_Rb = -(expect_w * (a + b));
						INS_para.Now_X = 0;
						INS_para.Now_Y = 0;
						put_mode = 1;
						
	//						putPhoto_motion(count);
					}
				}
			}else if(put_mode == 1){//放图片
//				if(put_dong == 0){
//					if(count == 0)
//						pwm_set_duty(PWM4_MODULE2_CHA_C30, 260);	
//				}
//				
//				if((count == 0)||(count == 1)){//抖一抖
//					if(put_dong<7){
//						SpeedLoopWant_Lf = -15-(expect_w * (a + b));
//						SpeedLoopWant_Rf = -15-(expect_w * (a + b));
//						SpeedLoopWant_Lb = 15-(expect_w * (a + b));
//						SpeedLoopWant_Rb = 15-(expect_w * (a + b));
//						if(count == 0) {gpio_set_level(B16, 1);}
//						else if(count == 1) {gpio_set_level(B17, 1);}
//						else if(count == 2) {gpio_set_level(B18, 1);}
//						system_delay_ms(100);
//						put_dong++;
//					}else if(7<=put_dong&&put_dong<14){
//						SpeedLoopWant_Lf = 15-(expect_w * (a + b));
//						SpeedLoopWant_Rf = 15-(expect_w * (a + b));
//						SpeedLoopWant_Lb = -15-(expect_w * (a + b));
//						SpeedLoopWant_Rb = -15-(expect_w * (a + b));
//						if(count == 0) {gpio_set_level(B16, 1);}
//						else if(count == 1) {gpio_set_level(B17, 1);}
//						else if(count == 2) {gpio_set_level(B18, 1);}
//						system_delay_ms(100);
//						put_dong++;
//					}else if(14<=put_dong&&put_dong<21){
//						SpeedLoopWant_Lf = -15-(expect_w * (a + b));
//						SpeedLoopWant_Rf = -15-(expect_w * (a + b));
//						SpeedLoopWant_Lb = 15-(expect_w * (a + b));
//						SpeedLoopWant_Rb = 15-(expect_w * (a + b));
//						if(count == 0) {gpio_set_level(B16, 1);}
//						else if(count == 1) {gpio_set_level(B17, 1);}
//						else if(count == 2) {gpio_set_level(B18, 1);}
//						system_delay_ms(100);
//						put_dong++;
//					}
//					else if(21<=put_dong&&put_dong<28){
//						SpeedLoopWant_Lf = 15-(expect_w * (a + b));
//						SpeedLoopWant_Rf = 15-(expect_w * (a + b));
//						SpeedLoopWant_Lb = -15-(expect_w * (a + b));
//						SpeedLoopWant_Rb = -15-(expect_w * (a + b));
//						if(count == 0) {gpio_set_level(B16, 1);}
//						else if(count == 1) {gpio_set_level(B17, 1);}
//						else if(count == 2) {gpio_set_level(B18, 1);}
//						system_delay_ms(100);
//						put_dong++;
//				}
//				}else if(count == 2){
//					if(put_dong<7){
//						SpeedLoopWant_Lf = -15-(expect_w * (a + b));
//						SpeedLoopWant_Rf = -15-(expect_w * (a + b));
//						SpeedLoopWant_Lb = 15-(expect_w * (a + b));
//						SpeedLoopWant_Rb = 15-(expect_w * (a + b));
//						if(count == 0) {gpio_set_level(B16, 1);}
//						else if(count == 1) {gpio_set_level(B17, 1);}
//						else if(count == 2) {gpio_set_level(B18, 1);}
//						system_delay_ms(100);
//						put_dong++;
//					}else if(7<=put_dong&&put_dong<14){
//						SpeedLoopWant_Lf = 15-(expect_w * (a + b));
//						SpeedLoopWant_Rf = 15-(expect_w * (a + b));
//						SpeedLoopWant_Lb = -15-(expect_w * (a + b));
//						SpeedLoopWant_Rb = -15-(expect_w * (a + b));
//						if(count == 0) {gpio_set_level(B16, 1);}
//						else if(count == 1) {gpio_set_level(B17, 1);}
//						else if(count == 2) {gpio_set_level(B18, 1);}
//						system_delay_ms(100);
//						put_dong++;
//					}else if(14<=put_dong&&put_dong<21){
//						SpeedLoopWant_Lf = -15-(expect_w * (a + b));
//						SpeedLoopWant_Rf = -15-(expect_w * (a + b));
//						SpeedLoopWant_Lb = 15-(expect_w * (a + b));
//						SpeedLoopWant_Rb = 15-(expect_w * (a + b));
//						if(count == 0) {gpio_set_level(B16, 1);}
//						else if(count == 1) {gpio_set_level(B17, 1);}
//						else if(count == 2) {gpio_set_level(B18, 1);}
//						system_delay_ms(100);
//						put_dong++;
//					}
//					else if(21<=put_dong&&put_dong<28){
//						SpeedLoopWant_Lf = 15-(expect_w * (a + b));
//						SpeedLoopWant_Rf = 15-(expect_w * (a + b));
//						SpeedLoopWant_Lb = -15-(expect_w * (a + b));
//						SpeedLoopWant_Rb = -15-(expect_w * (a + b));
//						if(count == 0) {gpio_set_level(B16, 1);}
//						else if(count == 1) {gpio_set_level(B17, 1);}
//						else if(count == 2) {gpio_set_level(B18, 1);}
//						system_delay_ms(100);
//						put_dong++;}
//				}
				
//				if(put_dong == 28){
					SpeedLoopWant_Lf = 0;
					SpeedLoopWant_Rf = 0;
					SpeedLoopWant_Lb = 0;
					SpeedLoopWant_Rb = 0;
					
//					if(count == 0) {gpio_set_level(B16, 0);}
//					else if(count == 1) {gpio_set_level(B17, 0);}
//					else if(count == 2) {gpio_set_level(B18, 0);}
					
//					system_delay_ms(1000);	
					countOver = 0;
					count++;
//					if(count == 1)
//						pwm_set_duty(PWM4_MODULE2_CHA_C30, 600);
//					else if(count == 2)
//						pwm_set_duty(PWM4_MODULE2_CHA_C30, 260);
					
					put_dong = 0;
					put_mode = 0;	
//					
//				}

			}
		}		

	}
}

int bdong = 0;

void BackCarport_entry(void *parameter)
{
	while(1){
		if((rt_sem_take(BackCarport_control_sem, RT_WAITING_FOREVER)) == RT_EOK){
			if(yellowLinesend){
//				gpio_set_level(B19, 1);
				SpeedLoopWant_Lf = -(XVCgo + 10 + expect_w * (a + b));
				SpeedLoopWant_Rf = -(XVCgo - 10 + expect_w * (a + b));
				SpeedLoopWant_Lb = (XVCgo - 10 - expect_w * (a + b));
				SpeedLoopWant_Rb = (XVCgo + 10 - expect_w * (a + b));
			}else{
//				gpio_set_level(B19, 0);
				if(bdong<40){
					SpeedLoopWant_Lf = -3-(expect_w * (a + b));
					SpeedLoopWant_Rf = 3-(expect_w * (a + b));
					SpeedLoopWant_Lb = -3-(expect_w * (a + b));
					SpeedLoopWant_Rb = 3-(expect_w * (a + b));
					bdong++;
				}else{
					SpeedLoopWant_Lf = -(expect_w * (a + b));
					SpeedLoopWant_Rf = -(expect_w * (a + b));
					SpeedLoopWant_Lb = -(expect_w * (a + b));
					SpeedLoopWant_Rb = -(expect_w * (a + b));
//					putPhoto_motion(count);
				}
					
				INS_para.Now_X = 0;
				INS_para.Now_Y = 0;
			}

		}
	}
}

void putPhoto_init()
{
	putPhoto_control_sem = rt_sem_create("putPhoto_control_sem", 0, RT_IPC_FLAG_FIFO);
	BackCarport_control_sem = rt_sem_create("BackCarport_control_sem", 0, RT_IPC_FLAG_FIFO);
	
	rt_thread_t tid;
	
		// 创建动态线程
	tid = rt_thread_create("putPhoto",							// 线程名称
		putPhoto_entry,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 1024 个字节的栈空间
		17,														// 线程优先级为8，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		5);														// 时间片为5

	if(tid != RT_NULL)											// 线程创建成功
	{
//		rt_kprintf("create thread1 OK\n");
		//运行该线程
		rt_thread_startup(tid);
	}
	
	tid = rt_thread_create("BackCarport",							// 线程名称
		BackCarport_entry,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 1024 个字节的栈空间
		19,														// 线程优先级为8，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		5);														// 时间片为5

	if(tid != RT_NULL)											// 线程创建成功
	{
//		rt_kprintf("create thread1 OK\n");
		//运行该线程
		rt_thread_startup(tid);
	}
}