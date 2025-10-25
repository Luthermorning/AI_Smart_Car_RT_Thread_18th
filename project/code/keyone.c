#include "keyone.h"

struct rt_event *key_event;
struct rt_event *control_event;

bool tianoo_start = 0;

extern int16 start;

void keyone_entry(void *parameter){
	while(1){
		rt_uint32_t e;

		if (rt_event_recv(key_event, (KEY_C15|KEY_C14|KEY_C13|KEY_C12),
                      RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER, &e) == RT_EOK)
    {
			if((e&KEY_C15)==KEY_C15){
//				tianoo_start = 1;//直接开始遍历
			}
			if((e&KEY_C14)==KEY_C14){
//				tianoo_start = 1;//直接开始遍历
			}
			if((e&KEY_C13)==KEY_C13){
//				tianoo_start = 1;//直接开始遍历
			}
			if((e&KEY_C12)==KEY_C12){
				tianoo_start = 1;//直接开始遍历
			}
		}
	}
		
}

void key_kkk(void)
{
	gpio_init(C12,GPI,1,GPI_FLOATING_IN);
//	exti_init(C12, EXTI_TRIGGER_FALLING);  
////	exti_enable(C12);
	gpio_init(C13,GPI,1,GPI_FLOATING_IN);	
//	exti_init(C13, EXTI_TRIGGER_FALLING);
////	exti_enable(C13);	
////	gpio_init(C14,GPI,0,GPI_FLOATING_IN);	
//	exti_init(C14, EXTI_TRIGGER_FALLING);
////	exti_enable(C14);	
////	gpio_init(C15,GPI,0,GPI_FLOATING_IN);	
//	exti_init(C15, EXTI_TRIGGER_FALLING);                                        
////	exti_enable(C15);
	
//	interrupt_set_priority(GPIO2_Combined_0_15_IRQn, 0);
}

void keyone_init(void)
{
		control_event = rt_event_create("control_event", RT_IPC_FLAG_PRIO);
		key_event = rt_event_create("key_event", RT_IPC_FLAG_PRIO);
    rt_thread_t tid = RT_NULL;
    /* 创建线程 1 */
    tid = rt_thread_create("keyone_entry",
                            keyone_entry, RT_NULL,
                            1024,
                            8, 5);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
}
