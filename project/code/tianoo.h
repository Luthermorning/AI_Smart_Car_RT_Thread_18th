#ifndef __TIANOO_H__
#define __TIANOO_H__

#include "zf_common_headfile.h"

extern int16 tianRun;
extern float stack_allx;
extern float stack_ally;
extern float XXtiango;
extern float YXtiango;
extern bool Xback;

void tianoo_init();
void startPiont(void);
void traverse(int16 direction);

#endif