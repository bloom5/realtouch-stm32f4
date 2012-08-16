#include <rtthread.h>

/* 定时器的控制块 */
static rt_timer_t timer1;
static rt_uint8_t count;
static rt_tick_t tick;

/* 定时器超时函数 */
static void timeout1(void* parameter)
{
	rt_tick_t timeout = 3000;
	count ++;

	rt_kprintf("periodic timer is timeout\n");
	rt_kprintf("the time set is %d\n",(rt_tick_get()-tick));
 	tick = rt_tick_get();
		
  if(count >= 8)
	{ 
		/* 控制定时器然后更改超时时间长度 */
    rt_timer_control(timer1, RT_TIMER_CTRL_SET_TIME, (void *)&timeout);
				
    rt_timer_stop(timer1);
		rt_timer_control(timer1, RT_TIMER_CTRL_SET_ONESHOT, (void *)&timeout);
    rt_timer_start(timer1);
		
		count = 0;		
	}	
}

int rt_application_init()
{
    /* 创建定时器1 */
    timer1 = rt_timer_create("timer1",  /* 定时器名字是 timer1 */
        timeout1, /* 超时时回调的处理函数 */
        RT_NULL, /* 超时函数的入口参数 */
        100, /* 定时长度，以OS Tick为单位，即10个OS Tick */
        RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */
    /* 启动定时器 */
    if (timer1 != RT_NULL)
        rt_timer_start(timer1);
		
		tick = rt_tick_get();

    return 0;
}

