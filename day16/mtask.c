#include "bootpack.h"
#include <stdio.h>

struct TIMER *mt_timer;
int mt_tr;

void mt_init(void)
{
    mt_timer = timer_alloc();
    timer_settime(mt_timer,2);
    mt_tr = 3*8;
    return;
}

void mt_taskswitch(void)
{
    if(mt_tr == 3*8)
    {
        mt_tr = 4*8;
    }
    else
    {
        mt_tr = 3*8;
    }
    timer_settime(mt_timer,2);
    farjmp(0,mt_tr);
    return;
    
}

void task_b_main(struct SHEET *sht_back)
{
	struct FIFO32 fifo;
    struct TIMER * timer_ts, *timer_put,*timer_1s;
    int i, fifobuf[128], count = 0, count0  = 0;
    char s[12];

    //sht_back = (struct SHEET *)*((int*)0X0FEC);
    fifo32_init(&fifo,128,fifobuf);
    timer_ts=timer_alloc();
    timer_init(timer_ts,&fifo,2);
    timer_settime(timer_ts,2);
    timer_put=timer_alloc();
    timer_init(timer_put,&fifo,1);
    timer_settime(timer_put,1);
    timer_1s=timer_alloc();
    timer_init(timer_1s,&fifo,100);
    timer_settime(timer_1s,100);

    for(;;)
    {
        count++;
        io_cli();
        if(fifo32_status(&fifo)==0)
        {
            io_stihlt();
        }
        else
        {
            i=fifo32_get(&fifo);
            io_sti();
            if(i==100)
            {
                sprintf(s,"%11d",count - count0);
                putfonts8_asc_sht(sht_back,90,144,COL8_FFFFFF,COL8_008484,s,11);
                count0 = count;
                timer_settime(timer_1s,100);

            }
            else
            {
            switch(i)
            {
            case 1:
                sprintf(s,"%11d",count);
                putfonts8_asc_sht(sht_back,0,144,COL8_FFFFFF,COL8_008484,s,11);
                timer_settime(timer_put,1);
                break;
            case 2:
                //farjmp(0,3*8);
                timer_settime(timer_ts,2);
                break;

            default:
                break;
            }
                
            }
            
        }
        
    }
    
}
