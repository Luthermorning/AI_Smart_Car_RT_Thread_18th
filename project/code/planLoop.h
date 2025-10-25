#ifndef __PLANLOOP_H__
#define __PLANLOOP_H__

#include "zf_common_headfile.h"

//#define Xmax 20//x方向的最大
//#define Ymax 22//y方向的最大
#define planPIDMax 30

void planPIDx_Init();
void planPIDy_Init();

#endif