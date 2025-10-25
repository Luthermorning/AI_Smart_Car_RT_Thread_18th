#ifndef __IMU_READ_H__
#define __IMU_READ_H__

#include "zf_common_headfile.h"

#define EVENT_FLAG1		(1<<1)
#define EVENT_FLAG3		(1<<3)

#define ICM20602gyro_ReadOver	EVENT_FLAG1
#define FirstHubu_InitOver	EVENT_FLAG3

typedef struct { //陀螺仪零点偏移三轴数据
    float Xdata;
    float Ydata;
    float Zdata;
    int16 gyroz[200];
} GyroPara;

typedef struct { //陀螺仪零点偏移三轴数据
    float Xdata;
    float Ydata;
    int16 accx[200];
    int16 accy[200];

    float fangchaX;
    float fangchaY;
} AccPara;

typedef struct { //陀螺仪处理后的六轴数据
    float gyro_x;
    float gyro_y;
    float gyro_z;

    float acc_x;
    float acc_y;
    float acc_z;

    float Angle_temp;
    float yaw;
    float fangcha;
} ICMdara;

void GyroOffset_Init(void);
void AccOffset_Init(void);
//void imu_config_init();

#endif
