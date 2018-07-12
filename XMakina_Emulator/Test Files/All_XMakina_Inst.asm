;
; Assemble all instructions
; Check codes
;
	LD	R1,R2
	LD.B	R2,R3
	ST	R1,R2
	ST.B	R2,R3
;
	LD	R1+,R2
	LD	R1-,R2
	LD	+R1,R2
	LD	-R1,R2
;
	LD.B	R1+,R2
	LD.B	R1-,R2
	LD.B	+R1,R2
	LD.B	-R1,R2
;
	ST	R1,R2+
	ST	R1,R2-
	ST	R1,+R2
	ST	R1,-R2
;
	ST.B	R1,R2+
	ST.B	R1,R2-
	ST.B	R1,+R2
	ST.B	R1,-R2
;
;	LDR	R1,#-500,R2	; Invalid relative value
	LDR	R2,#$5,R3
	LDR.B	R2,#$5,R3
	STR	R2,R3,#-15
	STR.B	R2,R3,#$5
;
	MOVL	#$1a,R3
	MOVLZ	#'A',R3
	MOVH	#$1234,R3
; Branching
	ORG	#$100
LOOP
	BL	LOOP
	BNE	LOOP
	BNZ	LOOP
	BEQ	LOOP
	BZ	LOOP
	BNC	LOOP
	BLO	LOOP
	BC	LOOP
	BHS	LOOP
	BN	LOOP
	BGE	LOOP
	BLT	LOOP
	BAL	LOOP
; Register-register
	ADD	R1,R2
	ADD.B	R2,R3
	ADDC	R2,R3	; ADD with Carry
	ADDC.B	R2,R3
	SUB	R2,R3
	SUB.B	R2,R3
	SUBC	R2,R3
	SUBC.B	R2,R3
; Note - the ADDC mnemonic has changed
; This might require changes to the pre-assembler's 
	DADD	R1,R2	; BCD arithmetic
	DADD.B	R2,R3	; BCD arithmetic
;
	CMP	R2,R3
	CMP.B	R2,R3
	XOR	R2,R3
	XOR.B	R2,R3
	AND	R2,R3
	AND.B	R2,R3
	BIT	R2,R3
	BIT.B	R2,R3
	BIC	R2,R3
	BIC.B	R2,R3
	BIS	R2,R3
	BIS.B	R2,R3
	MOV	R2,R3
	MOV.B	R2,R3
; Constant-register
	ADD	#$1,R2
	ADD.B	#$1,R3
	ADDC	#$1,R3
	ADDC.B	#$1,R3
	SUB	#$1,R3
	SUB.B	#$1,R3
	SUBC	#$1,R3
	SUBC.B	#$1,R3
;
	DADD	#$1,R2	; BCD
	DADD.B	#$1,R3	; BCD
;
	CMP	#$1,R3
	CMP.B	#$1,R3
	XOR	#$1,R3
	XOR.B	#$1,R3
	AND	#$1,R3
	AND.B	#$1,R3
	BIT	#$1,R3
	BIT.B	#$1,R3
	BIC	#$1,R3
	BIC.B	#$1,R3
	BIS	#$1,R3
	BIS.B	#$1,R3
	MOV	#$1,R3
	MOV.B	#$1,R3
;
	SWAP	R1,R2
;
	SRA	R3
	SRA.B	R2
	RRC	R3
	RRC.B	R2
	SWPB	R2
	SXT	R2
;
	END	LOOP
