INCLUDE OSI.INC

DSegStart
D_3088	dw 1	;random seed 1
D_308A	dw 0	;random seed 2
DSegEnd

CSegStart	SEG_33C5_TEXT
	
C_33C5_000C	PROC FAR
	MOV	CX,D_3088
	MOV	BX,CX
	MOV	AX,D_308A
	ADD	CX,CX
	RCL	AX,1
	ADD	CX,BX
	ADC	AX,D_308A
	ADD	AX,BX
	MOV	D_3088,CX
	MOV	D_308A,AX
	RET
C_33C5_000C	ENDP

PUBLIC OSI_rand
OSI_rand	PROC FAR
	PUSH	BP
	MOV	BP,SP
	;--
	CALL	C_33C5_000C
	;--
	MOV	BX,[BP+06]
	MOV	CX,[BP+08]
	CMP	BX,CX
	JNB	C_003D
	XCHG	BX,CX
C_003D:
	XOR	DX,DX
	SUB	BX,CX
	INC	BX
	JZ	C_0046
	DIV	BX
C_0046:
	ADD	DX,CX
	MOV	AX,DX
	POP	BP
	RET	4
OSI_rand	ENDP

PUBLIC OSI_randomize	;33C5:004E
OSI_randomize	PROC FAR
	PUSH	BP
	MOV	BP,SP
	;--
	MOV	AX,[BP+06]
	OR	AX,1
	MOV	D_3088,AX
	;--
	POP	BP
	RET	2
OSI_randomize	ENDP

CSegEnd	SEG_33C5_TEXT


CSegStart	SEG_33CA_TEXT

;generate random seed?
ProcStart	C_33CA_000E
	MOV	AH,2Ch	;get time
	INT	21h
	XOR	AX,AX
	SHL	CH,1	;ch*=2
	SHL	CL,1
	SHL	CL,1	;cl*=4
	SHL	DH,1
	SHL	DH,1
	SHL	DH,1	;dh*=8
	ADD	AX,DX
	ADD	AX,CX
	XOR	AX,91EBh
	AND	AX,0FFFh
ProcEnd	C_33CA_000E

CSegEnd	SEG_33CA_TEXT

END
