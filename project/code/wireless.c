#include "YJX.h"

uint8 wireless_data_buffer[32];
uint8 wireless_data_len;

int16 pointNum_wireless = 0;
int8 flag_w = -1;
extern int16 coordinateRead_over;

uint8 classification;
extern int16 carryFlag;
extern int16 binary_control;

extern int16 photokey;
extern INS2DPara INS_para;
extern float visited[AMOUNT][1][3];

void wireless_rev()
{
	  wireless_data_len = wireless_uart_read_buff(wireless_data_buffer, 32);                    // 查看是否有消息 默认缓冲区是 WIRELESS_UART_BUFFER_SIZE 总共 64 字节
		if(wireless_data_len != 0)                                                       // 收到了消息 读取函数会返回实际读取到的数据个数
		{
			flag_w = wireless_data_buffer[5];
			if(flag_w=='0'){
				if(wireless_data_len==8){
					wireless_uart_send_string("\r\nUART get data:");
					wireless_uart_send_buff(wireless_data_buffer, wireless_data_len);                     // 将收到的消息发送回去
					if(pointNum_wireless<photoNum){
						text_Init();
						pointNum_wireless++;
						if(pointNum_wireless==photoNum){
							coordinateRead_over = 1;
						}
					}
				}
				memset(wireless_data_buffer, 0, 32);
				func_uint_to_str((char *)wireless_data_buffer, wireless_data_len);
//				wireless_uart_send_string("\r\ndata len:");                                 // 显示实际收到的数据信息
//				wireless_uart_send_buff(wireless_data_buffer, strlen((const char *)wireless_data_buffer));    // 显示收到的数据个数
//				wireless_uart_send_string(".\r\n");
			}
			if(flag_w=='1'){
				if(wireless_data_len==8){
					wireless_uart_send_string("\r\nUART get data:");
					wireless_uart_send_buff(wireless_data_buffer, wireless_data_len);                     // 将收到的消息发送回去
					classification =  wireless_data_buffer[1]-48;//第一位为识别结果
					binary_control = 0;//该me干活辣
					INS_para.Now_X = 0;
					INS_para.Now_Y = 0;
					INS_para.all_X = visited[photokey][0][1];
					INS_para.all_Y = visited[photokey][0][2];
//					interrupt_disable(CSI_IRQn);
					carryFlag = 1;//该进行搬运辣
				}
			}
		}
}
