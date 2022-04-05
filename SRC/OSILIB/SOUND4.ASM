INCLUDE OSI.INC
INCLUDE SOUND.INC

DSegStart
D_3092	dw 0
delayCntH	dw 0	;D_3094
delayCntL	dw 0	;D_3096
D_3098	dw 7664h	;noise seed
DSegEnd

CSegStart	SEG_33DB_TEXT

ProcStart	OSI_playNoise	;C_33DB_0004
	MOV	D_3092,0
	;--
	CMP	BYTE PTR SoundFlag,0
	JZ	C_001D_
	SOUND_ON
C_001D_:
	;-- AX := pseudo-random --
	MOV	AX,D_3098
	ADD	AX,9248h
	MOV	CL,3
	ROR	AX,CL
	XOR	AX,9248h
	ADD	AX,11h
	MOV	D_3098,AX
	;-- AX := AX % (bp06 - 100 + 1) + 100
	MOV	BX,100
	MOV	CX,[BP+06]
	SUB	CX,BX
	INC	CX
	XOR	DX,DX
	DIV	CX
	ADD	DX,BX
	MOV	AX,DX
	;--
	SOUND_FREQ	AX
	;-- tempo --
	MOV	AX,D_2FC6
	MOV	CL,4
	SHR	AX,CL
	MOV	CX,[BP+0Ah]
	ADD	D_3092,CX
	DELAY_CXAX

	MOV	AX,D_3092
	CMP	AX,[BP+08]
	JL	C_001D_
	;--
	SOUND_OFF
ProcEndN	OSI_playNoise,6

CSegEnd	SEG_33DB_TEXT

END
