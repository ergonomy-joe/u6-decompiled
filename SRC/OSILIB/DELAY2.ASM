INCLUDE OSI.INC
INCLUDE SOUND.INC

DSegStart
delayCntL	dw 0	;D_3062
delayCntH	dw 0	;D_3064
D_3066	dw 0,0,1,1,2,2,2,3,3,3,3,3,3,3,4,4,4
DSegEnd

CSegStart	SEG_33C2

ProcStart	OSI_sDelay	;C_33C2_000A
	MOV	AX,D_2FC6
	MOV	BX,[BP+08]
	MOV	CX,D_3066[BX]
	JCXZ	C_001E
	SHR	AX,CL
C_001E:	MOV	CX,[BP+06]
	DELAY_CXAX
ProcEndN	OSI_sDelay,4

CSegEnd	SEG_33C2

END
