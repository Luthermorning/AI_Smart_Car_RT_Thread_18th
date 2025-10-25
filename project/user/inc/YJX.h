#ifndef __YJX_H__
#define __YJX_H__

//湮鲸虾队函数
#include "motor.h"
#include "encoder.h"
#include "timer_pit.h"
#include "buzzer.h"
#include "pid_all.h"
#include "SpeedLoop.h"
#include "imu_config.h"
#include "majorCalculate.h"
#include "angleLoop.h"
#include "filtering.h"
#include "masterControl.h"
#include "planLoop.h"
#include "inertial_navigation.h"
#include "path_planning_old.h"
#include "bluetooth_debug.h"
#include "binary.h"
#include "placeLoop.h"
#include "openartmini.h"
#include "wireless.h"
#include "tianoo.h"
#include "banyun.h"
#include "putPhoto.h"
#include "keyone.h"
#include "text.h"


//车模参数
#define a 0.2
#define b 0.4

//pi的值
#define Pi 3.14

//实际图片的数量
#define photoNum 12

//单搬放图片的边界位置
//5*7的场地 左-2 前26 右37 后-2
#define left_boundary -2
#define front_boundary 26
#define right_boundary 37
#define back_boundary -2

//遍历跑左右极限
//5*7场地 左3 右30 行数24
#define traverse_left 3
#define traverse_right 30
#define row_number 21

//多搬放图片位置
//5*7场地 右边36 前边27 左边-2 回库时到3
#define right_manyPut 36
#define front_manyPut 26
#define left_manyPut -2
#define behind_manyPut 3

void carry_Init();
void two_comInit(void);

#endif
