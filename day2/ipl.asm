;hello-os
;TAB=4
    ORG 0x7c00

    JMP entry
    DB  0x90

    DB  "HELLOIPL"
    DW  512
    DB  1
    DW  1
    DB  2
    DW  224
    DW  2880
    DB  0XF0
    DW  9
    DW  18
    DW  2
    DD  0
    DD  2880
    DB  0,0,0X29
    DD  0XFFFFFFFF
    DB  "HELLO-OS   "
    DB  "FAT12   "
    RESB    18

entry:
        MOV     AX, 0
        MOV     SS, AX
        MOV     SP, 0X7C00
        MOV     DS,AX
        MOV     ES,AX

        MOV     SI, msg
putloop:
        MOV     AL,[SI]
        ADD     SI,1
        CMP     AL,0

        JE      fin
        MOV     AH,0X0E
        MOV     BX,15
        INT     0X10
        JMP     putloop

fin:
        HLT
        JMP     fin

msg:
        DB  0X0A,0X0A
        DB  "HELLO, Lucky, DON'T GIVE UP"
        DB  0x07, 0x07
        DB  0X0A
        DB  0

	RESB	0x7dfe-$
        DB	0x55, 0xaa
