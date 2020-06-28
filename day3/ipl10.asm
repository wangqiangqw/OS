;hello-os
;TAB=4
    ORG 0x7c00

    JMP entry
    DB  0x90

    DB  "HARIBOTE"                      ;启动区的名称可以是任意的字符串（8字节）
    DW  512                             ;每个扇区（sector）的大小（必须为512字节）
    DB  1                               ;簇（cluster）的大小（必须为1个扇区）
    DW  1                               ;FAT的起始位置（一般从第一个扇区开始）
    DB  2                               ;FAT的个数（必须为2）
    DW  224                             ;根目录的大小（一般设成224项）
    DW  2880                            ;该磁盘的大小（必须是2880扇区）
    DB  0XF0                            ;磁盘的种类（必须是0XF0）
    DW  9                               ;FAT的长度（必须是9扇区）
    DW  18                              ;1个磁道（track）有几个扇区（必须是18）
    DW  2                               ;磁头数（必须是2)
    DD  0                               ;不适用分区，必须是0
    DD  2880                            ;重写一次磁盘大小
    DB  0,0,0X29                        ;意义不明固定
    DD  0XFFFFFFFF                      ;可能是卷标号
    DB  "HELLO-OS   "                   ;磁盘的名称（11字节）
    DB  "FAT12   "                      ;磁盘格式名称（8字节）
    RESB    18                           ;

;-----------------------START -- 清屏------------------
        MOV     AH, 15                 ;显示一个文字
        INT     0X10                    ;调用显卡BIOS
        MOV     AH, 0                 ;显示一个文字
        INT     0X10                    ;调用显卡BIOS

;-----------------------END -- 清屏------------------


entry:
        MOV     AX, 0
        MOV     SS, AX
        MOV     SP, 0X7C00
        MOV     DS,AX
        MOV     ES,AX

        MOV     AX, 0x0820
        MOV     ES,AX
        MOV     CH,0                    ;柱面0
        MOV     DH,0                    ;磁头0
        MOV     CL,2                    ;扇区2
readloop:
        MOV     SI,0                    ;记录失败次数的寄存器
retry:
        MOV     AH,0X02                 ;读盘
        MOV     AL,1                    ;1个扇区
        MOV     BX,0                    ;
        MOV     DL,0X00                 ;A驱动器
        INT     0X13                    ;调用磁盘BIOS
;-----------------------START -- 显示读取的扇区，磁头和柱面------------------
        MOV     AL,' '
        MOV     AH,0X0E                 ;显示一个文字
        MOV     BX,15                   ;指定字符颜色
        INT     0X10                    ;调用显卡BIOS
        MOV     AL,'0'
        ADD     AL, CL
        MOV     AH,0X0E                 ;显示一个文字
        MOV     BX,15                   ;指定字符颜色
        INT     0X10                    ;调用显卡BIOS
        MOV     AL,'0'
        ADD     AL, DH
        MOV     AH,0X0E                 ;显示一个文字
        MOV     BX,15                   ;指定字符颜色
        INT     0X10                    ;调用显卡BIOS
        MOV     AL,'0'
        ADD     AL, CH
        MOV     AH,0X0E                 ;显示一个文字
        MOV     BX,15                   ;指定字符颜色
        INT     0X10                    ;调用显卡BIOS
;-----------------------END -- 显示读取的扇区，磁头和柱面------------------
        JNC     next
        ADD     SI,1
        CMP     SI,5
        JAE     error
        MOV     AH,0X00
        MOV     DL,0X00
        INT     0X13
        JMP     retry
next:
;-------------读到18 扇区------------------------------------
        MOV     AX,ES
        ADD     AX,0X0020
        MOV     ES,AX
        ADD     CL,1                    ;扇区加1
        CMP     CL,18                   ;只到扇区18
        JBE     readloop
;------------读 磁头2 -----------------------------------------
        MOV     AX,ES
        ADD     AX,0X0020
        MOV     ES,AX
        MOV     CL, 1                   ;重新回到扇区1
        ADD     DH, 1                   ;磁头加1

        CMP     DH, 1
        JBE     readloop
;------------读到柱面 10 ---------------------------------------
        MOV     AX,ES
        ADD     AX,0X0020
        MOV     ES,AX
        MOV     CL, 1                   ;重新回到扇区1
        MOV     DH, 0                   ;磁头加1
        ADD     CH, 1
        CMP     CH, 9
        JBE     readloop


        
 success:                                       ;successful
        MOV     SI, succmsg
        JMP     putloop

fin:
 	MOV	[0x0ff0],CH		; IPL���ǂ��܂œǂ񂾂̂�������
	JMP	0xc200
        HLT
        JMP     fin

error:
        MOV     SI, errormsg
putloop:
        MOV     AL,[SI]
        ADD     SI,1
        CMP     AL,0

        JE      fin
        MOV     AH,0X0E                 ;显示一个文字
        MOV     BX,15                   ;指定字符颜色
        INT     0X10                    ;调用显卡BIOS
        JMP     putloop



errormsg:
        DB  0X0A,0X0D
        DB  "LOAD ERROR -- HELLO, Lucky, DON'T GIVE UP"
        DB  0x07, 0x07
        DB  0X0A
        DB  0

succmsg:
        DB  0X0A,0X0D
        DB  "LOAD Success -- HELLO, Lucky, DON'T GIVE UP"
        DB  0x07, 0x07
        DB  0X0A
        DB  0

	RESB	0x7dfe-$
        DB	0x55, 0xaa