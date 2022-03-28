INCLUDE OSI.INC

CSegStart	SEG_31FA

PUBLIC C_31FA_000C
C_31FA_000C	PROC FAR
	PUSH	BP
	MOV	BP,SP
	PUSH	DS
	PUSH	SI
	PUSH	DI
	;--
	TEST	WORD PTR [BP+06],0FFFFh
	JZ	C_0029
	;-- flag on --
	XOR	AX,AX
C_001B:	;--
	PUSH	AX
	MOV	AH,1	;Get Keyboard Status
	INT	16h
	POP	AX
	JZ	C_0036
	MOV	AH,0	;Wait for Keypress and Read Character
	INT	16h
	;--
	JMP	C_001B
C_0029:	;-- flag off --
	MOV	AH,1	;Get Keyboard Status
	INT	16h
	MOV	AX,0
	JZ	C_0036
	MOV	AH,0	;Wait for Keypress and Read Character
	INT	16h
C_0036:	;--
	OR	AL,AL
	JNZ	C_0044
	OR	AH,AH
	JZ	C_0067
	;--
	MOV	AL,AH
	MOV	AH,1
	JMP	SHORT C_0067
C_0044:	;--
	CMP	AH,47h
	JB	C_005A
	CMP	AH,51h
	JA	C_005A
	;-- 0x47~0x5a
	CMP	AH,4Ah	;numpad '-'
	JZ	C_0065
	CMP	AH,4Eh	;numpad '+'
	JZ	C_0065
	JMP	SHORT C_005F
C_005A:	;--
	CMP	AH,0Eh	;backspace
	JNZ	C_0065
C_005F:	;-- 0x0e [numeric keypad]0x47~0x5A (-)0x4a (-)0x4e
	MOV	AL,AH
	MOV	AH,1
	;--
	JMP	SHORT C_0067
C_0065:	;--
	XOR	AH,AH
C_0067:	;--
	POP	DI
	POP	SI
	POP	DS
	POP	BP
	RET	2
C_31FA_000C	ENDP

CSegEnd	SEG_31FA

END
