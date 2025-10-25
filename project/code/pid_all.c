#include "YJX.h"
#include "math.h"

int placePidMax=25;

//增量式pid
float IncrementalPID(IncrementalPIDpara* para, int16 Act, float Want){
    int16 ErrM;

    para->Err = Want - Act;
    para->out_P = para->Err - para->ErrLast;
    para->out_I = para->Err;
    para->out_D = para->Err - 2 * para->ErrLast + para->ErrLastLast;

    para->middle = (int16)(para->KP * para->out_P
        + para->KI * para->out_I
        + para->KD * para->out_D);
	
		if(para->middle>1200){
				para->middle=	1200;
		}else if(para->middle<-1200){
				para->middle=	-1200;
		}

    para->pwmDuty += para->middle;

    //用阈值限幅
    if (para->pwmDuty > 9000)
        para->pwmDuty = 9000;
    else if (para->pwmDuty < -9000)
        para->pwmDuty = -9000;

    ErrM = para->ErrLast;
    para->ErrLast = para->Err;
    para->ErrLastLast = ErrM;

    return para->pwmDuty;
}

//位置式pid
float PositionalPID(PositionalPIDpara* para, float Act, float Want, int PidMax, int16 per_flag)
{
		float percentt;
    para->Err = Want - Act;
		percentt = fabs(para->Err/Want);

    para->out_P = para->Err;
    para->out_I = para->Integ;
    //////////////////////////////////////////////////////////
    if (para->list_index < para->num_integral+1) {
        para->list_value[para->list_index] = para->Err;
        para->Integ += para->Err;
        para->list_index++;
    } else {
        para->Integ += para->Err - para->list_value[0];
        for (int i = 1; i < para->num_integral+1; i++) {
            para->list_value[i - 1] = para->list_value[i];
        }
        para->list_value[(int)(para->num_integral)] = para->Err;
    }

    //////////////////////////////////////////////////////////
    para->out_D = para->Err - para->LastErr;

    para->LastErr = para->Err;
		
		if(per_flag==1){
			if(percentt<0.3)
				para->out = para->Kp * para->out_P + para->Ki * para->out_I + para->out_D * para->Kd;
			else
				para->out = para->Kp * para->out_P + para->Kii * para->out_I + para->out_D * para->Kd;
		}else if(per_flag == 0){
			para->out = para->Kp * para->out_P + para->Ki * para->out_I + para->out_D * para->Kd;
		}else if(per_flag == 2){
			para->out = 0.3 * para->out_P + 0 * para->out_I + para->out_D * 0;
		}

    if (para->out > PidMax)
        para->out = PidMax;
    if (para->out < -PidMax)
        para->out = -PidMax;

    return para->out;
}
