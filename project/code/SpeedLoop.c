#include "YJX.h"
#include "zf_common_headfile.h"

//四个轮子分别的速度环参数
IncrementalPIDpara SpeedLoopParaLf;
IncrementalPIDpara SpeedLoopParaRf;
IncrementalPIDpara SpeedLoopParaLb;
IncrementalPIDpara SpeedLoopParaRb;	

GetV getv;

//四个轮子分别的实际速度
int16 SpeedLoopAct_Lf;
int16 SpeedLoopAct_Rf;
int16 SpeedLoopAct_Lb;
int16 SpeedLoopAct_Rb;

//四个轮子分别的期望速度
float SpeedLoopWant_Lf = 0;
float SpeedLoopWant_Rf = 0;
float SpeedLoopWant_Lb = 0;
float SpeedLoopWant_Rb = 0;

extern ECPULSE ECPULSE_v;

void speedLoop_init(IncrementalPIDpara* SpeedLoopPara){
	  SpeedLoopPara->Err = 0;
    SpeedLoopPara->ErrLast = 0;
    SpeedLoopPara->ErrLastLast = 0;
    SpeedLoopPara->out_P = 0;
    SpeedLoopPara->out_I = 0;
    SpeedLoopPara->out_D = 0;

    SpeedLoopPara->KD = 0;
    SpeedLoopPara->KP = 300;
    SpeedLoopPara->KI = 19.5;
    SpeedLoopPara->pwmDuty = 0;
    SpeedLoopPara->middle = 0;
}


void get4wheels_init(GetV* getv)
{
		getv->get_Vx = 0;
		getv->get_Vy = 0;
}

// PID赋值和速度反解算
int32 Lf;
int32 Rf;
int32 Lb;
int32 Rb;

//计算速度环pid并赋值，并且做4轮解算
void SpeedPIDdutySet_AND_Get4wheels(void)
{
    //编码器值赋给PID结构体
    if (ECPULSE_v.ECPULSELf > 30000)
        SpeedLoopAct_Lf = ECPULSE_v.ECPULSELf - 65535;
    else
        SpeedLoopAct_Lf = ECPULSE_v.ECPULSELf;

    if (ECPULSE_v.ECPULSERf < 30000)
        SpeedLoopAct_Rf = ECPULSE_v.ECPULSERf;
    else
        SpeedLoopAct_Rf = ECPULSE_v.ECPULSERf - 65535;

    if (ECPULSE_v.ECPULSELb > 30000)
        SpeedLoopAct_Lb = ECPULSE_v.ECPULSELb - 65535;
    else
        SpeedLoopAct_Lb = ECPULSE_v.ECPULSELb;

    if (ECPULSE_v.ECPULSERb > 30000)
        SpeedLoopAct_Rb = ECPULSE_v.ECPULSERb - 65535;
    else
        SpeedLoopAct_Rb = ECPULSE_v.ECPULSERb;

    int32 speeddutyLf = (int32)IncrementalPID(&SpeedLoopParaLf, -SpeedLoopAct_Lf, SpeedLoopWant_Lf);
    int32 speeddutyRf = (int32)IncrementalPID(&SpeedLoopParaRf, -SpeedLoopAct_Rf, SpeedLoopWant_Rf);
    int32 speeddutyLb = (int32)IncrementalPID(&SpeedLoopParaLb, -SpeedLoopAct_Lb, SpeedLoopWant_Lb);
    int32 speeddutyRb = (int32)IncrementalPID(&SpeedLoopParaRb, -SpeedLoopAct_Rb, SpeedLoopWant_Rb);
		
		Lf = speeddutyLf;
		Rf = speeddutyRf;
		Lb = speeddutyLb;
		Rb = speeddutyRb;
		
		getv.get_Vy = (-SpeedLoopAct_Lf + SpeedLoopAct_Rf - SpeedLoopAct_Lb + SpeedLoopAct_Rb) / 4;
    getv.get_Vx = (SpeedLoopAct_Lf + SpeedLoopAct_Rf- SpeedLoopAct_Lb - SpeedLoopAct_Rb) / 4;

    left_front(-1* speeddutyLf);
    right_front(speeddutyRf);
    left_behind(speeddutyLb);
    right_behind(-1*speeddutyRb);
}
