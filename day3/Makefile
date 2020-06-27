ipl.bin: ipl.asm Makefile
		../tolset/z_tools/nask.exe ipl.asm ipl.bin ipl.lst

helloos.img : ipl.bin Makefile
	../tolset/z_tools/edimg.exe   imgin:../tolset/z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0   imgout:helloos.img

asm :
	../tolset/z_tools/make.exe -r ipl.bin

img:
	../tolset/z_tools/make.exe -r helloos.img...

run :
	../tolset/z_tools/make.exe img
	copy helloos.img ..\tolset\z_tools\qemu\fdimage0.bin
	../tolset/z_tools/make.exe -C ../tolset/z_tools/qemu