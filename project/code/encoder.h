#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "zf_common_headfile.h"

typedef struct{
	//编码器传值
	uint16 ECPULSELf;
	uint16 ECPULSERf;
	uint16 ECPULSELb;
	uint16 ECPULSERb;
}ECPULSE;

void SpeedRead_Init(void); //编码器初始化
void SpeedRead(void);

#endif
