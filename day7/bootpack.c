#include <stdio.h>
#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

void HariMain(void)
{

	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;

	char s[40], mcursor[256],keybuf[32],mousebuf[128];
	int mx, my;

	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC‚Ì‰Šú‰»‚ªI‚í‚Á‚½‚Ì‚ÅCPU‚ÌŠ„‚èž‚Ý‹ÖŽ~‚ð‰ðœ */
	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
	io_out8(PIC0_IMR, 0xf9); /* PIC1�ƃL�[�{�[�h������(11111001) */
	io_out8(PIC1_IMR, 0xef); /* �}�E�X������(11101111) */

	init_keyboard();
	init_palette();
	init_screen(binfo->vram,binfo->scrnx,binfo->scrny);
	mx = (binfo->scrnx - 16) / 2; /* ��ʒ����ɂȂ�悤�ɍ��W�v�Z */
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfont8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
	
	
	enable_mouse();

	putfont8_asc(binfo->vram, binfo->scrnx, 16, 32, COL8_FFFFFF, "enable mouse");


	mx=0;
	my=16;
	int m,n = 0;
    for(;;)
    {
        io_cli();
		if(fifo8_status(&keyfifo)+fifo8_status(&mousefifo)== 0)
		{
			io_stihlt();
		}
		else
		{
			unsigned char i =0;
			if(fifo8_status(&keyfifo)!=0)
			{
				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s,"%02X",i);
				boxfill8(binfo->vram,binfo->scrnx,COL8_008484,0/*mx+m*16*/,16/*my+n*16*/,15,31);
				putfont8_asc(binfo->vram,binfo->scrnx,0/*mx+m*16*/,16/*my+n*16*/,COL8_FFFFFF,s);

			}
			else if(fifo8_status(&mousefifo)!=0)
			{
				i = fifo8_get(&mousefifo);
				io_sti();
				sprintf(s,"%02X",i);
				boxfill8(binfo->vram,binfo->scrnx,COL8_008484,32/*mx+m*16*/,16/*my+n*16*/,47,31);
				putfont8_asc(binfo->vram,binfo->scrnx,32/*mx+m*16*/,16/*my+n*16*/,COL8_FFFFFF,s);
				/* code */
			}
			
			if(m==16)
			{
				m=0;
				n++;
			}
			m++;
		}

    }   
}

#define PORT_KEYDAT				0X0060
#define PORT_KEYSTA				0X0064
#define PORT_KEYCMD				0X0064
#define KEYSTA_SEND_NOTREADY	0X02
#define	KEYCMD_WRITE_MODE		0X60
#define KBC_MODE				0X47
#define KEYCMD_SENDTO_MOUSE		0XD4
#define MOUSECMD_ENABLE			0XF4

void wait_KBC_sendready(void)
{
	for(;;)
	{
		if((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0)
		{
			break;
		}
	}
	return;
}

void init_keyboard(void)
{
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}

void enable_mouse(void)
{
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	return;
}
