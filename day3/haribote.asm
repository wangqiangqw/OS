;haribote-os
; TAB=4

;BOOT_INFO

CYLS    EQU     0X0FF0
LEDS    EQU     0X0FF1
VMODE   EQU     0X0FF2
SCRNX   EQU     0X0FF4
SCRNY   EQU     0X0FF6
VRAM    EQU     0X0FF8


    ORG     0XC200

    MOV     AL,0X13
    MOV     AH,0X00
    INT     0X10
    MOV     BYTE    [VMODE], 8;
    MOV     WORD    [SCRNX], 320
    MOV     WORD    [SCRNY], 200
    MOV     DWORD   [VRAM], 0X000A000

    MOV     AH, 0X02
    INT     0X16
    MOV     [LEDS],AL

fin:
    HLT
    JMP     fin