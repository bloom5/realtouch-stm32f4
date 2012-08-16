/*
 * �����嵥����Ϣ��������
 *
 * �������ᴴ��1����̬�̣߳�����̻߳����Ϣ��������ȡ��Ϣ������
 * ͨ����ʱ����ʱ��������Ϣ���з�����Ϣ
 */
#include <rtthread.h>

/* ָ���߳̿��ƿ��ָ�� */
static rt_thread_t tid = RT_NULL;

/* ��Ϣ���п��ƿ� */
static struct rt_messagequeue mq;
/* ��Ϣ�������õ��ķ�����Ϣ���ڴ�� */
static char msg_pool[2048];

/* ��ʱ���Ŀ��ƿ� */
static struct rt_timer timer;
static rt_uint16_t no = 0;
static void timer_timeout(void* parameter)
{
    char buf[32];
    rt_uint32_t length;

    length = rt_snprintf(buf, sizeof(buf), "message %d", no++);
    rt_mq_send(&mq, &buf[0], length);
}

/* �߳���ں��� */
static void thread_entry(void* parameter)
{
    char buf[64];
    rt_err_t result;

    /* ��ʼ����ʱ�� */
    rt_timer_init(&timer, "timer",  /* ��ʱ�������� timer1 */
        timer_timeout, /* ��ʱʱ�ص��Ĵ����� */
        RT_NULL, /* ��ʱ��������ڲ��� */
        1000, /* ��ʱ���ȣ���OS TickΪ��λ����1000��OS Tick */
        RT_TIMER_FLAG_PERIODIC); /* �����Զ�ʱ�� */
    
    rt_timer_start(&timer);

    while (1)
    {
        rt_memset(&buf[0], 0, sizeof(buf));

        /* ����Ϣ�����н�����Ϣ */
        result = rt_mq_recv(&mq, &buf[0], sizeof(buf), RT_WAITING_FOREVER);
        if (result == RT_EOK)
        {
            rt_kprintf("recv msg: %s\n", buf);
        }
        else if (result == -RT_ETIMEOUT)
        {
            rt_kprintf("recv msg timeout\n");
        }
    }
}

int rt_application_init()
{
    rt_err_t result;

    /* ��ʼ����Ϣ���� */
    result = rt_mq_init(&mq, "mqt", 
        &msg_pool[0], /* �ڴ��ָ��msg_pool */ 
        128 - sizeof(void*), /* ÿ����Ϣ�Ĵ�С�� 128 - void* */
        sizeof(msg_pool), /* �ڴ�صĴ�С��msg_pool�Ĵ�С */
        RT_IPC_FLAG_FIFO); /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
    
    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }

    /* �����߳� */
    tid = rt_thread_create("t",
        thread_entry, RT_NULL, /* �߳������thread_entry, ��ڲ�����RT_NULL */
        1024, 8, 50);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    
    return 0;
}
