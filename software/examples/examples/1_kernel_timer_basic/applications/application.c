#include <rtthread.h>

static rt_thread_t tid = RT_NULL;

/* 定时器的控制块 */
static rt_timer_t timer1;
static rt_uint8_t count;

/* 定时器超时函数 */
static void timeout1(void* parameter)
{
    rt_kprintf("periodic timer is timeout\n");

    count ++;
    /* 停止定时器自身 */
    if (count >= 8)
    {
        /* 停止定时器 */
        rt_timer_stop(timer1);
        count = 0;
    }
}

static void thread_entry(void* parameter)
{
	timer1 = rt_timer_create("timer1",  /* 定时器名字是 timer1 */
        timeout1, /* 超时时回调的处理函数 */
        RT_NULL, /* 超时函数的入口参数 */
        10, /* 定时长度，以OS Tick为单位，即10个OS Tick */
        RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */
    /* 启动定时器 */
    if (timer1 != RT_NULL)
        rt_timer_start(timer1);
}

int rt_application_init()
{
		
	 tid = rt_thread_create("t",
        thread_entry, RT_NULL, /* 线程入口是thread_entry, 入口参数是RT_NULL */
        1024, 8, 50);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    /* 创建定时器1 */
    
		return 0;
}
