INCLUDE OSI.INC
INCLUDE SOUND.INC

EXTRN OSI_sDelay:FAR
EXTRN OSI_nosound:FAR
EXTRN OSI_sound:FAR

;should be C code (see seg_33E3.c)!!!
CSegStart	SEG_33E3_TEXT

PUBLIC OSI_playNote	;C_33E3_0008
OSI_playNote	PROC FAR
	PUSH	BP
	MOV	BP,SP
	;--
	CMP	BYTE PTR SoundFlag,0
	JZ	C_001A
	;--
	PUSH	[BP+08]
	CALL	OSI_sound
C_001A:	;--
	MOV	AX,1
	PUSH	AX
	PUSH	[BP+06]
	CALL	OSI_sDelay
	;--
	CALL	OSI_nosound
	;--
	POP	BP
	RET	4
OSI_playNote	ENDP

EVEN	;__33E3_002F	db 90h	;(NOP)padding

CSegEnd	SEG_33E3_TEXT

END
