#ifndef __PID_ALL_H__
#define __PID_ALL_H__

#include "zf_common_headfile.h"

//增量式PID
typedef struct {
    float KP;
    float KI;
    float KD;
	
	  //比例 微分 积分项
    int16 out_P; // PID的比例项
    int16 out_I; // PID的积分项
    int16 out_D; // PID的微分项
	
	  int16 Err; //当前偏差量
    int16 ErrLast; //前一次偏差量
    int16 ErrLastLast; //上上次偏差量

    int16 pwmDuty;

    int32 middle;
} IncrementalPIDpara;

//位置式pid
typedef struct {
    float Kp;
    float Ki;
		float Kii;
    float Kd;
//    float Out;

    float Err; //当前偏差量
    float LastErr; //上次的偏差
    float Integ; //积分量
    float Deriv; //微分量

    float out_P; // PID的比例项
    float out_I; // PID的积分项
    float out_D; // PID的微分项

    float out;
    int list_index;
    int num_integral;
    float list_value[15];
} PositionalPIDpara;

float IncrementalPID(IncrementalPIDpara* para, int16 Act, float Want);
float PositionalPID(PositionalPIDpara* para, float Act, float Want, int PidMax, int16 per_flag);

#endif
