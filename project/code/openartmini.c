#include "YJX.h"

extern PositionalPIDpara PlanXpara;
extern PositionalPIDpara PlanYpara;
extern float visited[AMOUNT][1][3];
extern int16 coordinateRead_over;
extern rt_sem_t binary_control_sem;
extern float expect_w;
extern float Anglewant;
extern int dong;

uint8 openartmini_data[64];                                                        // 串口接收数据缓冲区
uint8 openartmini_fifo_get_data[64];                                                        // fifo 输出读出缓冲区

uint8 openartmini_get_data = 0;                                                             // 接收数据变量
uint32 openartmini_fifo_data_count = 0;                                                     // fifo 数据个数

fifo_struct openartmini_data_fifo;

//rt_sem_t openartmini_sem = RT_NULL;							// 创建openartmini串口中断的信号量的指针

uint8 openartmini_index = 0;

int16 pointNum_openartmini = -1;

rt_mailbox_t openartmini_mailbox;//创建串口接受邮箱

void openartmini_init()
{
	fifo_init(&openartmini_data_fifo, FIFO_DATA_8BIT, openartmini_data, 64);              // 初始化 fifo 挂载缓冲区
	
	uart_init(UART_4, 9600, UART4_TX_C16, UART4_RX_C17);
//	uart_init(UART_1, 9600, UART1_TX_B12, UART1_RX_B13);
	uart_rx_interrupt(UART_4, 1);
	interrupt_set_priority(LPUART4_IRQn, 0);
//	interrupt_set_priority(LPUART1_IRQn, 1);
}

void openartmini_debug_handler()
{
			uart_query_byte(UART_4, &openartmini_get_data);                                     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
			fifo_write_buffer(&openartmini_data_fifo, &openartmini_get_data, 1);                           // 将数据写入 fifo 中
			
			uint8 mid = openartmini_get_data;
			if (openartmini_index < 8){
					if (mid != 32) {
						switch (openartmini_index) {
						case 0:
								if (mid != '(') {
										fifo_clear(&openartmini_data_fifo);
										openartmini_index = 0;
								} else {
										openartmini_index++;
								}
								break;
						case 2:
								if (mid != ',') {
										fifo_clear(&openartmini_data_fifo);
										openartmini_index = 0;
								} else {
										openartmini_index++;
								}
								break;
						case 4:
								if (mid != ',') {
										fifo_clear(&openartmini_data_fifo);
										openartmini_index = 0;
								} else {
										openartmini_index++;
								}

								break;
						case 6:
								if (mid != ')') {
										fifo_clear(&openartmini_data_fifo);
										openartmini_index = 0;
								} else {
										openartmini_index++;
								}
								break;
						default:
								openartmini_index++;
								break;
						}
					if (openartmini_index == 7){
		//						rt_sem_release(openartmini_sem);
						openartmini_fifo_data_count = fifo_used(&openartmini_data_fifo);
						fifo_read_buffer(&openartmini_data_fifo, openartmini_fifo_get_data, &openartmini_fifo_data_count, FIFO_READ_AND_CLEAN);
						rt_mb_send(openartmini_mailbox, ((int)(openartmini_fifo_get_data[1])<<16)+((int)(openartmini_fifo_get_data[3])<<8)+((int)(openartmini_fifo_get_data[5])));//压缩
						fifo_clear(&openartmini_data_fifo);
						openartmini_index = 0;
					}
				}else {
						fifo_clear(&openartmini_data_fifo);
						openartmini_index = 0;
				}
			} else {
						fifo_clear(&openartmini_data_fifo);
						openartmini_index = 0;
			}
}

uint8 flag = 9;
extern uint8 classification;
extern int16 carryFlag;
extern int16 binary_control;

extern int16 photokey;
extern INS2DPara INS_para;
extern float visited[AMOUNT][1][3];

extern int16 send;

rt_ubase_t openartmini_rev_data;

extern int bb;
uint8 bias_angle;
uint8 zhengfu;

