#include <stdio.h>
#include "bootpack.h"



void HariMain(void)
{

	struct BOOTINFO *binfo = (struct BOOTINFO *) 0xff0;

	char s[40], mcursor[256];
	init_palette();
	init_screen(binfo->vram,binfo->scrnx,binfo->scrny);
	putfont8_asc(binfo->vram,binfo->scrnx,8,8,COL8_000000,"ABC 12c");
	putfont8_asc(binfo->vram,binfo->scrnx,31,31,COL8_000000,"Haribote OS.");
	putfont8_asc(binfo->vram,binfo->scrnx,30,30,COL8_FFFFFF,"Haribote OS.");
	s[0]='\0';
	sprintf(s,"scrnx = %d",binfo->scrnx);
	putfont8_asc(binfo->vram,binfo->scrnx,16,64,COL8_000000,s);
	int mx = (binfo->scrnx - 16) / 2; /* ��ʒ����ɂȂ�悤�ɍ��W�v�Z */
	int my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	
	io_out8(PIC0_IMR, 0xf9); /* PIC1�ƃL�[�{�[�h������(11111001) */
	io_out8(PIC1_IMR, 0xef); /* �}�E�X������(11101111) */


    for(;;)
    {
        io_hlt();
    }   
}

