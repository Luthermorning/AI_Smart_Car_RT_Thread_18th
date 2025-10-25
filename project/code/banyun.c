#include "YJX.h"
#include "math.h"

static uint16 servo1_duty = 0, servo2_duty = 0;
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     舵机连续控制函数
// 参数说明     _servo1_angle               舵机1的目标角度
// 参数说明     _servo2_angle               舵机2的目标角度
// 返回参数     _step_count                 舵机连续控制间隔次数
// 使用示例     servo_slow_ctrl(90, 90, 100);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void servo_slow_ctrl(uint16 _servo1_angle, uint16 _servo2_angle, float _step_count)
{
    float servo1_start = (float)servo1_duty, servo2_start = (float)servo2_duty;
    float servo1_step = (float)(_servo1_angle - servo1_duty)/_step_count, servo2_step = (float)(_servo2_angle - servo2_duty)/_step_count;
    while(1)
    {
        system_delay_ms(5);
        if(fabsf(servo1_start - (float)_servo1_angle) >= servo1_step)servo1_start += servo1_step;
        else servo1_start = _servo1_angle;
        pwm_set_duty(SERVO_MOTOR_PWM1, (uint32)SERVO_MOTOR_DUTY((uint16)servo1_start));
        
        if(fabsf(servo2_start - (float)_servo2_angle) >= servo2_step)servo2_start += servo2_step;
        else servo2_start = _servo2_angle;
        pwm_set_duty(SERVO_MOTOR_PWM2, (uint32)SERVO_MOTOR_DUTY((uint16)servo2_start));
        
        if(fabsf(servo1_start - (float)_servo1_angle) < 1 && fabsf(servo2_start - (float)_servo2_angle) < 1)
        {
            servo1_duty = (uint16)_servo1_angle;
            servo2_duty = (uint16)_servo2_angle;
            return;
        }
    }
}

void dongzuo_init()
{
	pwm_init(PWM4_MODULE2_CHA_C30, 50, 600);
	pwm_init(PWM1_MODULE3_CHA_D0, 50, 750);
	pwm_init(PWM1_MODULE3_CHB_D1, 50, 1100);
	gpio_init(D12, GPO, 0, GPO_PUSH_PULL);
//	servo_slow_ctrl(110, 120, 50);
}
int tdong = 0;
extern float expect_w;
extern float SpeedLoopWant_Lf;
extern float SpeedLoopWant_Rf;
extern float SpeedLoopWant_Lb;
extern float SpeedLoopWant_Rb;
void putPhoto_motion(int place)
{
	if(place == 0){//右边的B16 600
		if(!tdong){
			pwm_set_duty(PWM4_MODULE2_CHA_C30, 260);
			gpio_set_level(B16, 1);
			system_delay_ms(2000);
		}
		if (tdong<40){
			SpeedLoopWant_Lf = 3-(expect_w * (a + b));
			SpeedLoopWant_Rf = 3-(expect_w * (a + b));
			SpeedLoopWant_Lb = -3-(expect_w * (a + b));
			SpeedLoopWant_Rb = -3-(expect_w * (a + b));
			tdong++;	
		}
		else if((tdong>=40)&&(tdong<80)){
			SpeedLoopWant_Lf = -3-(expect_w * (a + b));
			SpeedLoopWant_Rf = -3-(expect_w * (a + b));
			SpeedLoopWant_Lb = 3-(expect_w * (a + b));
			SpeedLoopWant_Rb = 3-(expect_w * (a + b));
			tdong++;
		}
//		SpeedLoopWant_Lf = 0;
//		SpeedLoopWant_Rf = 0;
//		SpeedLoopWant_Lb = 0;
//		SpeedLoopWant_Rb = 0;
		if(tdong==80){
			gpio_set_level(B16, 0);
			system_delay_ms(2000);		
		}
	}else if(place == 1){//前边B17 920
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 750);
		system_delay_ms(900);
		gpio_set_level(B17, 1);
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 600);
		system_delay_ms(1000);
		gpio_set_level(B17, 0);
	}else if(place == 2){//左边B18 1260
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 1100);
		system_delay_ms(900);
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 920);
		gpio_set_level(B18, 1);
		system_delay_ms(1000);
		gpio_set_level(B18, 0);
	}else if(place == 3){//后边B19 260
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 1100);
		system_delay_ms(900);
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 1260);
		gpio_set_level(B19, 1);
		system_delay_ms(1000);
		gpio_set_level(B19, 0);
	}
}

void dongzuo(int type)
{
	gpio_set_level(D12, 1);
	servo_slow_ctrl(83, 5, 50);//下去
	system_delay_ms(500);
	servo_slow_ctrl(25, 5, 50);
	system_delay_ms(500);
	
	//  ['0 bean', '1 potato', '2 orange', '3 durian', '4 apple',"5 grape","6 banana","7 peanut","8 rice","9 cron",
	//	"10 cabbage","11 cucumber","12 chili","13 radish","14 eggplant"]
	if ((type == 11) || (type == 1) || (type == 3)) {//左
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 1260);
	} else if ((type == 0) || (type == 10) || (type == 2)) {//前
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 920);
	} else if ((type == 4) || (type == 12) || (type == 7)) {//右
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 600);
	}else if((type == 14) || (type == 5) || (type == 9)){//放中间
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 1070);
	}	else {//后
		pwm_set_duty(PWM4_MODULE2_CHA_C30, 260);
	}
	
//	if(type == 1)
//		pwm_set_duty(PWM4_MODULE2_CHA_C30, 600);
//	else if(type == 2)
//		pwm_set_duty(PWM4_MODULE2_CHA_C30, 940);
//	else if(type == 3)
//		pwm_set_duty(PWM4_MODULE2_CHA_C30, 1260);
//	else if(type == 4)
//		pwm_set_duty(PWM4_MODULE2_CHA_C30, 260);
	if((type == 14) || (type == 9) || (type == 5)){
		system_delay_ms(900);
		
		servo_slow_ctrl(70, 5, 50);//中间变换
		system_delay_ms(100);
		
		servo_slow_ctrl(70, 110, 50);//中间变换
		system_delay_ms(100);
		
		servo_slow_ctrl(110, 150, 100);//放图片
		system_delay_ms(500);
		gpio_set_level(D12, 0);
	}else if((type == 11) || (type == 1) || (type == 3)){
		system_delay_ms(900);
		
		servo_slow_ctrl(70, 5, 50);//中间变换
		system_delay_ms(100);
		
		servo_slow_ctrl(70, 130, 50);//中间变换
		system_delay_ms(100);
		
		servo_slow_ctrl(87, 140, 50);//中间变换
		system_delay_ms(100);
		
		servo_slow_ctrl(87, 180, 50);//中间变换
		system_delay_ms(100);
		
		servo_slow_ctrl(85, 180, 50);//放图片
		system_delay_ms(500);
		gpio_set_level(D12, 0);
	}else{
		system_delay_ms(900);
		
		servo_slow_ctrl(70, 5, 50);//中间变换
		system_delay_ms(100);
		
		servo_slow_ctrl(70, 130, 50);//中间变换
		system_delay_ms(100);
		
		servo_slow_ctrl(83, 130, 50);//中间变换
		system_delay_ms(100);
		
		servo_slow_ctrl(83, 180, 50);//放图片
		system_delay_ms(500);
		gpio_set_level(D12, 0);
	}
}
