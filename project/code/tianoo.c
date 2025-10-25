#include "YJX.h"
#include "math.h"

extern rt_sem_t tianoo_control_sem;
extern INS2DPara INS_para;
extern PositionalPIDpara PlanXpara;
extern PositionalPIDpara PlanYpara;
extern int16 binary_control;
extern GetV getv;
extern int16 stackFlag;

float lower_left;
float lower_right;
float XXtiango;
float YXtiango;
float traverse_targetX;
float traverse_targetY;

float back_targetX;
float back_targetY;

float XVVgo;
float YVVgo;
int16 flag_startPiont = 0;
int16 traverse_count = 0;

extern float expect_w;
extern float SpeedLoopWant_Lf;
extern float SpeedLoopWant_Rf;
extern float SpeedLoopWant_Lb;
extern float SpeedLoopWant_Rb;

int16 tianRun = 1;

extern int16 holdFlag;
extern int carryi;
extern int16 carryover; //搬运时候目标值只算一次的标志位 0为还没算 1为已经算过
extern uint8 classification;
extern int areaFlag;
extern rt_sem_t binary_control_sem;
extern rt_sem_t me_control_sem;
extern uint16 mid_x,mid_y;
bool lintong = 0;

float stack_allx;
float stack_ally;

bool Yback = 0;
bool Xback = 0;
bool XXback = 0;
bool YYback = 0;

