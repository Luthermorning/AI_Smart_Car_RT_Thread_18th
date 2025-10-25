#ifndef __KEYONE_H__
#define __KEYONE_H__
#include "zf_common_headfile.h"
#define KEY_C15 (1<<0)
#define KEY_C14 (1<<1)
#define KEY_C13 (1<<2)
#define KEY_C12 (1<<3)

extern struct rt_event *key_event;
extern struct rt_event *control_event;
extern bool tianoo_start;

void key_kkk(void);
void keyone_init(void);


#endif