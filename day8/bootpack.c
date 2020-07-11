#include <stdio.h>
#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

void HariMain(void)
{

	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	struct MOUSE_DEC mdec;

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
	
	
	enable_mouse(&mdec);

	//putfont8_asc(binfo->vram, binfo->scrnx, 16, 32, COL8_FFFFFF, "enable mouse");


	//mx=0;
	//my=16;
	//int m,n = 0;
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
				if(mouse_decode(&mdec,i)!=0)
				{
					sprintf(s,"[lcr %4d %4d] %d %d",mdec.x,mdec.y, (signed char)mdec.buf[1],(signed char)mdec.buf[2]);
					if((mdec.btn & 0x01)!=0)
						s[1]='L';
					if((mdec.btn &0x02)!=0)
						s[3]='R';
					if((mdec.btn &0x04)!=0)
						s[2]='C';
					boxfill8(binfo->vram,binfo->scrnx,COL8_008484,32/*mx+m*16*/,16/*my+n*16*/,32+23*8-1,31);
					putfont8_asc(binfo->vram,binfo->scrnx,32/*mx+m*16*/,16/*my+n*16*/,COL8_FFFFFF,s);

					boxfill8(binfo->vram,binfo->scrnx,COL8_008484,mx,my, mx+15,my+15);

					mx+=mdec.x;
					my +=mdec.y;
					if(mx<0) mx = 0;
					if (my<0) my = 0;
					if(mx>(binfo->scrnx-16)) mx=binfo->scrnx-16;
					if(my>(binfo->scrny-16)) my=binfo->scrny-16;
					sprintf(s, "(%3d, %3d)",mx,my);
					boxfill8(binfo->vram,binfo->scrnx,COL8_008484,0,0,79,15);
					putfont8_asc(binfo->vram,binfo->scrnx,0,0,COL8_FFFFFF,s);
					putblock8_8(binfo->vram,binfo->scrnx,16,16,mx,my,mcursor,16);
				}
				/* code */
			}
			
			/*if(m==16)
			{
				m=0;
				n++;
			}
			m++;*/
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

void enable_mouse(struct MOUSE_DEC *mdec)
{
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	mdec->phase=0; 
	return;
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
	switch (mdec->phase)
	{
	case 0:
		if(dat ==0xfa)
		{
			mdec->phase = 1;
		}
		break;
	case 1:
		if((dat&0xc8)==0x08)
		{
			mdec->buf[0]=dat;
			mdec->phase=2;

		}
		break;
	case 2:
		mdec->buf[1]= dat;
		mdec->phase=3;
		break;
	case 3:
		mdec->buf[2]=dat;
		mdec->phase=1;
		mdec->btn =mdec->buf[0]&0x07;
		mdec->x=(signed char)mdec->buf[1];
		mdec->y=(signed char)mdec->buf[2];
		//if((mdec->buf[0]&0x10)!=0)
		//	mdec->x!=0xffffff00;
		//if((mdec->buf[0]&0x20)!=0)
		//	mdec->y!=0xffffff00;
		mdec->y=-mdec->y;
		return 1;
		break;
	default:
		break;
	}
	return 0;
}