void tian_entry(void *parameter)
{
	while(1){
		if((rt_sem_take(tianoo_control_sem, RT_WAITING_FOREVER)) == RT_EOK){
			if(tianRun==1){//1为进行正常的遍历
//				if(!lintong){
//					mid_x = 0;
//					mid_y = 0;
//					lintong = 1;
//				}
				if(traverse_count==row_number){
					binary_control = 2;
					binary_control_sem->value = 0;
					SpeedLoopWant_Lf = -(expect_w * (a + b));
					SpeedLoopWant_Rf = -(expect_w * (a + b));
					SpeedLoopWant_Lb = -(expect_w * (a + b));
					SpeedLoopWant_Rb = -(expect_w * (a + b));
				
					INS_para.Now_X = 0;
					INS_para.Now_Y = 0;
					
					XXtiango = 0;
					YXtiango = 0;
					binary_control = 4;
//					servo_slow_ctrl(60, 110, 50);//放图片
//					system_delay_ms(1000);
				}
				if(!flag_startPiont)
					startPiont();
//				if(traverse_count<16)
//					binary_control = 3;	
				if(stackFlag==1){
//						INS_para.Now_X = 0;
//						INS_para.Now_Y = 0;
//						XXtiango = traverse_targetX - INS_para.all_X;
//						YXtiango = traverse_targetY - INS_para.all_Y;
						stackFlag = 0;
				}
				XVVgo = -PositionalPID(&PlanXpara, INS_para.Now_X, XXtiango, planPIDMax, 1);
				YVVgo = -PositionalPID(&PlanYpara, INS_para.Now_Y, YXtiango, planPIDMax, 1);
				
				SpeedLoopWant_Lf = -(XVVgo + YVVgo + expect_w * (a + b));
				SpeedLoopWant_Rf = -(XVVgo - YVVgo + expect_w * (a + b));
				SpeedLoopWant_Lb = (XVVgo - YVVgo - expect_w * (a + b));
				SpeedLoopWant_Rb = (XVVgo + YVVgo - expect_w * (a + b));
				if((traverse_count<row_number)&&(traverse_count>0))
					binary_control = 3;	
				if ((fabs(INS_para.Now_X) > (3.5/ 5.0) *fabs(XXtiango)) && (fabs(INS_para.Now_Y) > (3.5 / 5.0) *fabs(YXtiango))){
					if((fabs(getv.get_Vy) <= 0.5) && (fabs(getv.get_Vx) <= 0.5)){
						SpeedLoopWant_Lf = -(expect_w * (a + b));
						SpeedLoopWant_Rf = -(expect_w * (a + b));
						SpeedLoopWant_Lb = -(expect_w * (a + b));
						SpeedLoopWant_Rb = -(expect_w * (a + b));
						INS_para.Now_X = 0;
						INS_para.Now_Y = 0;
						system_delay_ms(500);
						if(stackFlag==0)
							traverse(flag_startPiont);
					}
				}
				
			}else if(tianRun == 2){//2为进行搬运
				
//					//搬起来
//					if (holdFlag == 0) {
//							SpeedLoopWant_Lf = -(expect_w * (a + b));
//							SpeedLoopWant_Rf = -(expect_w * (a + b));
//							SpeedLoopWant_Lb = -(expect_w * (a + b));
//							SpeedLoopWant_Rb = -(expect_w * (a + b));//先停车
//						
////							pwm_set_duty(PWM1_MODULE3_CHA_D0, 800); //舵机打角 800下去 200是上来
////							system_delay_us(200000);
////							gpio_set_level(D17, 1);
////							system_delay_us(500000);
////							for (carryi = 800; carryi > 600; carryi = carryi - 50) {
////									pwm_set_duty(PWM1_MODULE3_CHA_D0, carryi);
////									system_delay_us(20000);
////							}
//						
//							gpio_set_level(D12, 1);
//							servo_slow_ctrl(83, 5, 50);//下去
//							system_delay_ms(500);
//							servo_slow_ctrl(25, 5, 50);
//							system_delay_ms(500);
//							servo_slow_ctrl(70, 5, 50);
//						
//							holdFlag = 1;//搬起来辣
//						
//					}
//        //送过去
//        // 0-3黄瓜 榴莲 番薯 4-6蚕豆 橙子 白菜 7-9花生 苹果 辣椒 10-12萝卜 香蕉 水稻 13-15玉米 葡萄 茄子
//				//  ['0 bean', '1 potato', '2 orange', '3 durian', '4 apple',"5 grape","6 banana","7 peanut","8 rice","9 cron",
//				//	"10 cabbage","11 cucumber","12 chili","13 radish","14 eggplant"]
//        //0左边0-3 1前边4-6 2右边7-9 3后边10-12 4副车13-15
//        if (carryover == 0) {
//            if ((classification == 11) || (classification == 1) || (classification == 3)) {
//                areaFlag = 0;//左边
//								back_targetX = INS_para.all_X;
//                XXtiango = planover[areaFlag] - INS_para.all_X;
//                YXtiango = 0;
//                carryover = 1;
//            } else if ((classification == 0) || (classification == 10) || (classification == 2)) {
//                areaFlag = 1;//前边
//								back_targetY = INS_para.all_Y;
//                XXtiango = 0;
//                YXtiango = planover[areaFlag] - INS_para.all_Y;
//                carryover = 1;
//            } else if ((classification == 4) || (classification == 12) || (classification == 7)) {
//                areaFlag = 2;//右边
//								back_targetX = INS_para.all_X;
//                YXtiango = 0;
//                XXtiango = planover[areaFlag] - INS_para.all_X;
//                carryover = 1;
//            }else if((classification == 14) || (classification == 5) || (classification == 9)){
//								pwm_set_duty(PWM4_MODULE2_CHA_C30, 1100);
//								system_delay_ms(900);
//			
//								servo_slow_ctrl(70, 5, 100);//中间变换
//								system_delay_ms(100);
//								
//								servo_slow_ctrl(70, 110, 100);//中间变换
//								system_delay_ms(100);
//								
//								servo_slow_ctrl(115, 135, 100);//放图片
//								system_delay_ms(500);
//								gpio_set_level(D12, 0);
//							
//								holdFlag = 0;
//								tianRun = 3;//该下一个点辣
//								carryover = 0;
//								
//								INS_para.Now_X = 0;
//								INS_para.Now_Y = 0;
//								tianRun = 3;//该下一个点辣
//						}else {
//								areaFlag = 3;//后边
//								back_targetY = INS_para.all_Y;
//								XXtiango = 0;
//								YXtiango = planover[areaFlag] - INS_para.all_Y;
//								carryover = 1;
//						}
//        }
//					
//					XVVgo = -PositionalPID(&PlanXpara, INS_para.Now_X, XXtiango, planPIDMax, 1);
//					YVVgo = -PositionalPID(&PlanYpara, INS_para.Now_Y, YXtiango, planPIDMax, 1);
//					if(carryover){
//						SpeedLoopWant_Lf = -(XVVgo + YVVgo + expect_w * (a + b));
//						SpeedLoopWant_Rf = -(XVVgo - YVVgo + expect_w * (a + b));
//						SpeedLoopWant_Lb = (XVVgo - YVVgo - expect_w * (a + b));
//						SpeedLoopWant_Rb = (XVVgo + YVVgo - expect_w * (a + b));
//						
//					if ((fabs(INS_para.Now_X) > (4.7 / 5.0) *fabs(XXtiango)) && (fabs(INS_para.Now_Y) > (4.7 / 5.0) *fabs(YXtiango))) {
//						if((fabs(getv.get_Vy) <= 0.5) && (fabs(getv.get_Vx) <= 0.5)){
//							SpeedLoopWant_Lf = -(expect_w * (a + b));
//							SpeedLoopWant_Rf = -(expect_w * (a + b));
//							SpeedLoopWant_Lb = -(expect_w * (a + b));
//							SpeedLoopWant_Rb = -(expect_w * (a + b));//先停车
//							
//						//不进行搬运后位置更新	
//							INS_para.Now_X = 0;
//							INS_para.Now_Y = 0;

////							for (carryi = 600; carryi < 800; carryi = carryi + 100) {
////									pwm_set_duty(PWM1_MODULE3_CHA_D0, carryi);
////									if (carryi == 700) {
////											gpio_set_level(D17, 0);
////									}
////									system_delay_us(5000);
////							}
////							pwm_set_duty(PWM1_MODULE3_CHA_D0, 200); //舵机打角 800下去 200是上来
//							
//							gpio_set_level(D12, 0);
//							servo_slow_ctrl(70, 5, 100);//中间变换
//							system_delay_ms(100);
//							
//							servo_slow_ctrl(70, 110, 100);//中间变换
//							system_delay_ms(100);
//							
//							servo_slow_ctrl(115, 135, 100);//放图片
//							system_delay_ms(500);

//							holdFlag = 0;
//							carryover = 0;
////							XXtiango = stack_allx - INS_para.all_X;
////							XXtiango = stack_ally - INS_para.all_Y
//							tianRun = 5;
//						}
//					}
//					
//				}

				SpeedLoopWant_Lf = -(expect_w * (a + b));
				SpeedLoopWant_Rf = -(expect_w * (a + b));
				SpeedLoopWant_Lb = -(expect_w * (a + b));
				SpeedLoopWant_Rb = -(expect_w * (a + b));//先停车
				dongzuo(classification);	
				tianRun = 3;

			}else if(tianRun == 3){//3为回到被打断的点
				if(!Yback){
					binary_control_sem->value = 0;
					XXtiango = 0;
					YXtiango = traverse_targetY - INS_para.all_Y;
					Yback = 1;
				}

				if(YXtiango<1.5){
						INS_para.Now_X = 0;
						INS_para.Now_Y = 0;
						tianRun = 4;
						Yback = 0;
				}
					

				
				XVVgo = -PositionalPID(&PlanXpara, INS_para.Now_X, XXtiango, planPIDMax, 1);
				YVVgo = -PositionalPID(&PlanYpara, INS_para.Now_Y, YXtiango, planPIDMax, 1);
				SpeedLoopWant_Lf = -(XVVgo + YVVgo + expect_w * (a + b));
				SpeedLoopWant_Rf = -(XVVgo - YVVgo + expect_w * (a + b));
				SpeedLoopWant_Lb = (XVVgo - YVVgo - expect_w * (a + b));
				SpeedLoopWant_Rb = (XVVgo + YVVgo - expect_w * (a + b));
				
				if ((fabs(INS_para.Now_X) >= (2.0 / 5.0) *fabs(XXtiango)) && (fabs(INS_para.Now_Y) >= (2.0 / 5.0) *fabs(YXtiango))){
					if((fabs(getv.get_Vy) <= 0.5) && (fabs(getv.get_Vx) <= 0.5)){
						SpeedLoopWant_Lf = -(expect_w * (a + b));
						SpeedLoopWant_Rf = -(expect_w * (a + b));
						SpeedLoopWant_Lb = -(expect_w * (a + b));
						SpeedLoopWant_Rb = -(expect_w * (a + b));//先停车
						
//						stack_allx = 0;
//						stack_ally = 0;
						INS_para.Now_X = 0;
						INS_para.Now_Y = 0;
						tianRun = 4;
						Yback = 0;
					}
				}

			}
			else if(tianRun == 4){
				if(!Xback){
					binary_control = 3;
					binary_control_sem->value = 0;
					YXtiango = 0;
					XXtiango = traverse_targetX - INS_para.all_X;
					Xback = 1;
				}
				if(stackFlag==1){
						stackFlag = 0;
				}
				
				XVVgo = -PositionalPID(&PlanXpara, INS_para.Now_X, XXtiango, planPIDMax, 1);
				YVVgo = -PositionalPID(&PlanYpara, INS_para.Now_Y, YXtiango, planPIDMax, 1);
				SpeedLoopWant_Lf = -(XVVgo + YVVgo + expect_w * (a + b));
				SpeedLoopWant_Rf = -(XVVgo - YVVgo + expect_w * (a + b));
				SpeedLoopWant_Lb = (XVVgo - YVVgo - expect_w * (a + b));
				SpeedLoopWant_Rb = (XVVgo + YVVgo - expect_w * (a + b));
				
				if ((fabs(INS_para.Now_X) > (3.5 / 5.0) *fabs(XXtiango)) && (fabs(INS_para.Now_Y) > (3.5 / 5.0) *fabs(YXtiango))){
					if((fabs(getv.get_Vy) <= 0.5) && (fabs(getv.get_Vx) <= 0.5)){
						SpeedLoopWant_Lf = -(expect_w * (a + b));
						SpeedLoopWant_Rf = -(expect_w * (a + b));
						SpeedLoopWant_Lb = -(expect_w * (a + b));
						SpeedLoopWant_Rb = -(expect_w * (a + b));//先停车
						
//						stack_allx = 0;
//						stack_ally = 0;
						INS_para.Now_X = 0;
						INS_para.Now_Y = 0;
						if(stackFlag == 0)
							traverse(flag_startPiont);
						lintong = 0;
						tianRun = 1;
						Xback = 0;
					}
				}

			}
			else if(tianRun == 5){
				if((areaFlag==0)||(areaFlag==2)){
					if(!XXback){
						binary_control_sem->value = 0;
						YXtiango = 0;
						XXtiango = back_targetX - INS_para.all_X;
						XXback = 1;
					}
				}else if((areaFlag==1)||(areaFlag==3)){
					if(!YYback){
						binary_control_sem->value = 0;
						XXtiango = 0;
						YXtiango = back_targetY - INS_para.all_Y;
						YYback = 1;
					}
				}

				
				XVVgo = -PositionalPID(&PlanXpara, INS_para.Now_X, XXtiango, planPIDMax, 1);
				YVVgo = -PositionalPID(&PlanYpara, INS_para.Now_Y, YXtiango, planPIDMax, 1);
				SpeedLoopWant_Lf = -(XVVgo + YVVgo + expect_w * (a + b));
				SpeedLoopWant_Rf = -(XVVgo - YVVgo + expect_w * (a + b));
				SpeedLoopWant_Lb = (XVVgo - YVVgo - expect_w * (a + b));
				SpeedLoopWant_Rb = (XVVgo + YVVgo - expect_w * (a + b));
				
				if ((fabs(INS_para.Now_X) >= (2.0 / 5.0) *fabs(XXtiango)) && (fabs(INS_para.Now_Y) >= (2.0 / 5.0) *fabs(YXtiango))){
					if((fabs(getv.get_Vy) <= 0.5) && (fabs(getv.get_Vx) <= 0.5)){
						SpeedLoopWant_Lf = -(expect_w * (a + b));
						SpeedLoopWant_Rf = -(expect_w * (a + b));
						SpeedLoopWant_Lb = -(expect_w * (a + b));
						SpeedLoopWant_Rb = -(expect_w * (a + b));//先停车
						
//						stack_allx = 0;
//						stack_ally = 0;
						INS_para.Now_X = 0;
						INS_para.Now_Y = 0;
						tianRun = 3;
						if((areaFlag==0)||(areaFlag==2)){
							INS_para.all_X=back_targetX;
								XXback = 0;
								back_targetX = 0;
							//更新坐标
							
						}else if((areaFlag==1)||(areaFlag==3)){
							INS_para.all_Y=back_targetY;
							//
							YYback = 0;
							back_targetY = 0;
						}
					}
				}
			}


		}
	}
}

