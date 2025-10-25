#include "YJX.h"
#include "math.h"

float visited[AMOUNT][1][3] = { 0 }; //该三维矩阵中 0放置标志位 1为x轴 2为y轴
extern uint8 bluetooth_fifo_get_data[64];  
extern int16 pointNum;
extern int16 pointNum_wireless;
extern int16 pointNum_openartmini;

extern uint8 wireless_data_buffer[32];

extern rt_ubase_t openartmini_rev_data;

//测试用
void text_Init(void)
{
//			visited[pointNum][0][0] = 1;
//			visited[pointNum][0][1] = coordinateTransformationX((int16)(bluetooth_fifo_get_data[1]));
//			visited[pointNum][0][2] = coordinateTransformationY((int16)(bluetooth_fifo_get_data[3]));
	
//			visited[pointNum_wireless][0][0] = 1;
//			visited[pointNum_wireless][0][1] = coordinateTransformationX((int16)(wireless_data_buffer[1])-48);
//			visited[pointNum_wireless][0][2] = coordinateTransformationY((int16)(wireless_data_buffer[3])-48);
			
			uint8 point_x, point_y;
			point_y = openartmini_rev_data;//解码
			openartmini_rev_data = openartmini_rev_data>>8;
			point_x = openartmini_rev_data;
			visited[pointNum_openartmini][0][0] = 1;
			visited[pointNum_openartmini][0][1] = coordinateTransformationX(point_x, 1);
			visited[pointNum_openartmini][0][2] = coordinateTransformationY(point_y, 1);
//			if((point_y==0)&&(point_x==0))
//				pointNum_openartmini = photoNum;
				
	
//    visited[0][0][0] = 1;
//    visited[0][0][1] = coordinateTransformationX(0);
//    visited[0][0][2] = coordinateTransformationY(0);

//    visited[0][0][0] = 1;
//    visited[0][0][1] = coordinateTransformationX(0);
//    visited[0][0][2] = coordinateTransformationY(5);
//	
//	  visited[1][0][0] = 1;
//    visited[1][0][1] = coordinateTransformationX(0);
//    visited[1][0][2] = coordinateTransformationY(10);
}

float coordinateTransformationX(int16 targetPiont, bool changeFlag)//changeFlag为1时进行除以5的精度换算
{
    float realDistance;
		if(!changeFlag)
			realDistance = (targetPiont)*5.06;
		else
			realDistance = (targetPiont/5.0)*5.06;
//		realDistance = (targetPiont);

    return realDistance;
}

float coordinateTransformationY(int16 targetPiont, bool changeFlag)//changeFlag为1时进行除以5的精度换算
{
    float realDistance;
		if(!changeFlag)
			realDistance = (targetPiont)*5.06;
		else
			realDistance = (targetPiont/5.0)*5.06;
//		realDistance = (targetPiont);

    return realDistance;
}

//路径规划
int16 locationcount = 0;
float distance[AMOUNT] = { 0 };
float targetdistance = 0;
float targetangle = 0;
float photoStopgo = 0;
int16 planCount = 0;
int16 photokey = 0;

float XXstopgo = 0;
float YXstopgo = 0;

extern INS2DPara INS_para;

/**************************************************************************/
//————————————————路径规划——————————————
//注意在使用这个函数前要将A4纸上的
//该函数得到应该往x和y方向走的距离以及总距离
//该函数需要每次搬运完之后都运行一次
void plan_Init(void)
{
    planCount++;
    float Xstopgo = 0;
    float Ystopgo = 0;
    DistanceCalculate(&INS_para);
    photokey = GoalSeeking();
    visited[photokey][0][0] = 0; //应在确定位置是否到达图片地点后进行赋0

    if (planCount == 1) {
        Xstopgo = visited[photokey][0][1] - INS_para.all_X;
        Ystopgo = visited[photokey][0][2] - INS_para.all_Y;
    } else {
        Xstopgo = visited[photokey][0][1] - INS_para.all_X;
        Ystopgo = visited[photokey][0][2] - INS_para.all_Y;

//        if (Xstopgo > 0)
//            Xstopgo += 20 * (1.0 / 4.0);
//        else if (Xstopgo < 0)
//            Xstopgo -= 20 * (1.0 / 4.0);

//        if (Ystopgo > 0)
//            Ystopgo += 20 * (1.0 / 4.0);
//        else if (Ystopgo < 0)
//            Ystopgo -= 20 * (1.0 / 4.0);
    }

    XXstopgo = Xstopgo;
    YXstopgo = Ystopgo;

//    photoStopgo = sqrt(Xstopgo * Xstopgo + Ystopgo * Ystopgo);
}

//该函数计算当前点到未访问的图片的距离
//储存在distance这个数组中

float distance_mid;
float distance_midx;
float distance_midy;
void DistanceCalculate(INS2DPara* INS_para)
{
    int16 photokeyDis = 0;
    while (photokeyDis <= AMOUNT) {
        if (visited[photokeyDis][0][0] != 0){ //标志位若不为0则表示该点未被访问
						distance_midx = (INS_para->all_X - visited[photokeyDis][0][1]) * (INS_para->all_X - visited[photokeyDis][0][1]);
						distance_midy = (INS_para->all_Y - visited[photokeyDis][0][2]) * (INS_para->all_Y - visited[photokeyDis][0][2]);
						distance_mid = sqrt(distance_midx + distance_midy);
        }else if (visited[photokeyDis][0][0] == 0)
            distance_mid = 0;
				distance[photokeyDis] = distance_mid;
        photokeyDis++;
    }
}

//寻找下一个目标
//返回标志值
int16 GoalSeeking(void)
{
    int16 photokeyGoal = 0;
    int16 i, seekFlag = 0;
    float distanceMin = 0;
    for (i = 0; i < AMOUNT; i++) {
        if (distanceMin == 0) { // distanceMin的一个初始化 只跑一遍
            if (visited[i][0][0] != 0) {
                distanceMin = distance[i];
                photokeyGoal = i;
                seekFlag++;
            }
            continue;
        }
        if (visited[i][0][0] != 0) {
            if (distance[i] < distanceMin) {
                distanceMin = distance[i];
                photokeyGoal = i;
            }
            seekFlag++;
        }
    }

    if (seekFlag)
        return photokeyGoal;
    else
        return -1;
}

