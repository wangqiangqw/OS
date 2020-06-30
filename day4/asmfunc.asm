;asmfunc
;TAB = 4

[FORMAT "WCOFF"]
[BITS 32]
[INSTRSET "i486p"]
[FILE "asmfunc.asm"]

    GLOBAL  _io_hlt
    GLOBAL  _write_mem8

[SECTION .text]

_io_hlt:  ; void io_io_hlt(void)
        HLT 
        RET 

_write_mem8:    ; void write_mem8(int addr, int data);
        MOV     ECX, [ESP+4]
        MOV     AL,[ESP+8]
        MOV     [ECX], AL
        RET