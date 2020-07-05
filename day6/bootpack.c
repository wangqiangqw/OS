#include <stdio.h>
#include "bootpack.h"



void HariMain(void)
{

	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;

	char s[40], mcursor[256];
	int mx, my;

	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC‚Ì‰Šú‰»‚ªI‚í‚Á‚½‚Ì‚ÅCPU‚ÌŠ„‚èž‚Ý‹ÖŽ~‚ð‰ðœ */
	init_palette();
	init_screen(binfo->vram,binfo->scrnx,binfo->scrny);
	mx = (binfo->scrnx - 16) / 2; /* ��ʒ����ɂȂ�悤�ɍ��W�v�Z */
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfont8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
	
	io_out8(PIC0_IMR, 0xf9); /* PIC1�ƃL�[�{�[�h������(11111001) */
	io_out8(PIC1_IMR, 0xef); /* �}�E�X������(11101111) */


    for(;;)
    {
        io_hlt();
    }   
}

