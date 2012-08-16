#include <rtthread.h>

/* 定时器的控制块 */
static struct rt_timer timer1;
static struct rt_timer timer2;

/* 定时器1超时函数 */
static void timeout1(void* parameter)
{
    rt_kprintf("periodic timer is timeout\n");
}

/* 定时器2超时函数 */
static void timeout2(void* parameter)
{
    rt_kprintf("one shot timer is timeout\n");
}

int rt_application_init()
{
    /* 初始化定时器 */
    rt_timer_init(&timer1, "timer1",  /* 定时器名字是 timer1 */
        timeout1, /* 超时时回调的处理函数 */
        RT_NULL, /* 超时函数的入口参数 */
        1000, /* 定时长度，以OS Tick为单位，即1000个OS Tick */
        RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */
    rt_timer_init(&timer2, "timer2",   /* 定时器名字是 timer2 */
        timeout2, /* 超时时回调的处理函数 */
        RT_NULL, /* 超时函数的入口参数 */
        5000, /* 定时长度为5000个OS Tick */
        RT_TIMER_FLAG_ONE_SHOT); /* 单次定时器 */

    /* 启动定时器 */
    rt_timer_start(&timer1);
    rt_timer_start(&timer2);
    
    return 0;
}

