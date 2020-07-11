/* ���荞�݊֌W */

#include "bootpack.h"
struct FIFO8 keyfifo;
struct FIFO8 mousefifo;

void init_pic(void)
/* PIC�̏����� */
{
	io_out8(PIC0_IMR,  0xff  ); /* �S�Ă̊��荞�݂��󂯕t���Ȃ� */
	io_out8(PIC1_IMR,  0xff  ); /* �S�Ă̊��荞�݂��󂯕t���Ȃ� */

	io_out8(PIC0_ICW1, 0x11  ); /* �G�b�W�g���K���[�h */
	io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7�́AINT20-27�Ŏ󂯂� */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1��IRQ2�ɂĐڑ� */
	io_out8(PIC0_ICW4, 0x01  ); /* �m���o�b�t�@���[�h */

	io_out8(PIC1_ICW1, 0x11  ); /* �G�b�W�g���K���[�h */
	io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15�́AINT28-2f�Ŏ󂯂� */
	io_out8(PIC1_ICW3, 2     ); /* PIC1��IRQ2�ɂĐڑ� */
	io_out8(PIC1_ICW4, 0x01  ); /* �m���o�b�t�@���[�h */

	io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1�ȊO�͑S�ċ֎~ */
	io_out8(PIC1_IMR,  0xff  ); /* 11111111 �S�Ă̊��荞�݂��󂯕t���Ȃ� */

	return;
}
void inthandler21(int *esp)
{

	unsigned char	data;
	io_out8(PIC0_OCW2,0X61);
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&keyfifo,data);
	return;
}

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
