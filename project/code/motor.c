#include "YJX.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pwm.h"
/**************************************************************************/
//————————————————电机控制————————————————
void speed_Init(void) // pwm初始化
{
    //前轮
//    pwm_init(PWM2_MODULE3_CHA_D2, 17 * 1000, 0); //左前
		gpio_init(D2, GPO, 0, GPO_PUSH_PULL);
    pwm_init(PWM2_MODULE3_CHB_D3, 17 * 1000, 0);
	
//    pwm_init(PWM1_MODULE3_CHA_D0, 17 * 1000, 0); //右前
//    pwm_init(PWM1_MODULE3_CHB_D1, 17 * 1000, 0);
		gpio_init(C10, GPO, 0, GPO_PUSH_PULL);
    pwm_init(PWM2_MODULE2_CHB_C11, 17 * 1000, 0);

    //后轮
//    pwm_init(PWM2_MODULE1_CHB_C9, 17 * 1000, 0); //左后
//    pwm_init(PWM2_MODULE1_CHA_C8, 17 * 1000, 0);
		gpio_init(C9, GPO, 0, GPO_PUSH_PULL);
    pwm_init(PWM2_MODULE1_CHA_C8, 17 * 1000, 0);

//    pwm_init(PWM2_MODULE0_CHB_C7, 17 * 1000, 0); //右后
//    pwm_init(PWM2_MODULE0_CHA_C6, 17 * 1000, 0);
		gpio_init(C7, GPO, 0, GPO_PUSH_PULL);
    pwm_init(PWM2_MODULE0_CHA_C6, 17 * 1000, 0);
}
	
void right_front(int32 motor) //左前轮赋值
{
    if (motor > 0) {
//        pwm_set_duty(PWM2_MODULE3_CHA_D2, motor);
//        pwm_set_duty(PWM2_MODULE3_CHB_D3, 0);
			gpio_set_level(D2, 0);
			pwm_set_duty(PWM2_MODULE3_CHB_D3, motor);
			
    } else {
//        pwm_set_duty(PWM2_MODULE3_CHA_D2, 0);
//        pwm_set_duty(PWM2_MODULE3_CHB_D3, -motor);
			gpio_set_level(D2, 1);
			pwm_set_duty(PWM2_MODULE3_CHB_D3, -motor);
    }
}

void left_front(int32 motor) //右前轮赋值
{
    if (motor > 0) {
//        pwm_set_duty(PWM1_MODULE3_CHA_D0, motor);
//        pwm_set_duty(PWM1_MODULE3_CHB_D1, 0);
			gpio_set_level(C10, 0);
			pwm_set_duty(PWM2_MODULE2_CHB_C11, motor);
    } else {
//        pwm_set_duty(PWM1_MODULE3_CHA_D0, 0);
//        pwm_set_duty(PWM1_MODULE3_CHB_D1, -motor);
			gpio_set_level(C10, 1);
			pwm_set_duty(PWM2_MODULE2_CHB_C11, -motor);
    }
}

void right_behind(int32 motor) //左后轮赋值
{
    if (motor > 0) {
//        pwm_set_duty(PWM2_MODULE1_CHA_C8, motor);
//        pwm_set_duty(PWM2_MODULE1_CHB_C9, 0);
			gpio_set_level(C9, 0);
			pwm_set_duty(PWM2_MODULE1_CHA_C8, motor);
    } else {
//        pwm_set_duty(PWM2_MODULE1_CHA_C8, 0);
//        pwm_set_duty(PWM2_MODULE1_CHB_C9, -motor);
			gpio_set_level(C9, 1);
			pwm_set_duty(PWM2_MODULE1_CHA_C8, -motor);
    }
}

void left_behind(int32 motor) //右后轮赋值
{
    if (motor > 0) {
//        pwm_set_duty(PWM2_MODULE0_CHA_C6, motor);
//        pwm_set_duty(PWM2_MODULE0_CHB_C7, 0);
			gpio_set_level(C7, 0);
			pwm_set_duty(PWM2_MODULE0_CHA_C6, motor);
    } else {
//        pwm_set_duty(PWM2_MODULE0_CHA_C6, 0);
//        pwm_set_duty(PWM2_MODULE0_CHB_C7, -motor);
			gpio_set_level(C7, 1);
			pwm_set_duty(PWM2_MODULE0_CHA_C6, -motor);
    }
}
