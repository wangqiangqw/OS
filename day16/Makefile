OBJS_BOOTPACK = bootpack.obj asmfunc.obj hankaku.obj graphic.obj dsctbl.obj \
		int.obj fifo.obj keyboard.obj mouse.obj memory.obj sheet.obj timer.obj \
		mtask.obj
TOOLPATH = ../tolset/z_tools/

INCPATH  = ../tolset/z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe

BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

ipl10.bin: ipl10.asm Makefile
		$(NASK) ipl10.asm ipl10.bin ipl10.lst


asmhead.bin: asmhead.asm Makefile
		$(NASK) asmhead.asm asmhead.bin asmhead.lst

asmfunc.obj: asmfunc.asm Makefile
		$(NASK) asmfunc.asm asmfunc.obj asmfunc.lst

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

%.gas 	: %.c Makefile
		$(CC1) -o $*.gas $*.c

%.asm : %.gas Makefile
		$(GAS2NASK) $*.gas $*.asm

%.obj:	%.asm Makefile
		$(NASK) $*.asm $*.obj $*.lst
	
bootpack.bim : 	$(OBJS_BOOTPACK) Makefile
		$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
			$(OBJS_BOOTPACK)

bootpack.hrb : bootpack.bim Makefile
		$(BIM2HRB) bootpack.bim bootpack.hrb 0

haribote.sys:  asmhead.bin bootpack.hrb Makefile
		copy /B asmhead.bin+bootpack.hrb haribote.sys

haribote.img : ipl10.bin haribote.sys Makefile
	$(EDIMG)   imgin:..\tolset\z_tools\fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0   \
		copy from:haribote.sys to:@: \
		imgout:haribote.img

asm :
	$(MAKE) ipl10.bin

img:
	$(MAKE) haribote.img

run :
	$(MAKE) img
	copy haribote.img ..\tolset\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../tolset/z_tools/qemu

clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) bootpack.nas
	-$(DEL) bootpack.map
	-$(DEL) bootpack.bim
	-$(DEL) bootpack.hrb
	-$(DEL) haribote.sys

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img