void traverse(int16 direction)//遍历输出
{	
	if(direction == 1){
		if(!((traverse_count/3)%2)){
			traverse_targetX = coordinateTransformationX(traverse_right, 0);
			traverse_targetY = coordinateTransformationY((3+traverse_count), 0);
		}else if((traverse_count/3)%2){
			traverse_targetX = coordinateTransformationX(traverse_left, 0);
			traverse_targetY = coordinateTransformationY((3+traverse_count), 0);
		}
	}else if(direction == 2){
		if(!((traverse_count/3)%2)){
			traverse_targetX = coordinateTransformationX(traverse_left, 0);
			traverse_targetY = coordinateTransformationY((3+traverse_count), 0);
		}else if((traverse_count/3)%2){
			traverse_targetX = coordinateTransformationX(traverse_right, 0);
			traverse_targetY = coordinateTransformationY((3+traverse_count), 0);
		}
	}

	XXtiango = traverse_targetX - INS_para.all_X;
	YXtiango = traverse_targetY - INS_para.all_Y;
	
	traverse_count = traverse_count + 3; 
}

float D_midxL;
float D_midyL;
float D_midxR;
float D_midyR;
void startPiont(void)//算出从左还是从右开始遍历
{
	binary_control_sem->value = 0;
	INS_para.Now_X = 0;
	INS_para.Now_Y = 0;
	
	D_midxL = coordinateTransformationX(traverse_left, 0) - INS_para.all_X;
	D_midyL = coordinateTransformationY(3, 0) - INS_para.all_Y;
	lower_left = sqrt(D_midxL*D_midxL+D_midyL*D_midyL);
	
	D_midxR = coordinateTransformationX(traverse_right, 0) - INS_para.all_X;
	D_midyR = coordinateTransformationY(3, 0) - INS_para.all_Y;
	lower_right = sqrt(D_midxR*D_midxR+D_midyR*D_midyR);
	
	if(lower_left>lower_right){
		XXtiango = D_midxR;
		YXtiango = D_midyR;
		flag_startPiont = 2;//从右边
	}else{
		XXtiango = D_midxL;
		YXtiango = D_midyL;
		flag_startPiont = 1;//从左边
	}
}

void tianoo_init()
{
	rt_thread_t tid;
	
		// 创建动态线程
	tid = rt_thread_create("tianoo",							// 线程名称
		tian_entry,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 1024 个字节的栈空间
		16,														// 线程优先级为8，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		5);														// 时间片为5

	if(tid != RT_NULL)											// 线程创建成功
	{
//		rt_kprintf("create thread1 OK\n");
		//运行该线程
		rt_thread_startup(tid);
	}
}