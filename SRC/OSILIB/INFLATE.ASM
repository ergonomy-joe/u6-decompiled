INCLUDE OSI.INC

DSegStart

masks	dw 1FFh,3FFh,7FFh,0FFFh
input_handle	dw 0
hash_seg	dw 0
output_offset	dw 0
bit_offset	dw 0
cur_code	dw 0
old_code	dw 0
in_code	dw 0
free_code	dw 0
stack_count	dw 0
max_code	dw 0
nbits	dw 0
fin_char	db 0
k	db 0
segment_0	dw 0	;a 3*1000h segment
lDestSz	dd 0
output_segment	dw 0
lpDest	dw 0,0
input_segment	dw 0	;a 1000h segment

DSegEnd

CSegStart	C_32FD_TEXT

ProcStart	_decompress	;32FD:0000
	MOV	AX,[BP+0Ch]
	MOV	DX,AX
	AND	AX,0Fh
	MOV	output_offset,AX
	MOV	lpDest+2,AX
	SHR	DX,1
	SHR	DX,1
	SHR	DX,1
	SHR	DX,1
	ADD	DX,[BP+0Ch+2]
	MOV	output_segment,DX
	MOV	lpDest,DX
	MOV	AX,[BP+10h]
	MOV	segment_0,AX
	ADD	AX,(3*1000h) / 10h
	MOV	input_segment,AX
	;-- --
	MOV	BX,[BP+06]	;opened file handle
	MOV	CX,[BP+08+2]
	MOV	DX,[BP+08]
	MOV	AX,4200h	;move file pointer?
	INT	21h
	;-- --
	MOV	AX,[BP+06]	;opened file handle
	MOV	BX,AX
	CALL	FAR PTR C_32FD_028C	;load&inflate?
	JB	C_0059
	;--
	MOV	DX,lpDest
	MOV	AX,lpDest+2
ProcEnd	_decompress

C_0059:	;--
	MOV	DX,0
	MOV	AX,0
	POP	DS
	POP	DI
	POP	SI
	POP	BP
	RET	;should be far, right? bug!

INCLUDE LZ.INC

;load&inflate?
C_32FD_028C	PROC FAR










	MOV	input_handle,BX
	;-- read Z-file header
	MOV	CX,4
	MOV	DX,OFFSET DGROUP:lDestSz
	MOV	AH,3Fh	;read file
	INT	21h
	JB	C_02BB
	;--
	MOV	BX,WORD PTR lDestSz
	MOV	DX,WORD PTR lDestSz+2
	MOV	CL,4
C_02A6:
	SHR	DX,1
	RCR	BX,1
	LOOP	C_02A6
	INC	BX











	CALL	decompress
	CLC
	MOV	AX,WORD PTR lDestSz
	OR	AX,AX
	JNZ	C_02BB
	DEC	AX
C_02BB:
	PUSHF
	PUSH	AX
	MOV	BX,input_handle
	JZ	C_02C3


C_02C3:
	MOV	BX,output_segment
	CMP	BX,-1
	JNZ	C_02D1
	INC	BX
	POP	AX
	POPF
C_02CF:	JMP	C_02CF
C_02D1:
	POP	AX
	POPF
	JNB	C_02DA
	;--
	MOV	BX,1
	STC
	RET
C_02DA:	;--
	MOV	BX,lpDest
	RET
C_32FD_028C	ENDP

CSegEnd	C_32FD_TEXT

END
