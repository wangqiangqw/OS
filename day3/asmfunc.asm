;asmfunc
;TAB = 4

[FORMAT "WCOFF"]
[BITS 32]

[FILE "asmfunc.asm"]

    GLOBAL  _io_hlt
[SECTION .text]

_io_hlt:  ; void io_io_hlt(void)
        HLT 
        RET 