void openartmini_debug (void *parameter)
{
	while(1){
		rt_mb_recv(openartmini_mailbox, (rt_ubase_t*)&openartmini_rev_data, RT_WAITING_FOREVER);
		flag = openartmini_rev_data;
		if(flag==0x00){
			openartmini_rev_data = openartmini_rev_data>>8;
			if(pointNum_openartmini<photoNum){
				text_Init();
				pointNum_openartmini++;
//				uart_write_string(UART_4, "\r\nUART get data:");                // 输出测试信息
//				printf("%d\n", pointNum_openartmini);
				if(pointNum_openartmini==photoNum-1){
					coordinateRead_over = 1;
				}
			}
		}else if(flag==0x01){	//读取识别结果
			if(send){
				openartmini_rev_data = openartmini_rev_data>>8;
				openartmini_rev_data = openartmini_rev_data>>8;
				classification =  openartmini_rev_data;//第一位为识别结果
				if(binary_control == 3){//无框图时用
					INS_para.Now_X = 0;
					INS_para.Now_Y = 0;
//					tianRun = 2;//该进行搬运辣
					
//					XXtiango = stack_allx - INS_para.all_X;
//					YXtiango = stack_ally - INS_para.all_Y;
					binary_control = 2;
					Xback = 0;
					binary_control_sem->value = 0;
					tianRun = 2;
				}else {//有框图用
					INS_para.Now_X = 0;
					INS_para.Now_Y = 0;
					INS_para.all_X = visited[photokey][0][1];
					INS_para.all_Y = visited[photokey][0][2];
					binary_control = 0;//该me干活辣
//				carryFlag = 2;
					carryFlag = 1;//该进行搬运辣
					dong = 0;
				}

				rt_mb_send(buzzer_mailbox, 900);
				send  = 0;
		//			bb = 0;//测试用
			}
		}else if(flag==0x07){
			openartmini_rev_data = openartmini_rev_data>>8;
			openartmini_rev_data = openartmini_rev_data>>8;
			Xcarportgo = openartmini_rev_data;
			if(Xcarportgo != 255)
				Xcarportgo = Xcarportgo * 4;
			else
				Xcarportgo = Xcarportgo;
			binary_control = 5;
			if(Xcarportgo == 255)
				yellowLinesend = 0;
			else
				XVCgo = PositionalPID(&PlanXpara, Xcarportgo, 85,  planPIDMax, 2);
		}
//		else if(flag==0x08){
//			openartmini_rev_data = openartmini_rev_data>>8;
//			zhengfu= openartmini_rev_data;
//			openartmini_rev_data = openartmini_rev_data>>8;
//			bias_angle = openartmini_rev_data;
//			if(!zhengfu)
//				bias_angle = bias_angle;
//			else
//				bias_angle = -bias_angle;
//			Anglewant = -bias_angle;
//		}
		
//		else if(flag==0x02){
//			openartmini_rev_data = openartmini_rev_data>>8;
//			openartmini_rev_data = openartmini_rev_data>>8;
//			classification =  openartmini_rev_data;//第一位为识别结果
//			if((classification >= 0) && (classification < 4)){
//				uart_write_string(UART_1, "(");
//				uart_write_byte(UART_1, (uint8)(visited[photokey][0][1]));
//				uart_write_string(UART_1, ",");
//				uart_write_byte(UART_1, (uint8)(visited[photokey][0][2]));
//				uart_write_string(UART_1, ",");
//				uart_write_byte(UART_1, flag);
//				uart_write_string(UART_1, ",");
//				uart_write_byte(UART_1, classification);
//				uart_write_string(UART_1, ")");
//				uart_write_string(UART_1, " ");
//			}else if((classification >= 10) && (classification < 13)){
//				uart_write_string(UART_1, "(");
//				uart_write_byte(UART_1, (uint8)(visited[photokey][0][1]));
//				uart_write_string(UART_1, ",");
//				uart_write_byte(UART_1, (uint8)(visited[photokey][0][2]));
//				uart_write_string(UART_1, ",");
//				uart_write_byte(UART_1, flag);
//				uart_write_string(UART_1, ",");
//				uart_write_byte(UART_1, classification);
//				uart_write_string(UART_1, ")");
//				uart_write_string(UART_1, " ");
//			}else if((classification >= 15) && (classification < 16)){
//				uart_write_string(UART_1, "(");
//				uart_write_byte(UART_1, (uint8)(visited[photokey][0][1]));
//				uart_write_string(UART_1, ",");
//				uart_write_byte(UART_1, (uint8)(visited[photokey][0][2]));
//				uart_write_string(UART_1, ",");
//				uart_write_byte(UART_1, flag);
//				uart_write_string(UART_1, ",");
//				uart_write_byte(UART_1, classification);
//				uart_write_string(UART_1, ")");
//				uart_write_string(UART_1, " ");
//			}
//			carryFlag = 2;
//			binary_control = 0;//该me干活辣
//			INS_para.Now_X = 0;
//			INS_para.Now_Y = 0;
//			INS_para.all_X = visited[photokey][0][1];
//			INS_para.all_Y = visited[photokey][0][2];	
//		}
	}
}

void openartmini_debug_init()
{
	rt_thread_t tid;
//	openartmini_sem = rt_sem_create("openartmini_sem", 0 ,RT_IPC_FLAG_FIFO);
	openartmini_mailbox = rt_mb_create("openartmini_mb", 20, RT_IPC_FLAG_FIFO);

	tid = rt_thread_create("openartmini_debug",							// 线程名称
		openartmini_debug,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 1024 个字节的栈空间
		12,														// 线程优先级为12，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		5);														// 时间片为5

	if(tid != RT_NULL)											// 线程创建成功
	{
		//运行该线程
		rt_thread_startup(tid);
	}
}
