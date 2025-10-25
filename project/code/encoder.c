#include "YJX.h"

ECPULSE ECPULSE_v;

/**************************************************************************/
//————————————————编码器速度获取——————————————
void SpeedRead_Init(void) //编码器初始化
{
//    encoder_dir_init(QTIMER_1, QTIMER1_TIMER0_C0, QTIMER1_TIMER1_C1);
//    encoder_dir_init(QTIMER_1, QTIMER1_TIMER2_C2, QTIMER1_TIMER3_C24);
//    encoder_dir_init(QTIMER_2, QTIMER2_TIMER0_C3, QTIMER2_TIMER3_C25);
//    encoder_dir_init(QTIMER_3, QTIMER3_TIMER2_B18, QTIMER3_TIMER3_B19);
		encoder_dir_init(QTIMER2_ENCODER2, QTIMER2_ENCODER2_CH1_C5, QTIMER2_ENCODER2_CH2_C25);
		encoder_dir_init(QTIMER2_ENCODER1, QTIMER2_ENCODER1_CH1_C3, QTIMER2_ENCODER1_CH2_C4);
		encoder_dir_init(QTIMER1_ENCODER1, QTIMER1_ENCODER1_CH1_C0, QTIMER1_ENCODER1_CH2_C1);
		encoder_dir_init(QTIMER1_ENCODER2, QTIMER1_ENCODER2_CH1_C2, QTIMER1_ENCODER2_CH2_C24);
	
		ECPULSE_v.ECPULSELf = 0;
		ECPULSE_v.ECPULSERf = 0;
		ECPULSE_v.ECPULSELb = 0;
		ECPULSE_v.ECPULSERb = 0;
}

void SpeedRead(void)
{
    //获取速度
//    ECPULSE_v.ECPULSELf = qtimer_quad_get(QTIMER_1, QTIMER1_TIMER0_C0);
//    ECPULSE_v.ECPULSERf = qtimer_quad_get(QTIMER_1, QTIMER1_TIMER2_C2);
//    ECPULSE_v.ECPULSELb = qtimer_quad_get(QTIMER_2, QTIMER2_TIMER0_C3);
//    ECPULSE_v.ECPULSERb = qtimer_quad_get(QTIMER_3, QTIMER3_TIMER2_B18);	
	  ECPULSE_v.ECPULSELf = encoder_get_count(QTIMER2_ENCODER2);
    ECPULSE_v.ECPULSERf = encoder_get_count(QTIMER2_ENCODER1);
    ECPULSE_v.ECPULSELb = encoder_get_count(QTIMER1_ENCODER1);
    ECPULSE_v.ECPULSERb = encoder_get_count(QTIMER1_ENCODER2);

    //清除
//	    qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER0_C0);
//    qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER2_C2);
//    qtimer_quad_clear(QTIMER_2, QTIMER2_TIMER0_C3);
//    qtimer_quad_clear(QTIMER_3, QTIMER3_TIMER2_B18);
    encoder_clear_count(QTIMER2_ENCODER2);
    encoder_clear_count(QTIMER2_ENCODER1);
    encoder_clear_count(QTIMER1_ENCODER1);
    encoder_clear_count(QTIMER1_ENCODER2);
}
