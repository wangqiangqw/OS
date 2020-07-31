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
    for(i =0 ;i<MAX_TIMER;i++)
    {
        timerctl.timer[i].flags = 0;
    }
    return;
}

struct TIMER *timer_alloc(void)
{
    int i;
    for(i=0; i< MAX_TIMER; i++)
    {
        if(timerctl.timer[i].flags == 0)
        {
            timerctl.timer[i].flags = TIMER_FLAGS_ALLOC;
            return &timerctl.timer[i];
        }
    }
}

void timer_free(struct TIMER * timer)
{
    timer->flags = 0;
    return;
}

void timer_init(struct TIMER * timer, struct FIFO8 * fifo, unsigned char data)
{
    timer->fifo = fifo;
    timer ->data = data;
    return;
}

void timer_settime(struct TIMER * timer, unsigned int timeout)
{
    timer->timeout = timeout;
    timer->flags = TIMER_FLAGS_USING;
    return;
}

void inthandler20(int *esp)
{
    int i;
    io_out8(PIC0_OCW2,0x60);
    timerctl.count++;
    for(i = 0; i< MAX_TIMER; i++)
    {
        if(timerctl.timer[i].flags == TIMER_FLAGS_USING)
        {
            timerctl.timer[i].timeout--;
            if(timerctl.timer[i].timeout==0)
            {
                timerctl.timer[i].flags=TIMER_FLAGS_ALLOC;
                fifo8_put(timerctl.timer[i].fifo,timerctl.timer[i].data);
            }
        }

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