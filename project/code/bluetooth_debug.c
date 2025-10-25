#include "YJX.h"

extern float visited[AMOUNT][1][3];
extern int16 coordinateRead_over;

uint8 bluetooth_data[64];                                                        // 串口接收数据缓冲区
uint8 bluetooth_fifo_get_data[64];                                                        // fifo 输出读出缓冲区

uint8 bluetooth_get_data = 0;                                                             // 接收数据变量
uint32 bluetooth_fifo_data_count = 0;                                                     // fifo 数据个数

fifo_struct bluetooth_data_fifo;

rt_sem_t bluetooth_sem = RT_NULL;							// 创建蓝牙串口中断的信号量的指针

uint8 bluetooth_index = 0;

int16 pointNum = 0;

void bluetooth_init()
{
	fifo_init(&bluetooth_data_fifo, FIFO_DATA_8BIT, bluetooth_data, 64);              // 初始化 fifo 挂载缓冲区
	
	uart_init(UART_4, 9600, UART4_TX_C16, UART4_RX_C17);
	uart_rx_interrupt(UART_4, 1);
	interrupt_set_priority(LPUART4_IRQn, 0);
}

void bluetooth_debug_handler()
{
			uart_query_byte(UART_4, &bluetooth_get_data);                                     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
			fifo_write_buffer(&bluetooth_data_fifo, &bluetooth_get_data, 1);                           // 将数据写入 fifo 中
			
			uint8 mid = bluetooth_get_data;
			if (bluetooth_index < 8){
					if (mid != 32) {
						switch (bluetooth_index) {
						case 0:
								if (mid != '(') {
										fifo_clear(&bluetooth_data_fifo);
										bluetooth_index = 0;
								} else {
										bluetooth_index++;
								}
								break;
						case 2:
								if (mid != ',') {
										fifo_clear(&bluetooth_data_fifo);
										bluetooth_index = 0;
								} else {
										bluetooth_index++;
								}
								break;
						case 4:
								if (mid != ',') {
										fifo_clear(&bluetooth_data_fifo);
										bluetooth_index = 0;
								} else {
										bluetooth_index++;
								}

								break;
						case 6:
								if (mid != ')') {
										fifo_clear(&bluetooth_data_fifo);
										bluetooth_index = 0;
								} else {
										bluetooth_index++;
								}
								break;
						default:
								bluetooth_index++;
								break;
						}
				}
				if (bluetooth_index == 7){
						rt_sem_release(bluetooth_sem);
				}
			}
			else {
				fifo_clear(&bluetooth_data_fifo);
				bluetooth_index = 0;
			}
}

void bluetooth_debug (void *parameter)
{
	while(1){
		if((rt_sem_take(bluetooth_sem, RT_WAITING_FOREVER)) == RT_EOK){
			bluetooth_fifo_data_count = fifo_used(&bluetooth_data_fifo);                           // 查看 fifo 是否有数据
			if(bluetooth_fifo_data_count != 0)
			{
				fifo_read_buffer(&bluetooth_data_fifo, bluetooth_fifo_get_data, &bluetooth_fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
				if(bluetooth_fifo_data_count==8)
				{
					  uart_write_string(UART_4, "\r\nUART get data:");                // 输出测试信息
            uart_write_buffer(UART_4, bluetooth_fifo_get_data, bluetooth_fifo_data_count);      // 将读取到的数据发送出去
						if(pointNum<photoNum){
							text_Init();
							pointNum++;
							if(pointNum==photoNum){
								coordinateRead_over = 1;
							}
						}
						bluetooth_index = 0;
				}
			}
		}
	}
}

void bluetooth_debug_init()
{
	rt_thread_t tid;
	bluetooth_sem = rt_sem_create("bluetooth_sem", 0 ,RT_IPC_FLAG_FIFO);

	tid = rt_thread_create("bluetooth_debug",							// 线程名称
		bluetooth_debug,											// 线程入口函数
		RT_NULL,												// 线程参数
		1024,													// 1024 个字节的栈空间
		11,														// 线程优先级为12，数值越小，优先级越高，0为最高优先级。
																// 可以通过修改rt_config.h中的RT_THREAD_PRIORITY_MAX宏定义(默认值为8)来修改最大支持的优先级
		5);														// 时间片为5

	if(tid != RT_NULL)											// 线程创建成功
	{
		//运行该线程
		rt_thread_startup(tid);
	}
}
