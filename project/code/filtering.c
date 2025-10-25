#include "YJX.h"

kalman_para kalman_dara;
kalman_para AccXkalman_dara;
kalman_para AccYkalman_dara;

extern ICMdara icm_data; 
extern GyroPara GyroOffset;
extern AccPara AccOffset;
//extern rt_event_t gyro_event;

//extern int16 imu660ra_gyro_x, imu660ra_gyro_y, imu660ra_gyro_z;
//extern int16 imu660ra_acc_x, imu660ra_acc_y, imu660ra_acc_z;

extern int16 icm20602_gyro_x, icm20602_gyro_y , icm20602_gyro_z;            // 三轴陀螺仪数据      gyro (陀螺仪)
extern int16 icm20602_acc_x, icm20602_acc_y, icm20602_acc_z;               // 三轴加速度计数据    acc (accelerometer 加速度计)

//float acc_angle = 0;
float gyro_angle = 0;
double gyro_con = 0;
// double yaw = 0;
float alphaA = 0.3;
float alphaB = 0.5;
float dtt = 0.01;
double gyroz_mid;
float accx_mid;
float accy_mid;
void FristHubu_Filer()
{
    static float gyro_angle_Last = 0;
    static float gyro_angle_LastLast = 0;
    gyroz_mid = kalmanfliter(&kalman_dara, icm20602_gyro_z, GyroOffset.Zdata);
	  accx_mid = kalmanfliter(&AccXkalman_dara, icm20602_acc_x, AccOffset.Xdata);
    accy_mid = kalmanfliter(&AccYkalman_dara, icm20602_acc_y, AccOffset.Ydata);

    icm_data.gyro_z = icm20602_gyro_transition(gyroz_mid);
		icm_data.acc_x = icm20602_acc_transition(accx_mid);
		icm_data.acc_y = icm20602_acc_transition(accy_mid);

    if (icm_data.gyro_z > 0.6 || icm_data.gyro_z < -0.6)
        gyro_angle = icm_data.gyro_z;
    else
        gyro_angle = 0;

    //三阶牛顿科特斯公式
    gyro_con = (dtt / 6) * (gyro_angle + 4 * gyro_angle_Last + gyro_angle_LastLast) + icm_data.yaw;
    gyro_angle_LastLast = gyro_angle_Last;
    gyro_angle_Last = gyro_angle;

    if (gyro_con < 180 && gyro_con > (-180))
        icm_data.yaw = gyro_con;
    else {
        if (gyro_con > 0)
            icm_data.yaw = gyro_con - 360;
        else
            icm_data.yaw = gyro_con + 360;
    }
		
//		rt_event_send(gyro_event, FirstHubu_InitOver);
}

void kalmanInit(void)
{
    kalman_dara.R = icm_data.fangcha;
    kalman_dara.P = 0;
    kalman_dara.Q = 0.15;
    kalman_dara.X = 0;

    AccXkalman_dara.R = AccOffset.fangchaX;
    AccXkalman_dara.P = 0;
    AccXkalman_dara.Q = 0.15;
    AccXkalman_dara.X = 0;

    AccYkalman_dara.R = AccOffset.fangchaY;
    AccYkalman_dara.P = 0;
    AccYkalman_dara.Q = 0.15;
    AccYkalman_dara.X = 0;
}

//一阶卡尔曼滤波
float kalmanfliter(kalman_para* data, float z, float dataReal)
{
    float Xpre = data->X - dataReal;
    float Ppre = data->P + data->Q;
    float k = Ppre / (Ppre + data->R);
    data->X = Xpre + k * (z - Xpre);
    data->P = (1 - k) * Ppre;

    return data->X;
}