#include "bootpack.h"
#include <stdio.h>

struct TIMER *mt_timer;
int mt_tr;
struct TASKCTL *taskctl;
struct TIMER *task_timer;

struct TASK *task_init(struct MEMMAN * memman)
{
    int i;
    struct TASK *task;
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
    taskctl = (struct TASKCTL *)memman_alloc_4k(memman, sizeof (struct TASKCTL));
    for(i = 0; i< MAX_TASKS; i++)
    {
        taskctl->tasks0[i].flags = 0;
        taskctl->tasks0[i].sel = (TASK_GDT0 + i) * 8;
        set_segmdesc(gdt+TASK_GDT0+i, 103,(int)&taskctl->tasks0[i].tss,AR_TSS32);
    }
    task=task_alloc();
    task->flags = 2;
    taskctl->running = 1;
    taskctl->now = 0;
    taskctl->tasks[0] = task;
    load_tr(task->sel);
    task_timer = timer_alloc();
    timer_settime(task_timer, 2);
    return task;
}

struct TASK * task_alloc(void)
{
    int i;
    struct TASK * task;
    for(i = 0; i< MAX_TASKS; i++)
    {
        if(taskctl->tasks0[i].flags == 0)
        {
            task=&taskctl->tasks0[i];
            task->flags = 1;
            task->tss.eflags = 0x00000202;
            task->tss.eax=0;
            task->tss.ecx=0;
            task->tss.edx=0;
            task->tss.ebx=0;
            task->tss.ebp=0;
            task->tss.esi=0;
            task->tss.edi=0;
            task->tss.es=0;
            task->tss.ds=0;
            task->tss.fs=0;
            task->tss.gs=0;
            task->tss.ldtr=0;
            task->tss.iomap=0x40000000;
            return task;
       }
    }
    return 0;
}

void task_run(struct TASK *task)
{
    task->flags = 2;
    taskctl->tasks[taskctl->running]=task;
    taskctl->running++;
    return;
}

void task_switch(void)
{
    timer_settime(task_timer,2);
    if(taskctl->running>=2)
    {
        taskctl->now++;
        if(taskctl->now==taskctl->running)
        {
            taskctl->now = 0;
        }
        farjmp(0,taskctl->tasks[taskctl->now]->sel);
    }
    return;
}

void task_sleep(struct TASK *task)
{
    int i;
    char ts = 0;
    if(task->flags == 2)
    {
        if(task == taskctl->tasks[taskctl->now])
        {
            ts = 1;
        }
        for(i=0;i<taskctl->running;i++)
        {
            if(taskctl->tasks[i]==task)
            {
                break;
            }
        }
        taskctl->running--;
        if(i<taskctl->now)
        {
            taskctl->now--;

        }
        for(;i<taskctl->running;i++)
        {
            taskctl->tasks[i] = taskctl->tasks[i+1];
        }
        task->flags = 1;
        if(ts != 0)
        {
            if(taskctl->now = taskctl->running)
            {
                taskctl->now= 0;
            }
            farjmp(0,taskctl->tasks[taskctl->now]->sel);
        }
    }
    return;
}

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
    struct TIMER *timer_1s;
    int i, fifobuf[128], count = 0, count0  = 0;
    char s[12];

    fifo32_init(&fifo,128,fifobuf,0);
    timer_1s=timer_alloc();
    timer_init(timer_1s,&fifo,100);
    timer_settime(timer_1s,100);

    for(;;)
    {
        count++;
        io_cli();
        if(fifo32_status(&fifo)==0)
        {
            io_sti();
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
        }
        
    }
    
}
