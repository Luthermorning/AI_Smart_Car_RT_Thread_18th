#include "YJX.h"
#include "math.h"

INS2DPara INS_para;
float accvx, accvy;

extern ICMdara icm_data;
extern GetV getv;

//惯导参数初始化
void INS_Init()
{
    INS_para.accX_ob = 0;
    INS_para.accY_ob = 0;
    INS_para.Last_Vx = 0;
    INS_para.Last_Vy = 0;

    INS_para.Now_Vx = 0;
    INS_para.Now_Vy = 0;
    INS_para.Now_X = 0;
    INS_para.Now_Y = 0;
}

//对应坐标轴加速度转换
void TurnAcc(INS2DPara* INS_para, ICMdara* icm_data)
{
    float turn = 180 / Pi;
    INS_para->accX_ob = icm_data->acc_x * cos(turn * icm_data->yaw) + icm_data->acc_y * sin(turn * icm_data->yaw);
    INS_para->accY_ob = -icm_data->acc_x * sin(turn * icm_data->yaw) + icm_data->acc_y * cos(turn * icm_data->yaw);
}

// 2D惯导更新算法
void Update_INS(float dt)
{
    //加速度转换
    // TurnAcc(euler_para, INS_para, icm_data);

    //速度更新
    // float beita = 0.5;

    // accvx = accvx + icm_data->acc_x * dt;
    // accvy = accvy + icm_data->acc_y * dt;

    // INS_para->Now_Vx = INS_para->Last_Vx + icm_data->acc_x * dt;
    // INS_para->Last_Vx = beita * getv->get_Vx + (1 - beita) * INS_para->Now_Vx;
    // INS_para->Now_Vy = INS_para->Last_Vy + icm_data->acc_y * dt;
    // INS_para->Last_Vy = beita * getv->get_Vy + (1 - beita) * INS_para->Now_Vy;

    //位置更新

    INS_para.Now_X += getv.get_Vx * dt*(-1);
    INS_para.Now_Y += getv.get_Vy * dt;

    INS_para.all_X += getv.get_Vx * dt*(-1);
    INS_para.all_Y += getv.get_Vy * dt;
//    INS_para.Now_go = sqrt(INS_para.Now_X * INS_para.Now_X + INS_para.Now_Y * INS_para.Now_Y);
    // INS_para->all_go = INS_para->Now_go; //暂时不知道有啥用
}
