#ifndef __TEXT_H__
#define __TEXT_H__

#include "zf_common_headfile.h"

extern rt_sem_t text_sem;
extern int text_flag;

void text_init(void);

#endif