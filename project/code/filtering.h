#ifndef __FILTERING_H__
#define __FILTERING_H__

#include "zf_common_headfile.h"
#include "imu_config.h"

typedef struct kalman {
    float Q;
    float R;
    float X;
    float P;
} kalman_para;

void kalmanInit(void);
float kalmanfliter(kalman_para* data, float z, float dataReal);
void FristHubu_Filer();

#endif
