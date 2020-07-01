void io_hlt(void);
void write_mem8(int addr, int data);
void io_cli(void);
void io_out8(int port, int data);
int  io_load_eflags(void);
void io_store_eflags(int eflags);


void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(unsigned char *vram, int xsize, int ysize);
void putfont8(char *vram, int xsize,int x,int y, char c, char *font);
void putfont8_asc(char *vram, int xsize,int x,int y, char c, unsigned char *s);
struct BOOTINFO
{
	char cyls, leds, vmode, reserve;
	short scrnx,scrny;
	char *vram;
};

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

void HariMain(void)
{

	struct BOOTINFO *binfo = (struct BOOTINFO *) 0xff0;
	extern char hankaku[4096];
	
	init_palette();
	init_screen(binfo->vram,binfo->scrnx,binfo->scrny);
	putfont8_asc(binfo->vram,binfo->scrnx,8,8,COL8_000000,"ABC 12c");
	putfont8_asc(binfo->vram,binfo->scrnx,31,31,COL8_000000,"Haribote OS.");
	putfont8_asc(binfo->vram,binfo->scrnx,30,30,COL8_FFFFFF,"Haribote OS.");
/*    for(i=0x0000;i<=0xffff;i++)
    {
        
        p[i]=i&0xf;
        //write_mem8(i,i&15);
    }*/

    for(;;)
    {
        io_hlt();
    }   
}

void init_screen(unsigned char *vram, int xsize, int ysize)
{


	boxfill8(vram, xsize, COL8_008484,  0,         0,          xsize -  1, ysize - 29);
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 28, xsize -  1, ysize - 28);
	boxfill8(vram, xsize, COL8_FFFFFF,  0,         ysize - 27, xsize -  1, ysize - 27);
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 26, xsize -  1, ysize -  1);

	boxfill8(vram, xsize, COL8_FFFFFF,  3,         ysize - 24, 59,         ysize - 24);
	boxfill8(vram, xsize, COL8_FFFFFF,  2,         ysize - 24,  2,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484,  3,         ysize -  4, 59,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484, 59,         ysize - 23, 59,         ysize -  5);
	boxfill8(vram, xsize, COL8_000000,  2,         ysize -  3, 59,         ysize -  3);
	boxfill8(vram, xsize, COL8_000000, 60,         ysize - 24, 60,         ysize -  3);

	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize -  3, ysize - 24, xsize -  3, ysize -  3);/**/

}
void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:�� */
		0xff, 0x00, 0x00,	/*  1:���邢�� */
		0x00, 0xff, 0x00,	/*  2:���邢�� */
		0xff, 0xff, 0x00,	/*  3:���邢���F */
		0x00, 0x00, 0xff,	/*  4:���邢�� */
		0xff, 0x00, 0xff,	/*  5:���邢�� */
		0x00, 0xff, 0xff,	/*  6:���邢���F */
		0xff, 0xff, 0xff,	/*  7:�� */
		0xc6, 0xc6, 0xc6,	/*  8:���邢�D�F */
		0x84, 0x00, 0x00,	/*  9:�Â��� */
		0x00, 0x84, 0x00,	/* 10:�Â��� */
		0x84, 0x84, 0x00,	/* 11:�Â����F */
		0x00, 0x00, 0x84,	/* 12:�Â��� */
		0x84, 0x00, 0x84,	/* 13:�Â��� */
		0x00, 0x84, 0x84,	/* 14:�Â����F */
		0x84, 0x84, 0x84	/* 15:�Â��D�F */
	};
	set_palette(0, 15, table_rgb);
	return;

	/* static char ���߂́A�f�[�^�ɂ����g���Ȃ�����DB���ߑ��� */
}

void set_palette(int start, int end, unsigned char *rgb)
{
    int i, eflags;
    eflags = io_load_eflags();
    io_cli();
    io_out8(0x3c8,start);
    for(i = start; i<= end; i++)
    {
        io_out8(0x3c9,rgb[0]/4);
        io_out8(0x3c9,rgb[1]/4);
        io_out8(0x3c9,rgb[2]/4);
        rgb+=3;
        
    }

    io_store_eflags(eflags);
    return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
    int x, y;
    for (y=y0;y<=y1;y++)
    {
        for(x=x0;x<=x1;x++)
        {
            vram[y*xsize+x]=c;
        }
    }
}

void putfont8(char *vram, int xsize,int x,int y, char c, char *font)
{
	int i;
	char d;
	char *p;
	for(i=0;
	i<16;i++)
	{
		p=vram+(y+i)*xsize+x;
		d=font[i];
		if((d&0x80)!=0){p[0]=c;}
		if((d&0x40)!=0){p[1]=c;}
		if((d&0x20)!=0){p[2]=c;}
		if((d&0x10)!=0){p[3]=c;}
		if((d&0x08)!=0){p[4]=c;}
		if((d&0x04)!=0){p[5]=c;}
		if((d&0x02)!=0){p[6]=c;}
		if((d&0x01)!=0){p[7]=c;}
	}
}

void putfont8_asc(char *vram, int xsize,int x,int y, char c, unsigned char *s)
{
	extern char hankaku[4096];
	for(;*s!=0x00;s++)
	{
		putfont8(vram,xsize,x,y,c,hankaku +*s*16);
		x+=8;
	}

	return;
}