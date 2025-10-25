#ifndef __MASTERCONTROL_H__
#define __MASTERCONTROL_H__

#include "zf_common_headfile.h"

extern float planover[4];

void masterControl_init();
void control(void);
void planover_Init(void);
void control_text();

#endif
