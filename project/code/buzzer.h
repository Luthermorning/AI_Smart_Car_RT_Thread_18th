#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "zf_common_headfile.h"

extern rt_mailbox_t buzzer_mailbox;

void buzzer_init(void);

#endif