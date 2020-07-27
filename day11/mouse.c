#include "bootpack.h"
struct FIFO8 mousefifo;
void inthandler2c(int *esp)
{
	/*struct BOOTINFO *binfo  = (struct BOOTINFO *) ADR_BOOTINFO;
	boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32*8-1,15);
	static i=1;
	extern char hankaku[4096];
	putfont8_asc(binfo->vram,binfo->scrnx,0,0,COL8_FFFFFF,"INT 2c (IRQ-12) : PS/2 MOUSE");
	putfont8(binfo->vram,binfo->scrnx,8*i,8*i,COL8_FFFFFF,hankaku +i*16);
	for(;;)
		io_hlt();*/
	unsigned char data;
	io_out8(PIC1_OCW2,0x64);
	io_out8(PIC0_OCW2,0x62);
	data=io_in8(PORT_KEYDAT);
	fifo8_put(&mousefifo,data);
	return ;
}
#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4
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
		mdec->x=/*(signed char)*/mdec->buf[1];
		mdec->y=/*(signed char)*/mdec->buf[2];
		if((mdec->buf[0]&0x10)!=0){
			mdec->x|=0xffffff00;
		}
		if((mdec->buf[0]&0x20)!=0){
			mdec->y|=0xffffff00;
		}
		mdec->y=-mdec->y;
		return 1;
		break;
	default:
		break;
	}
	return 0;
}


