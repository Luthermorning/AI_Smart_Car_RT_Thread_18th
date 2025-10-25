#include "angleLoop.h"
#include "pid_all.h"
#include "imu_config.h"

PositionalPIDpara anglePIDpara;
float Anglewant = 0;
int anglePidMax=30;

extern ICMdara icm_data; //处理后的六轴数据
extern float expect_w;

//角度环pid初始化
void anglePID_Init(void)
{
    int i;

    anglePIDpara.Kd = 12;
    anglePIDpara.Ki = 0.045;
    anglePIDpara.Kp = 0.65;

    anglePIDpara.Deriv = 0;
    anglePIDpara.Err = 0;
    anglePIDpara.Integ = 0;
    anglePIDpara.LastErr = 0;
    anglePIDpara.out = 0;
    anglePIDpara.out_D = 0;
    anglePIDpara.out_I = 0;
    anglePIDpara.out_P = 0;
    anglePIDpara.num_integral = 14;
    anglePIDpara.list_index = 0;
    for (i = 0; i < anglePIDpara.num_integral; i++)
        anglePIDpara.list_value[i] = 0;
}

//void anglebias(void)
//{
//    float bias;
//    bias = PositionalPID(&anglePIDpara, Anglewant, icm_data.yaw, anglePidMax);
//    // printf("%d\n", (int16)icm_data.yaw);
//    expect_w = bias;
//}
