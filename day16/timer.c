#include "bootpack.h"

#define PIT_CTRL    0x0043
#define PIT_CNT0    0x0040
struct TIMERCTL timerctl;
#define TIMER_FLAGS_ALLOC   1
#define TIMER_FLAGS_USING   2
extern struct TIMER *mt_timer;
extern struct TIMER *task_timer;
void init_pit(void)
{
    int i;
    struct TIMER *t;
    io_out8(PIT_CTRL, 0x34);
    io_out8(PIT_CNT0, 0x9c);
    io_out8(PIT_CNT0, 0x2e);
    timerctl.count=0;

    for(i =0 ;i<MAX_TIMER;i++)
    {
        timerctl.timers0[i].flags = 0;
    }
    t=timer_alloc();
    t->timeout = 0xffffffff;
    t->flags=TIMER_FLAGS_USING;
    t->nextTimer=0;
    t->fifo=0; // D A N G E R !!!  no fifo, make sure  fifo never used in this timer.
	timerctl.t0 = t;
	timerctl.next = 0xffffffff;
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
	return 0;
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
    timer->nextTimer=0;
    return;
}

void timer_settime(struct TIMER * timer, unsigned int timeout)
{
    int e;
    struct TIMER *t, *s;
    timer->timeout = timeout + timerctl.count;
    timer->flags = TIMER_FLAGS_USING;
    e=io_load_eflags();
    io_cli();
    t = timerctl.t0;
    if(timer->timeout<=t->timeout)
    {
        timerctl.t0=timer;
        timer->nextTimer= t;
        timerctl.next = timer->timeout;
    }
    else
    {
        s=t;
        t=s->nextTimer;
        for(;;)
        {
            if(t==0)
                break;
            if(timer->timeout<=t->timeout)
            {
                timer->nextTimer=t;
                s->nextTimer=timer;
                break;
            }
            s=t;
            t=t->nextTimer;
        }
    }
    

    io_store_eflags(e);
    return;
}

void inthandler20(int *esp)
{

    char ts = 0;
    io_out8(PIC0_OCW2,0x60);
    timerctl.count++;
    if(timerctl.next>timerctl.count)
    {
        return;
    }
    struct TIMER *s, *t;
    s=timerctl.t0;
    t=s->nextTimer;
    for(;;)
    {
        if(s->timeout<=timerctl.count)
        {
            s->flags=TIMER_FLAGS_ALLOC;
            if(s!=task_timer)
            {
            fifo32_put(s->fifo,s->data);

            }
            else
            {
                ts = 1;
            }
            
            s=t;
            t=t->nextTimer;
        }
        else
        {
            break;
        }
        
    }
    timerctl.next = s->timeout;
    timerctl.t0 = s;
    if(ts!=0)
    {
        task_switch();
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

void adjustTimerCtl ( void )
{
    int t0 = timerctl.count;
    int i;
    io_cli();
    timerctl.count-=t0;
    for(i = 0;i<MAX_TIMER; i++)
    {
        if(timerctl.timers0[i].flags == TIMER_FLAGS_USING)
        {
            if(timerctl.timers0[i].timeout != 0xffffffff)
                timerctl.timers0[i].timeout -= t0;
        }
    }
    io_sti();
}