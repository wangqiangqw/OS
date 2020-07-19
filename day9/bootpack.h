#ifndef BOOTPACK_H
#define BOOTPACK_H

struct BOOTINFO
{
	char cyls;
	char leds;
	char vmode; 
	char reserve;
	short scrnx,scrny;
	char *vram;
};
#define ADR_BOOTINFO	0x00000ff0
#define PORT_KEYDAT	0X0060

struct KEYBUF
{
	unsigned char data, flag;
} ;

/* asmfunc.asm */
void io_hlt(void);
void write_mem8(int addr, int data);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
void io_out8(int port, int data);
int io_in8(int port);
int  io_load_eflags(void);
void io_store_eflags(int eflags);


void  load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void asm_inthandler21();
void asm_inthandler2c();
int load_cr0(void);
void store_cr0(int cr0);
unsigned int memtest_sub(unsigned int start, unsigned int end);
/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(unsigned char *vram, int xsize, int ysize);
void putfont8(char *vram, int xsize,int x,int y, char c, char *font);
void putfont8_asc(char *vram, int xsize,int x,int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize);
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

/* dsctbl.c */

struct SEGMENT_DESCRIPTOR
{
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};

struct GATE_DESCRIPTOR
{
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e

/* int.c */

void init_pic(void);	
void inthandler21(int *esp);
void inthandler2c(int *esp);




#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

/* fifo8.c */

struct FIFO8
{
	unsigned char *buf;
	int p, q, size, free, flags;
};

#define FLAGS_OVERRUN	0X0001

void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf);
int fifo8_put(struct FIFO8 *fifo, unsigned char data);
int fifo8_get(struct FIFO8 *fifo);
int fifo8_status(struct FIFO8 *fifo);

/*bootpack.c*/
#define PORT_KEYDAT				0X0060
#define PORT_KEYSTA				0X0064
#define PORT_KEYCMD				0X0064
#define KEYSTA_SEND_NOTREADY	0X02
#define	KEYCMD_WRITE_MODE		0X60
#define KBC_MODE				0X47
#define KEYCMD_SENDTO_MOUSE		0XD4
#define MOUSECMD_ENABLE			0XF4
struct MOUSE_DEC
{
	unsigned char buf[3], phase;
	int x, y, btn;
};

void wait_KBC_sendready(void);
void init_keyborad(void);
void enable_mouse(struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

#endif
