#include "YJX.h"
#include "math.h"

extern float expect_w;
extern float SpeedLoopWant_Lf;
extern float SpeedLoopWant_Rf;
extern float SpeedLoopWant_Lb;
extern float SpeedLoopWant_Rb;

extern float XVgo;
extern float YVgo;
extern float XXstopgo;
extern float YXstopgo;
extern INS2DPara INS_para;
extern float visited[AMOUNT][1][3];

extern GetV getv;

extern int16 photokey;
int16 carryFlag = 2; // 0为未搬运 1为正在搬运 2为上一次搬运结束

int16 start = 0;//按键开车标志
int16 coordinateRead_over = 0;//坐标读取完成标志
int16 binary_control = 0;//0是me可控制，1是凌瞳控制 2是无框图片的遍历

extern rt_sem_t me_control_sem;

void masterControl_entry (void *parameter)
{
	planover_Init();
	while(1){
		if((rt_sem_take(me_control_sem, RT_WAITING_FOREVER)) == RT_EOK){
//			coordinateRead_over = 1;
			if(coordinateRead_over)
				control();
		}
	}
}
//}

void control_text()
{
		SpeedLoopWant_Lf = -(expect_w * (a + b));
		SpeedLoopWant_Rf = -(expect_w * (a + b));
		SpeedLoopWant_Lb = -(expect_w * (a + b));
		SpeedLoopWant_Rb = -(expect_w * (a + b));
		binary_control = 1;//该凌瞳干活辣
}

int16 holdFlag;
int carryi;
int16 carryover = 0; //搬运时候目标值只算一次的标志位 0为还没算 1为已经算过
extern uint8 classification;
int areaFlag;
float planover[4] = { 0 };
int bb;

extern rt_mailbox_t buzzer_mailbox;
extern rt_sem_t binary_control_sem;

