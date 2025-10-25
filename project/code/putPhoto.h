#ifndef __PUTPHOTO_H__
#define __PUTPHOTO_H__

#include "zf_common_headfile.h"

void putPhoto_init();
extern bool yellowLinesend;
extern rt_sem_t BackCarport_control_sem;
extern int Xcarportgo;
extern float Ycarportgo;
extern float XVCgo;
extern float YVCgo;

#endif