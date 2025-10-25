/*********************************************************************************************************************
* RT1064DVL6A Opensourec Library 即（RT1064DVL6A 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 RT1064DVL6A 开源库的一部分
* 
* RT1064DVL6A 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.33
* 适用平台          RT1064DVL6A
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-09-21        SeekFree            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "YJX.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库移植用空工程

extern int16 SpeedLoopAct_Lf;
extern int16 SpeedLoopAct_Rf;
extern int16 SpeedLoopAct_Lb;
extern int16 SpeedLoopAct_Rb;
extern INS2DPara INS_para;

extern IncrementalPIDpara SpeedLoopParaLf;
extern IncrementalPIDpara SpeedLoopParaRf;
extern IncrementalPIDpara SpeedLoopParaLb;
extern IncrementalPIDpara SpeedLoopParaRb;
extern float XXstopgo;
extern float YXstopgo;
extern GetV getv;
float expect_w = 0;

extern int16 binary_control;
extern int16 photokey;
extern int16 coordinateRead_over;

extern int16 pointNum_openartmini;

int ab = 0;

int main(void)
{
//    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
//    debug_init();                   // 调试端口初始化
    // 此处编写用户代码 例如外设初始化代码等
//	do{
//	gpio_init(B9, GPO, 1, GPO_PUSH_PULL);
//	}while((imu660ra_init()));
	
	do{
	gpio_init(B9, GPO, 1, GPO_PUSH_PULL);
	}while((icm20602_init()));
	
	speed_Init();
	SpeedRead_Init();
	dongzuo_init();
	system_delay_ms(2000);
	GyroOffset_Init();
	AccOffset_Init();
	kalmanInit();
	speedLoop_init(&SpeedLoopParaLf);
	speedLoop_init(&SpeedLoopParaRf);
	speedLoop_init(&SpeedLoopParaLb);
	speedLoop_init(&SpeedLoopParaRb);
	anglePID_Init();
	get4wheels_init(&getv);
	INS_Init();
	planPIDx_Init();
	planPIDy_Init();
	placePIDx_Init();
	placePIDy_Init();
//	bluetooth_init();
//	wireless_uart_init();
	openartmini_init();
//	two_comInit();
//	carry_Init();
//	text_Init();
	
	//线程初始化
	timer_pit_init();
	buzzer_init();
	majorCalculate_init();
//	keyone_init();
	key_kkk();
	masterControl_init();
//	bluetooth_debug_init();
	openartmini_debug_init();
	read_cemera_init();
//	show_image_init();
	tianoo_init();
	putPhoto_init();
//	text_init();
  gpio_init(B9, GPO, 0, GPO_PUSH_PULL);
	
	gpio_init(B16, GPO, 0, GPO_PUSH_PULL);//右
	gpio_init(B17, GPO, 0, GPO_PUSH_PULL);//前
	gpio_init(B18, GPO, 0, GPO_PUSH_PULL);//左
	gpio_init(B19, GPO, 0, GPO_PUSH_PULL);//后
    // 此处编写用户代码 例如外设初始化代码等
    while(1)
    {
        // 此处编写需要循环执行的代码
      gpio_toggle_level(B9);
			if(pointNum_openartmini==-1){
//				printf("%d", 1);
				pointNum_openartmini = 0;
			}
			if(!gpio_get_level(C12)){//若遇到扫描坐标纸bug 则按下C12按键，直接开始跑遍历
//					photokey = -1;
//					coordinateRead_over = 1;
					binary_control = 2;	//开始无框图片的遍历
					printf("%d", 3);
					rt_thread_mdelay(300);
			}
//			if(!gpio_get_level(C12)){
//				tdong = 0;
//				binary_control = -1;
//				text_flag = 1;
//			}
			
//			if(!ab){//调试回库用，记得删
//				binary_control = 4;
//				ab = 1;
//			}
				
//		uart_write_string(UART_4, "0");	
//		printf("%d,%d,%d,%d,%d,%d,%d\n",(int16)XXstopgo, (int16)YXstopgo, (int16)INS_para.Now_X, (int16)INS_para.Now_Y, (int16)INS_para.all_X, (int16)INS_para.all_Y, (int16)camera_finish.value, binary_control);
//			wireless_rev();
			rt_thread_mdelay(300);
        
        // 此处编写需要循环执行的代码
    }
}

int32_t fputc (int32_t ch, FILE* f)
{
    uart_write_byte(UART_4, (ch & 0xFF));
    return ch;
}

void carry_Init()
{
	gpio_init(D17, GPO, 0, GPO_PUSH_PULL);
	pwm_init(PWM1_MODULE3_CHA_D0, 50, 200);
}

void two_comInit(void){
	uart_init(UART_8, 9600, UART8_TX_D16, UART8_RX_D17);
//	uart_init(UART_1, 9600, UART1_TX_B12, UART1_RX_B13);
	uart_rx_interrupt(UART_8, 1);
	interrupt_set_priority(LPUART8_IRQn, 1);
}