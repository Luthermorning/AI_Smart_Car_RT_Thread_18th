#ifndef __INERTIAL_NAVIGATION_H__
#define __INERTIAL_NAVIGATION_H__

#include "zf_common_headfile.h"
#include "imu_config.h"

typedef struct { // 2D惯导中运用到的所有参数
    //加速度转换
    float accX_ob;
    float accY_ob;

    //速度更新
    float Now_Vx;
    float Last_Vx;
    float Now_Vy;
    float Last_Vy;

    //位置更新
    float Now_X;
    float Now_Y;
    float Now_go;

    float New_X;
    float New_Y;
    float New_stop;

    float all_X;
    float all_Y;
    float all_go;

} INS2DPara;

void INS_Init();
void TurnAcc(INS2DPara* INS_para, ICMdara* icm_data);
void Update_INS(float dt);

#endif