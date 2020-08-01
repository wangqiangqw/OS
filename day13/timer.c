#include "bootpack.h"

#define PIT_CTRL    0X0043
#define PIT_CNT0    0X0040
#define TIMER_FLAGS_ALLOC   1
#define TIMER_FLAGS_USING   2
struct TIMERCTL timerctl;

void init_pit(void)
{
    int i;
    io_out8(PIT_CTRL, 0X34);
    io_out8(PIT_CNT0, 0X9C);
    io_out8(PIT_CNT0, 0X2E);
    timerctl.count=0;
    timerctl.next = 0xffffffff;
    for(i =0 ;i<MAX_TIMER;i++)
    {
        timerctl.timers0[i].flags = 0;
    }
    return;
}

struct TIMER *timer_alloc(void)
{
    int i;
    for(i=0; i< MAX_TIMER; i++)
    {
        if(timerctl.timers0[i].flags == 0)
        {
            timerctl.timers0[i].flags = TIMER_FLAGS_ALLOC;
            return &timerctl.timers0[i];
        }
    }
}

void timer_free(struct TIMER * timer)
{
    timer->flags = 0;
    return;
}

void timer_init(struct TIMER * timer, struct FIFO32 * fifo, int data)
{
    timer->fifo = fifo;
    timer ->data = data;
    return;
}

void timer_settime(struct TIMER * timer, unsigned int timeout)
{
    int e, i, j;
    timer->timeout = timeout + timer->timeout;
    timer->flags = TIMER_FLAGS_USING;
    e=io_load_eflags();
    io_cli();
    for(i=0;i<timerctl.using;i++)
    {
        if(timerctl.timers[i]->timeout>=timer->timeout)
        {
            break;
        }
    }
    for(j=timerctl.using;j>i;j--)
    {
        timerctl.timers[j] = timerctl.timers[j-1];
    }
    timerctl.using++;
    timerctl.timers[i] = timer;
    timerctl.next = timerctl.timers[0]->timeout;
    io_store_eflags(e);
    return;
}

void inthandler20(int *esp)
{
    int i,j;
    io_out8(PIC0_OCW2,0x60);
    timerctl.count++;
    if(timerctl.next>timerctl.count)
    {
        return;
    }
    for(i = 0; i< timerctl.using;i++)
    {
        if(timerctl.timers[i]->timeout>timerctl.count)
        {
            break;
        }
        timerctl.timers[i]->flags = TIMER_FLAGS_ALLOC;
        fifo32_put(timerctl.timers[i]->fifo,timerctl.timers[i]->data);
    }
    timerctl.using -=i;
    for(j=0;j<timerctl.using;j++)
    {
        timerctl.timers[j]= timerctl.timers[i+j];
    }
    if(timerctl.using>0){
        timerctl.next = timerctl.timers[0]->timeout;
    } else {
        timerctl.next = 0xffffffff;
    }


    return;
}

/*void settimer(unsigned int timeout, struct FIFO8 *fifo, unsigned char data)
{
    int eflags;
    eflags = io_load_eflags();
    io_cli();
    timerctl.timeout = timeout;
    timerctl.fifo = fifo;
    timerctl.data = data;
    io_store_eflags(eflags);
    return;
}*/