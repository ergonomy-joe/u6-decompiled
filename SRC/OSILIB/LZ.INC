;----------------------------------------
;inflate module
;very close to code from Tom Pfau
;----------------------------------------

CLEAR      EQU 256
EOF        EQU 257
FIRST_FREE EQU 258

read_code	MACRO
	LOCAL rd0,rd1,rd2
	PUSH	BP

	MOV	AX,bit_offset	;Get bit offset
	ADD	AX,nbits	;Adjust by code size
	XCHG	AX,bit_offset	;Swap
	MOV	CX,8
	XOR	DX,DX
	DIV	CX
	CMP	AX,1021	;Approaching end of buffer?
	JL	rd0	;no

	PUSH	DX	;Save offset in byte
	ADD	DX,nbits	;Calculrate new bit offset
	MOV	bit_offset,DX
	MOV	CX,1024	;1k buffer
	MOV	BP,AX	;save byte offset
	SUB	CX,AX	;Calculates bytes left
	;-- --
	PUSH	DS
	MOV	BX,input_handle
	MOV	DS,input_segment
	PUSH	DS
	POP	ES
	XOR	SI,SI
	MOV	DI,SI
	ADD	SI,AX
	REPZ	MOVSB
	MOV	CX,BP
	MOV	DX,DI
	MOV	AH,3Fh	;read file
	INT	21h
	POP	DS
	;-- --
	XOR	AX,AX
	POP	DX
rd0:
	PUSH	DS
	MOV	DS,input_segment
	MOV	SI,AX
	LODSW	;Get word
	MOV	BX,AX	;Save in AX
	LODSB	;Next byte
	MOV	CX,DX	;Offset in byte
	JCXZ	rd2	;If zero skip shifts
rd1:	SHR	AL,1	;Put code in low (code size) bits of BX
	RCR	BX,1
	LOOP	rd1
rd2:
	POP	DS
	MOV	AX,BX	;put code in ax
	MOV	BX,nbits	;mask off unwanted bits
	SUB	BX,9
	SHL	BX,1
	AND	AX,masks[BX]

	POP	BP
ENDM

init_tab	MACRO
	MOV	nbits,9	;Initialize variables
	MOV	max_code,512
	MOV	free_code,FIRST_FREE
ENDM

write_char	MACRO
	LOCAL wc1

	PUSH	ES
	MOV	ES,output_segment
	MOV	DI,output_offset
	STOSB	;Store char
	POP	ES
	INC	output_offset	;Increment number of chars in buffer
	CMP	DI,10h
	JNZ	wc1
	INC	output_segment
	MOV	output_offset,0
wc1:
ENDM

index MACRO
	;bx = bx *3 (3 bytes entries)
	PUSH	AX
	MOV	AX,BX
	SHL	BX,1
	ADD	BX,AX
	POP	AX
ENDM

add_code	MACRO
	MOV	BX,free_code	;Get new code
	index	;convert to address
	PUSH	ES
	MOV	ES,hash_seg
	MOV	AL,k	;get suffix char
	MOV	ES:2[BX],AL	;save it
	MOV	AX,old_code	;get prefix code
	MOV	ES:[BX],AX	;save it
	POP	ES
	INC	free_code	;set next code
ENDM

decompress	PROC FAR	;C_32FD_0064
	MOV	nbits,9
if 0	;commented in GAME.EXE
	MOV	output_offset,0
endif
	MOV	bit_offset,0
	MOV	free_code,FIRST_FREE
	MOV	max_code,512

	MOV	AX,segment_0
	MOV	hash_seg,AX

	MOV	BX,input_handle
	PUSH	DS
	MOV	DS,input_segment
	XOR	DX,DX
	MOV	CX,1024
	MOV	AH,3Fh	;read file
	INT	21h
	POP	DS

L1:	read_code	;Get a code
	CMP	AX,EOF	;End of file?
	JNE	L2	;no

	MOV	CX,1800h
	XOR	AX,AX
	MOV	ES,hash_seg
	MOV	DI,AX
	REPZ	STOSW

	RET	;done

L2:	CMP	AX,CLEAR	;Clear code?
	JE	C_011D
	JMP	L7	;no
C_011D:

	init_tab	;Initialize table
	read_code	;Read next code
	MOV	cur_code,AX	;Initialize variables
	MOV	old_code,AX
	MOV	k,AL
	MOV	fin_char,AL
	write_char	;Write character
	JMP	L1	;Get next code

L7:	MOV	cur_code,AX	;Save new code
	MOV	in_code,AX
	MOV	ES,hash_seg
	CMP	AX,free_code	;Code in table? (k<w>k<w>k)
	JL	L11	;yes
	MOV	AX,old_code	;get previous code
	MOV	cur_code,AX	;make current
	MOV	AL,fin_char	;get old last char
	PUSH	AX	;push it
	INC	stack_count
L11:	CMP	cur_code,255	;Code or character?
	JLE	L15	;Char

	MOV	BX,cur_code	;Convert code to address
	index
	MOV	AL,ES:2[BX]	;Get suffix char
	PUSH	AX	;push it
	INC	stack_count
	MOV	AX,ES:[BX]	;Get prefix code
	MOV	cur_code,AX	;Save it
	JMP	L11	;Translate again

L15:
	PUSH	DS	;restore seg reg
	POP	ES
	MOV	AX,cur_code	;Get code
	MOV	fin_char,AL	;Save as final, k
	MOV	k,AL
	PUSH	AX	;Push it
	INC	stack_count
	MOV	CX,stack_count	;Pop stack
	JCXZ	L18	;If anything there
L17:	POP	AX
	write_char
	LOOP	L17
L18:	MOV	stack_count,CX	;Clear count on stack
	add_code	;Add new code to table
	MOV	AX,in_code	;Save input code
	MOV	old_code,AX
	MOV	BX,free_code	;Hit table limit?
	CMP	BX,max_code
	JL	L23	;Less means no
	CMP	nbits,12	;Still within twelve bits?
	JE	L23	;no (next code should be clear)
	INC	nbits	;Increase code size
	SHL	max_code,1	;Double max code
L23:	JMP	L1	;Get next code
decompress	ENDP
