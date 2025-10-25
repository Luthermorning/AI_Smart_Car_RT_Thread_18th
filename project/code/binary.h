#ifndef BINARY
#define BINARY
#include "zf_common_headfile.h"
#include <rthw.h>
#define KEY_C15 (1<<0)
#define KEY_C14 (1<<1)
#define KEY_C13 (1<<2)
#define KEY_C12 (1<<3)
#define BUZZER_PIN B11
typedef struct {
    int x;
    int y;
} Point;

extern struct rt_semaphore camera_finish;
extern struct rt_semaphore image_finish;
extern struct rt_event *key_event;
extern struct rt_event *control_event;
extern rt_mailbox_t uart_mb;
extern rt_mailbox_t buzzer_mailbox;
extern uint8 event_c15;
extern uint8 binary_threshold;
extern void draw_points(uint8 *image, int x,int y);

extern int U_thres;
static void fastCornerDetection(uint8 *image, int width, int height);

int line_scan(uint8 *image);
void show_image_init(void);
void read_cemera_init(void);
void buzzer_init();
void white_edge(uint8 (*image)[SCC8660_W]);
void key();
int xunxian(uint8 (*image)[SCC8660_W], int x, int y,int block_size, int clip_value);
int translate(uint8 x,uint8 y);



void rgb_yuv_binary(uint16 (*image)[SCC8660_W],uint8 (*binary_image)[SCC8660_W]);

#endif