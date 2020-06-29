TOOLPATH = ../tolset/z_tools/

INCPATH  = ../tolset/z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
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

bootpack.gas : bootpack.c Makefile
		$(CC1) -o bootpack.gas bootpack.c

bootpack.asm : bootpack.gas Makefile
		$(GAS2NASK) bootpack.gas bootpack.asm

bootpack.obj:	bootpack.asm Makefile
		$(NASK) bootpack.asm bootpack.obj bootpack.lst
	
bootpack.bim : 	bootpack.obj Makefile
		$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
			bootpack.obj

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
	-$(DEL) ipl10.bin
	-$(DEL) ipl10.lst
	-$(DEL) haribote.sys
	-$(DEL) haribote.lst

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img