void control(void)
{
		if (carryFlag == 2) {//进行路径规划
			plan_Init();
			if (photokey >= 0) {
					carryFlag = 0;
			} else {
					SpeedLoopWant_Lf = -(expect_w * (a + b));
					SpeedLoopWant_Rf = -(expect_w * (a + b));
					SpeedLoopWant_Lb = -(expect_w * (a + b));
					SpeedLoopWant_Rb = -(expect_w * (a + b));
				
					INS_para.Now_X = 0;
					INS_para.Now_Y = 0;
					binary_control_sem->value = 0;
//					system_delay_ms(500);
//					binary_control = 2;	//开始无框图片的遍历
					binary_control = 4;	//单搬完直接放图回库

			} 
		}else if (carryFlag == 0) {//跑路径规划
				SpeedLoopWant_Lf = -(XVgo + YVgo + expect_w * (a + b));
				SpeedLoopWant_Rf = -(XVgo - YVgo + expect_w * (a + b));
				SpeedLoopWant_Lb = (XVgo - YVgo - expect_w * (a + b));
				SpeedLoopWant_Rb = (XVgo + YVgo - expect_w * (a + b));
			  if ((fabs(INS_para.Now_X) > (3.7 / 5.0) *fabs(XXstopgo)) && (fabs(INS_para.Now_Y) > (3.7 / 5.0) *fabs(YXstopgo))) {
            if ((fabs(getv.get_Vx) <= 0.5) && (fabs(getv.get_Vy) <= 0.5)) {
								SpeedLoopWant_Lf = -(expect_w * (a + b));
								SpeedLoopWant_Rf = -(expect_w * (a + b));
								SpeedLoopWant_Lb = -(expect_w * (a + b));
								SpeedLoopWant_Rb = -(expect_w * (a + b));
								binary_control = 1;//该凌瞳干活辣
								rt_mb_send(buzzer_mailbox, 50);
//							if(!bb){//测试用
//								printf("%d", 0);//该openart干活辣
//								bb=1;
//							}
            }
        }
		}else if(carryFlag==1){
			
				//搬起来
        if (holdFlag == 0) {
						SpeedLoopWant_Lf = -(expect_w * (a + b));
						SpeedLoopWant_Rf = -(expect_w * (a + b));
						SpeedLoopWant_Lb = -(expect_w * (a + b));
						SpeedLoopWant_Rb = -(expect_w * (a + b));//先停车
					
//            pwm_set_duty(PWM1_MODULE3_CHA_D0, 800); //舵机打角 800下去 200是上来
//            system_delay_us(200000);
//            gpio_set_level(D17, 1);
//            system_delay_us(500000);
//            for (carryi = 800; carryi > 600; carryi = carryi - 50) {
//                pwm_set_duty(PWM1_MODULE3_CHA_D0, carryi);
//                system_delay_us(20000);
//            }
					gpio_set_level(D12, 1);
					servo_slow_ctrl(83, 5, 50);//下去
					system_delay_us(500000);
					servo_slow_ctrl(25, 5, 50);
					system_delay_us(500000);
					servo_slow_ctrl(70, 5, 50);
					
            holdFlag = 1;//搬起来辣
					
        }
        //送过去
        // 0-3黄瓜 榴莲 番薯 4-6蚕豆 橙子 白菜 7-9花生 苹果 辣椒 10-12萝卜 香蕉 水稻 13-15玉米 葡萄 茄子
				//  ['0 bean', '1 potato', '2 orange', '3 durian', '4 apple',"5 grape","6 banana","7 peanut","8 rice","9 cron",
				//	"10 cabbage","11 cucumber","12 chili","13 radish","14 eggplant"]
        //0左边0-3 1前边4-6 2右边7-9 3后边10-12 4副车13-15
        if (carryover == 0) {
            if ((classification == 11) || (classification == 1) || (classification == 3)) {
                areaFlag = 0;//左边
                XXstopgo = planover[areaFlag] - INS_para.all_X;
                YXstopgo = 0;
                carryover = 1;
            } else if ((classification == 0) || (classification == 10) || (classification == 2)) {
                areaFlag = 1;//前边
                XXstopgo = 0;
                YXstopgo = planover[areaFlag] - INS_para.all_Y;
                carryover = 1;
            } else if ((classification == 4) || (classification == 12) || (classification == 7)) {
                areaFlag = 2;//右边
                YXstopgo = 0;
                XXstopgo = planover[areaFlag] - INS_para.all_X;
                carryover = 1;
            }else if((classification == 14) || (classification == 5) || (classification == 9)){
								pwm_set_duty(PWM4_MODULE2_CHA_C30, 1070);
								system_delay_us(900000);
			
								servo_slow_ctrl(70, 5, 50);//中间变换
								system_delay_us(100000);
								
								servo_slow_ctrl(70, 110, 50);//中间变换
								system_delay_us(100000);
								
								servo_slow_ctrl(110, 140, 100);//放图片
								system_delay_us(500000);
								gpio_set_level(D12, 0);
							
								holdFlag = 0;
								carryFlag = 2;//该下一个点辣
								carryover = 0;
								
								INS_para.Now_X = 0;
								INS_para.Now_Y = 0;
								carryFlag = 2;//该下一个点辣
						}else {
								areaFlag = 3;//后边
								XXstopgo = 0;
								YXstopgo = planover[areaFlag] - INS_para.all_Y;
								carryover = 1;
						}
        }
				
				if(carryover){
					SpeedLoopWant_Lf = -(XVgo + YVgo + expect_w * (a + b));
					SpeedLoopWant_Rf = -(XVgo - YVgo + expect_w * (a + b));
					SpeedLoopWant_Lb = (XVgo - YVgo - expect_w * (a + b));
					SpeedLoopWant_Rb = (XVgo + YVgo - expect_w * (a + b));
					
				if ((fabs(INS_para.Now_X) > (4.7 / 5.0) *fabs(XXstopgo)) && (fabs(INS_para.Now_Y) > (4.7 / 5.0) *fabs(YXstopgo))) {
					if((fabs(getv.get_Vy) <= 0.5) && (fabs(getv.get_Vx) <= 0.5)){
						SpeedLoopWant_Lf = -(expect_w * (a + b));
						SpeedLoopWant_Rf = -(expect_w * (a + b));
						SpeedLoopWant_Lb = -(expect_w * (a + b));
						SpeedLoopWant_Rb = -(expect_w * (a + b));//先停车
						
					//不进行搬运后位置更新	
//						if (areaFlag == 0) {
//                INS_para.all_X = planover[areaFlag];
//                INS_para.all_Y = visited[photokey][0][2];
//            } else if (areaFlag == 1) {
//                INS_para.all_X = visited[photokey][0][1];
//                INS_para.all_Y = planover[areaFlag];
//            } else if (areaFlag == 2) {
//                INS_para.all_X = planover[areaFlag];
//                INS_para.all_Y = visited[photokey][0][2];
//            }else if	(areaFlag == 3){
//								INS_para.all_X = visited[photokey][0][1];
//                INS_para.all_Y = planover[areaFlag];
//						}
            INS_para.Now_X = 0;
            INS_para.Now_Y = 0;

//            for (carryi = 600; carryi < 800; carryi = carryi + 100) {
//                pwm_set_duty(PWM1_MODULE3_CHA_D0, carryi);
//                if (carryi == 700) {
//                    gpio_set_level(D17, 0);
//                }
//                system_delay_us(5000);
//            }
//            pwm_set_duty(PWM1_MODULE3_CHA_D0, 200); //舵机打角 800下去 200是上来
						gpio_set_level(D12, 0);
						servo_slow_ctrl(110, 140, 100);//放图片
						system_delay_us(500000);
						

            holdFlag = 0;
            carryFlag = 2;//该下一个点辣
            carryover = 0;
						
						INS_para.Now_X = 0;
						INS_para.Now_Y = 0;
						carryFlag = 2;//该下一个点辣
					}
				}
			}

//			//嘎嘎分类
//			SpeedLoopWant_Lf = -(expect_w * (a + b));
//			SpeedLoopWant_Rf = -(expect_w * (a + b));
//			SpeedLoopWant_Lb = -(expect_w * (a + b));
//			SpeedLoopWant_Rb = -(expect_w * (a + b));//先停车
//			dongzuo(classification);	


//			INS_para.Now_X = 0;
//			INS_para.Now_Y = 0;
//      carryFlag = 2;//该下一个点辣
		}
}

void planover_Init(void)
{
    planover[0] = coordinateTransformationX(left_boundary, 0); //左边
    planover[1] = coordinateTransformationY(front_boundary, 0); //前边
    planover[2] = coordinateTransformationX(right_boundary, 0); //右边
		planover[3] = coordinateTransformationY(back_boundary, 0); //后边
}

void masterControl_init()
{
	rt_thread_t tid;
	
		// 创建动态线程
	tid = rt_thread_create("imu",							// 线程名称
		masterControl_entry,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 1024 个字节的栈空间
		15,														// 线程优先级为8，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		5);														// 时间片为5

	if(tid != RT_NULL)											// 线程创建成功
	{
//		rt_kprintf("create thread1 OK\n");
		//运行该线程
		rt_thread_startup(tid);
	}
}
