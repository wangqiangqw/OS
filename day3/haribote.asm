;haribote-os
; TAB=4

    ORG     0XC200

    MOV     AL,0X13
    MOV     AH,0X00
    INT     0X10

fin:
    HLT
    JMP     fin