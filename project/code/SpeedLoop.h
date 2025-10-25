#ifndef __SPEEDLOOP_H__
#define __SPEEDLOOP_H__

#include "zf_common_headfile.h"
#include "pid_all.h"

typedef struct { //编码器读到的实际参量
    float get_Vx;
    float get_Vy;
    // float get_w;

    float get_angle;
} GetV;

void speedLoop_init(IncrementalPIDpara* SpeedLoopPara);
void SpeedPIDdutySet_AND_Get4wheels(void);
void get4wheels_init(GetV* getv);

#endif
