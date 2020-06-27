ipl.bin: ipl.asm Makefile
		../tolset/z_tools/nask.exe ipl.asm ipl.bin ipl.lst

helloos.img : ipl.bin Makefile
	../tolset/z_tools/edimg.exe   imgin:../tolset/z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0   imgout:helloos.img