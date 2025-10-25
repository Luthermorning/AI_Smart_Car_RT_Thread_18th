#include "YJX.h"
#include <math.h>

rt_sem_t binary_controlChange = RT_NULL;

uint8 copy_image[SCC8660_H][SCC8660_W];
uint8 grey_image[SCC8660_H][SCC8660_W];
uint8 out_image[SCC8660_H][SCC8660_W];
struct rt_semaphore image_finish;
struct rt_semaphore camera_finish;
uint8 binary_threshold=126;
int seed = 0;
int seedx = 0;
int seedy = 0;
uint8 trans_image[500][460];
uint8 exclude_area = 20;
uint8 exclude_area_front = 40;
#define NUM_POINTS 16
#define THRESHOLD 20
#define youhua
extern rt_sem_t binary_control_sem;
uint8 wide = 20;

	
Point edge_points[SCC8660_W*SCC8660_H];

Point cornerpoint[SCC8660_W*SCC8660_H]={0};


AT_ITCM_SECTION_INIT(void adaptiveThreshold(uint8_t* img_data, uint8_t* output_data, uint16 width, uint16 height, int block, uint8_t clip_value)){
  assert(block % 2 == 1); // block必须为奇数
  int half_block = block / 2;
  for(int y=half_block; y<height-half_block; y++){
    for(int x=half_block; x<width-half_block; x++){
      // 计算局部阈值
      int thres = 0;
      for(int dy=-half_block; dy<=half_block; dy++){
        for(int dx=-half_block; dx<=half_block; dx++){
          thres += img_data[(x+dx)+(y+dy)*width];
        }
      }
      thres = thres / (block * block) - clip_value;
      // 进行二值化
      output_data[x+y*width] = img_data[x+y*width]>thres ? 255 : 0;
    }
  }
}

void binary(uint8_t* img_data, uint8_t* output_data, uint16 width, uint16 height,uint8 thres){
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
					output_data[x+y*width] = img_data[x+y*width]>thres ? 255 : 0;
			}
		}
}

uint16 mid_x,mid_y=0;

float XVgo_lin;
float YVgo_lin;
extern PositionalPIDpara PlaceXpara;
extern PositionalPIDpara PlaceYpara;
extern float XVVgo;
extern float YVVgo;
extern PositionalPIDpara PlanXpara;
extern PositionalPIDpara PlanYpara;

float beita = 0.2;
int mid_point;
int dong=0;
int dong2=0;
extern float expect_w;
extern float SpeedLoopWant_Lf;
extern float SpeedLoopWant_Rf;
extern float SpeedLoopWant_Lb;
extern float SpeedLoopWant_Rb;
extern int16 photokey;
extern INS2DPara INS_para;
extern float visited[AMOUNT][1][3];
extern int16 binary_control;
void read_cemera_entry(){
	scc8660_init();
	rt_sem_init(&camera_finish,"camera_finish",0,RT_IPC_FLAG_PRIO);
	while(1){
		if((rt_sem_take(binary_control_sem, RT_WAITING_FOREVER)) == RT_EOK){
			rt_sem_take(&camera_finish, RT_WAITING_FOREVER);
			
			if(tianRun == 4){
				XVVgo = -PositionalPID(&PlanXpara, INS_para.Now_X, XXtiango, planPIDMax, 1);
				YVVgo = -PositionalPID(&PlanYpara, INS_para.Now_Y, YXtiango, planPIDMax, 1);
				SpeedLoopWant_Lf = -(XVVgo + YVVgo + expect_w * (a + b));
				SpeedLoopWant_Rf = -(XVVgo - YVVgo + expect_w * (a + b));
				SpeedLoopWant_Lb = (XVVgo - YVVgo - expect_w * (a + b));
				SpeedLoopWant_Rb = (XVVgo + YVVgo - expect_w * (a + b));
			}
			
			if(binary_control == 1){//有框图用
				wide = 20;
				exclude_area_front = 60;
			}
			else if(binary_control == 3){//无框图用
				wide = 25;
				exclude_area_front = 5;
			}
			
			rgb_yuv_binary(scc8660_image,copy_image);

			white_edge(&copy_image[0]);
			seed = line_scan((uint8 *)copy_image);
			seedy = seed>>16;
			seedx = (uint16)seed;
			mid_point = xunxian(copy_image,seedx,seedy,7,9);
			mid_x =mid_point>>16;
			mid_y = (uint16)mid_point;
			//draw_points((uint8 *)copy_image,seedx,seedy);
			//draw_points((uint8 *)copy_image,mid_x,mid_y);
			
			rt_sem_release(binary_controlChange);
//			rt_sem_release(&image_finish);
		}
	}
}

