;========================================
EXTRN D_2FC6:WORD	;computer speed

EXTRN SoundFlag:WORD
;========================================
SOUND_ON	MACRO
	IN	AL,61h
	OR	AL,3
	OUT	61h,AL
ENDM

SOUND_OFF	MACRO
	IN	AL,61h
	AND	AL,NOT 3
	OUT	61h,AL
ENDM

OUT_42_AX	MACRO
	OUT	42h,AL
	MOV	AL,AH
	OUT	42h,AL
ENDM

SOUND_FREQ	MACRO freq
	MOV	CX,freq
	;-- port[0x42] := 0x1234de / cx
	MOV	DX,12h
	MOV	AX,34DEh	;Divide 1,193,182 hertz(clock frequency)
	DIV	CX
	OUT_42_AX
ENDM

;delayCntH and delayCntL must be declared
DELAY_CXAX	MACRO
	LOCAL loop_l,loop_h
	;-- active delay cx:ax --
	MOV	delayCntH,CX
loop_h:	MOV	delayCntL,AX
loop_l:	DEC	delayCntL
	JNZ	loop_l
	DEC	delayCntH
	JNZ	loop_h
ENDM
;========================================
