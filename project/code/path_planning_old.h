#ifndef __PATH_PLANNING_OLD_H__
#define __PATH_PLANNING_OLD_H__

#include "zf_common_headfile.h"
#include "inertial_navigation.h"

#define photoN 30 //场地内图片数量
#define AMOUNT 30

void text_Init(void);

float coordinateTransformationX(int16 targetPiont, bool changeFlag);
float coordinateTransformationY(int16 targetPiont, bool changeFlag);

void plan_Init(void);
void DistanceCalculate(INS2DPara* INS_para);
int16 GoalSeeking(void);

#endif