int16 wantmid_x = 224;
int16 wantmid_y = 419;
int16 send=0;
int16 stackFlag = 0;
int16 guolvCishu = 0; 
extern int16 carryFlag;
extern GetV getv;

void binary_control_entry(){
	while(1){
		if((rt_sem_take(binary_controlChange, RT_WAITING_FOREVER)) == RT_EOK){
			if(binary_control == 1){//有框图用
				if(!(mid_x==0||mid_y==0)){
					XVgo_lin = PositionalPID(&PlaceXpara, mid_x, wantmid_x, placePIDMax, 0);
					YVgo_lin = -PositionalPID(&PlaceYpara, mid_y, wantmid_y, placePIDMax, 0);
					SpeedLoopWant_Lf = -(XVgo_lin + YVgo_lin + expect_w * (a + b));
					SpeedLoopWant_Rf = -(XVgo_lin - YVgo_lin + expect_w * (a + b));
					SpeedLoopWant_Lb = (XVgo_lin - YVgo_lin - expect_w * (a + b));
					SpeedLoopWant_Rb = (XVgo_lin + YVgo_lin - expect_w * (a + b));
				}
				else{//看不到图片就原地晃一晃
//							SpeedLoopWant_Lf = -(expect_w * (a + b));//看不见图片停车
//							SpeedLoopWant_Rf = -(expect_w * (a + b));
//							SpeedLoopWant_Lb = -(expect_w * (a + b));
//							SpeedLoopWant_Rb = -(expect_w * (a + b));
					
						if(dong<95){
							SpeedLoopWant_Lf = -3-(expect_w * (a + b));
							SpeedLoopWant_Rf = 3-(expect_w * (a + b));
							SpeedLoopWant_Lb = -3-(expect_w * (a + b));
							SpeedLoopWant_Rb = 3-(expect_w * (a + b));
							dong++;
						}else if(95<=dong&&dong<210){
							SpeedLoopWant_Lf = 3-(expect_w * (a + b));
							SpeedLoopWant_Rf = -3-(expect_w * (a + b));
							SpeedLoopWant_Lb = 3-(expect_w * (a + b));
							SpeedLoopWant_Rb = -3-(expect_w * (a + b));
							dong++;
						}else if(210<=dong&&dong<230){
							SpeedLoopWant_Lf = -3-(expect_w * (a + b));
							SpeedLoopWant_Rf = 3-(expect_w * (a + b));
							SpeedLoopWant_Lb = -3-(expect_w * (a + b));
							SpeedLoopWant_Rb = 3-(expect_w * (a + b));
							dong++;
						}else if(230<=dong&&dong<270){
							SpeedLoopWant_Lf = 3-(expect_w * (a + b));
							SpeedLoopWant_Rf = 3-(expect_w * (a + b));
							SpeedLoopWant_Lb = -3-(expect_w * (a + b));
							SpeedLoopWant_Rb = -3-(expect_w * (a + b));
							dong++;
						}else if(270<=dong&&dong<350){
							SpeedLoopWant_Lf = -3-(expect_w * (a + b));
							SpeedLoopWant_Rf = -3-(expect_w * (a + b));
							SpeedLoopWant_Lb = 3-(expect_w * (a + b));
							SpeedLoopWant_Rb = 3-(expect_w * (a + b));
							dong++;
						}else if(350<=dong&&dong<390){
							SpeedLoopWant_Lf = 3-(expect_w * (a + b));
							SpeedLoopWant_Rf = 3-(expect_w * (a + b));
							SpeedLoopWant_Lb = -3-(expect_w * (a + b));
							SpeedLoopWant_Rb = -3-(expect_w * (a + b));
							dong++;
							if(dong==390){
								INS_para.Now_X = 0;
								INS_para.Now_Y = 0;
								binary_control = 4;//看不到了就回库
								dong=0;
							}
						}
					} 
				
				if((fabs(mid_x-wantmid_x)<=3)&&(fabs(mid_y-wantmid_y)<=3)){	
					if((fabs(getv.get_Vx)<=0.5)&&(fabs(getv.get_Vy)<=0.5)){
							SpeedLoopWant_Lf = -(expect_w * (a + b));
							SpeedLoopWant_Rf = -(expect_w * (a + b));
							SpeedLoopWant_Lb = -(expect_w * (a + b));
							SpeedLoopWant_Rb = -(expect_w * (a + b));
						if(send==0){
		//						uart_write_string(UART_4, "0");	
							rt_mb_send(buzzer_mailbox, 400);
							printf("%d", 0);//该openart干活辣
		//						wireless_uart_send_string("0");
							mid_x = 0;
							mid_y = 0;
							send = 1;
						}
					}
				}
			}else if((binary_control == 3)&&((tianRun == 1)||(tianRun == 0)||(tianRun == 4))){//无框图遍历时用
				if(!(mid_x==0||mid_y==0)){
					guolvCishu++;
					if(guolvCishu>5){
						if(stackFlag==0){
	//						stack_allx = INS_para.all_X;//将被打断的坐标压入堆栈
	//						stack_ally = INS_para.all_Y;
							stackFlag = 1;
						}
						tianRun = 0;
						XVgo_lin = PositionalPID(&PlaceXpara, mid_x, wantmid_x, placePIDMax, 0);
						YVgo_lin = -PositionalPID(&PlaceYpara, mid_y, wantmid_y, placePIDMax, 0);
						SpeedLoopWant_Lf = -(XVgo_lin + YVgo_lin + expect_w * (a + b));
						SpeedLoopWant_Rf = -(XVgo_lin - YVgo_lin + expect_w * (a + b));
						SpeedLoopWant_Lb = (XVgo_lin - YVgo_lin - expect_w * (a + b));
						SpeedLoopWant_Rb = (XVgo_lin + YVgo_lin - expect_w * (a + b));
					}
				}else{
					if(dong2<20){
						SpeedLoopWant_Lf = 3-(expect_w * (a + b));
						SpeedLoopWant_Rf = -3-(expect_w * (a + b));
						SpeedLoopWant_Lb = 3-(expect_w * (a + b));
						SpeedLoopWant_Rb = -3-(expect_w * (a + b));
						dong++;
					}
					if(dong2 == 20)
						tianRun = 3;
				}
				
				if((fabs(mid_x-wantmid_x)<=3)&&(fabs(mid_y-wantmid_y)<=3)){
					if((fabs(getv.get_Vx)<=0.5)&&(fabs(getv.get_Vy)<=0.5)){
							SpeedLoopWant_Lf = -(expect_w * (a + b));
							SpeedLoopWant_Rf = -(expect_w * (a + b));
							SpeedLoopWant_Lb = -(expect_w * (a + b));
							SpeedLoopWant_Rb = -(expect_w * (a + b));
							dong2 = 0;
							guolvCishu = 0;
							if(send==0){
								rt_mb_send(buzzer_mailbox, 400);
								printf("%d", 0);//该openart干活辣
								mid_x = 0;
								mid_y = 0;
								send = 1;
							}
					}
				}
			}

		}
	}
}

