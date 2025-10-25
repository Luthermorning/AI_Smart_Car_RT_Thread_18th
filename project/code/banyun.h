#ifndef _banyun_h_
#define _banyun_h_

#include "zf_common_headfile.h"

#define SERVO_MOTOR_PWM1                (PWM1_MODULE3_CHA_D0)                      // 定义主板上舵机对应引脚
#define SERVO_MOTOR_PWM2                (PWM1_MODULE3_CHB_D1)                       // 定义主板上舵机对应引脚 
#define SERVO_MOTOR_FREQ                (50 )                                       // 定义主板上舵机频率  请务必注意范围 50-300
// ------------------ 舵机占空比计算方式 ------------------
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))    //舵机角度设置为0 - 180，90度为中值

void servo_slow_ctrl(uint16 _servo1_angle, uint16 _servo2_angle, float _step_count);
void dongzuo_init();
void dongzuo(int type);
void putPhoto_motion(int place);

extern int tdong;

#endif