void read_cemera_init(void)
{
	binary_controlChange = rt_sem_create("binary_controlChange", 0 ,RT_IPC_FLAG_FIFO);
	
    rt_thread_t tid = RT_NULL;
    /* 创建线程 1 */
    tid = rt_thread_create("read_cemera",
                            read_cemera_entry, RT_NULL,
                            10244,
                            13, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
		
		tid = rt_thread_create("binary_control",
                            binary_control_entry, RT_NULL,
                            2048,
                            14, 5);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}


void show_image_entry(){
	rt_sem_init(&image_finish,"image_finish",0,RT_IPC_FLAG_PRIO);
	ips200_init(IPS200_TYPE_PARALLEL8);
	while(1){
		rt_sem_take(&image_finish, RT_WAITING_FOREVER);
		
			//ips200_show_gray_image(0, 0, copy_image[0], SCC8660_W, SCC8660_H, SCC8660_W, SCC8660_H,0);
//		//ips200_show_gray_image(10, 0, out_image[0], SCC8660_W, SCC8660_H, SCC8660_W, SCC8660_H,0);
		 ips200_show_rgb565_image(0, 0, scc8660_image[0], SCC8660_W, SCC8660_H, SCC8660_W, SCC8660_H, 0);
		
		//ips200_show_gray_image(0, 0, trans_image[0], 460, 500, 230, 250,0);
		}
		
	}
	
void show_image_init(void)
{
    rt_thread_t tid = RT_NULL;
    /* 创建线程 1 */
    tid = rt_thread_create("show_image",
                            show_image_entry, RT_NULL,
                            1024,
                            18, 5);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}





int isCorner(uint8* image, int width, int height, int x, int y) {
    unsigned char center = image[y * width + x];
    int count= 0;

    Point points[NUM_POINTS] = {
        {-3, 0}, {-3, -1}, {-2, -2}, {-1, -3},
        {0, -3}, {1, -3}, {2, -2}, {3, -1},
        {3, 0}, {3, 1}, {2, 2}, {1, 3},
        {0, 3}, {-1, 3}, {-2, 2}, {-3, 1}
    };
		
		for (int i = 0; i < 16; i+=4) {
        int px = x + points[i].x;
        int py = y + points[i].y;
				unsigned char pixel = image[py * width + px];
				if(abs(pixel-center) > THRESHOLD){
					count++;
				}
		}
		if(count==3){
		count=0;
    for (int i = 0; i < NUM_POINTS; i++) {
        int px = x + points[i].x;
        int py = y + points[i].y;
        if (px < 0 || px >= width || py < 0 || py >= height) {
            continue;
        }
        unsigned char pixel = image[py * width + px];
        if (pixel > center + THRESHOLD) {
            count++;
        } else if (pixel < center - THRESHOLD) {
            count++;
        }
    }
		if(count>=10)return 1;
		else return 0;
    }
		

    return 0;
}

void fastCornerDetection(uint8 *image, int width, int height) {
	int aa = 0;
	memset(cornerpoint,0,sizeof(cornerpoint));
    for (int y = 3; y < height - 3; y++) {
        for (int x = 3; x < width - 3; x++) {
            if (isCorner(image, width, height, x, y)) {
							cornerpoint[aa].x = x;
							cornerpoint[aa].y = y;		
							aa++;
            }
        }
    }
}


void draw_points(uint8 *image, int x,int y){
		uint8 pixel;
		if(abs(x-SCC8660_W)>3&&abs(y-SCC8660_H)>3){
    Point points[NUM_POINTS] = {
        {-3, 0}, {-3, -1}, {-2, -2}, {-1, -3},
        {0, -3}, {1, -3}, {2, -2}, {3, -1},
        {3, 0}, {3, 1}, {2, 2}, {1, 3},
        {0, 3}, {-1, 3}, {-2, 2}, {-3, 1}
    };
		for (int i = 0; i < NUM_POINTS; i++){
        int px = x + points[i].x;
        int py = y + points[i].y;
				pixel = image[py * SCC8660_W + px];
				if(pixel==255){
					image[py * SCC8660_W + px]=0;
				}
				else if(pixel==0){
					image[py * SCC8660_W + px]=255;
				}
		}
	}
}

AT_ITCM_SECTION_INIT(int line_scan(uint8 *image)){
	int line_count[SCC8660_H-exclude_area];
	for(int i =exclude_area_front;i<SCC8660_H-exclude_area;i++){
		int count = 0;
		for(int j=0;j<SCC8660_W;j++){
			if(image[i*SCC8660_W+j]==255){
				count++;
			}
		}
		line_count[i]=count;
	}
	int max = line_count[exclude_area_front];
	int maxline = 0;
	for(int i =exclude_area_front+1;i<SCC8660_H-exclude_area;i++){
		if(line_count[i]>max && line_count[i]<SCC8660_W-50 && line_count[i]>wide){
			max = line_count[i];
			maxline = i;
		}
	}
	int count1max = 0;
	int count1 = 0;
	int index=0;
	int indexmax=0;
	for(int i=0;i<SCC8660_W;i++){
		if(image[maxline*SCC8660_W+i]==255){
			if(count1==0){
				index = i;
			}
			count1++;
		}
		else{
			if(count1>count1max){
				count1max=count1;
				indexmax = index;
			}
			count1=0;

		}
	}
	return (maxline<<16)+indexmax;
}

void white_edge(uint8 (*image)[SCC8660_W]){
	for(int i =0;i<SCC8660_W;i++){
		image[exclude_area_front][i] = 0;
		image[1+exclude_area_front][i] = 0;
		image[SCC8660_H-exclude_area+1][i] = 0;
		image[SCC8660_H-exclude_area][i] = 0;


	}
	for(int i =0;i<SCC8660_H;i++){
		image[i][0] = 0;
		image[i][1] = 0;
		image[i][SCC8660_W-2] = 0;
		image[i][SCC8660_W-1] = 0;
	}
}



AT_ITCM_SECTION_INIT(int xunxian(uint8 (*image)[SCC8660_W], int x, int y,int block_size, int clip_value)){
#ifndef youhua
	
	for(int i =0;i<SCC8660_H;i++){
		for(int j=0;j<SCC8660_W;j++){
			out_image[i][j]=0;
		}
	}
	for(int i =0;i<500;i++){
		for(int j=0;j<460;j++){
			trans_image[i][j]=0;
		}
	}
#endif
	if((abs(x-SCC8660_W)==0||abs(y-SCC8660_H)==0||x==0||y==0)){
		return 0;
	}
	int i_x =x;
	int i_y =y;
	int all_y,all_x=0;
	int half = block_size / 2;
	Point dir_front[4] = {{0,-1},{1,0},{0,1},{-1,0}};
	Point dir_frontleft[4] = {{-1,-1},{1,-1},{1,1},{-1,1}};
	int step = 0, dir = 0, turn = 0;
	while(step<1500&&x>0&&y>0&&x<SCC8660_W&&y<SCC8660_H&&turn<4){
//			int local_thres = 0;
//			for (int dy = -half; dy <= half; dy++) {
//					for (int dx = -half; dx <= half; dx++) {
//							local_thres += image[y+dy][x+dx];
//					}
//			}
//			local_thres /= block_size * block_size;
//			local_thres -= clip_value;
		int current_value = image[y][x];
		int front_value = image[y+dir_front[dir].y][x+dir_front[dir].x];
		int frontleft_value = image[y+dir_frontleft[dir].y][x+dir_frontleft[dir].x];
		if (front_value==0) {
				dir = (dir + 1) % 4;
				turn++;
		} else if (frontleft_value ==0) {
				x += dir_front[dir].x;
				y += dir_front[dir].y;
				out_image[y][x] = 255;
				int tran = translate(x,y);
				int y_t = (uint16)tran;
				int x_t = tran>>16;
				if(y_t>0&&x_t>0&&y_t<500&&x_t<460){
#ifndef youhua
				trans_image[y_t][x_t]=255;
#endif
				all_y+=y_t;
				all_x+=x_t;
				}
					
				
				step++;
				turn = 0;
			if(x==i_x&&y==i_y) break;
		} else {
				x += dir_frontleft[dir].x;
				y += dir_frontleft[dir].y;
				dir = (dir + 3) % 4;
				out_image[y][x] = 255;
				int tran = translate(x,y);
				int y_t = (uint16)tran;
				int x_t = tran>>16;
			
				if(y_t>0&&x_t>0&&y_t<500&&x_t<460){
#ifndef youhua
				trans_image[y_t][x_t]=255;
#endif
				all_y+=y_t;
				all_x+=x_t;
				}
				step++;
				turn = 0;
			if(x==i_x&&y==i_y) break;
		}
		
	}
	all_x/=step;
	all_y/=step;
	return (all_x<<16)+all_y;
}



//int U_thres = 15;
//AT_ITCM_SECTION_INIT(void rgb_yuv_binary(uint16 (*image)[SCC8660_W],uint8 (*binary_image)[SCC8660_W])){
//	for(int i =0;i<SCC8660_H;i++){
//		for(int j =0;j<SCC8660_W;j++){
//			uint8 red = (image[i][j]>>11)<<3;
//			uint8 green = ((image[i][j]<<5)>>10)<<2;
//			uint8 blue = ((image[i][j]<<11)>>11)<<3;
//			//uint8 Y =0.299 * red + 0.587 * green + 0.114 * blue;
//			int U =(-11075 * red - 21692 * green +32703 * blue)>>16;
//			//uint 8 U =-0.169 * red - 0.331 * green + 0.499 * blue;

//			//uint8 V = 0.499 * red - 0.439 * green - 0.081 * blue;
//	
//			if(U>U_thres)
//			binary_image[i][j] = 0;
//			else
//			binary_image[i][j] = 255;

//		
//		}
//	}
//	
//}

int U_thres = 140;
AT_ITCM_SECTION_INIT(void rgb_yuv_binary(uint16 (*image)[SCC8660_W],uint8 (*binary_image)[SCC8660_W])){
	for(int i =0;i<SCC8660_H;i++){
		for(int j =0;j<SCC8660_W;j+=2){
			uint8 u0 = (uint8)(image[i][j]>>8);
			if(u0>U_thres){
			binary_image[i][j] = 255;
			binary_image[i][j+1]=255;
			}
			else{
			binary_image[i][j] = 0;
			binary_image[i][j+1]=0;
			}
		}
	}
	
}


int translate(uint8 x,uint8 y){
	int x1 = (4096*x+6758*y-266649)/(29*y+375);
	int y1 = (16465*y-369770)/(29*y+375);
	return (x1<<16)+y1;
	
}
