;
; Polling example 
; Polling three input devices (0, 1, and 2)
;
; Registers:
;
PC	equ	R7
PSW	equ	R6
SP	equ	R5
LR	equ	R4
;
; Device memory
;
	ORG	#0
Dev0	BSS	#2
Dev1	BSS	#2
Dev2	BSS	#2
;
; Start of code
;
	ORG	#$100
Mainline
	MOVL	Dev0,R0		; Address of device 0 (base address)
	MOVH	Dev0,R0
;
PollingLoop
	LDR	R0,#0,R1	; Read Dev0 Data and S/C reg
	AND.B	#4,R1		; Check DBA
	BNE	Dev0Hdlr	; Char avail -> goto Dev0Hdlr
	LDR	R0,#2,R1	; Read Dev1 Data and S/C reg
	AND.B	#4,R1		; Check DBA
	BNE	Dev1Hdlr	; Char avail -> goto Dev1Hdlr
	LDR	R0,#4,R1	; Read Dev2 Data and S/C reg
	AND.B	#4,R1		; Check DBA
	BNE	Dev2Hdlr	; Char avail -> goto Dev2Hdlr
;
	BAL	PollingLoop	; Nothing found -- repeat
;
Dev0Hdlr
	SWPB	R1		; Data byte in LSB (Dev 0)
	SXT	R1
	BIC #1,PSW
	RRC	R1
;	...			; Process byte
	BAL	PollingLoop	; Repeat polling loop
Dev1Hdlr
	SWPB	R1		; Data byte in LSB (Dev 1)
	MOV.B	R1,R3
	ADD	R1,R3
;	...			; Process byte
	BAL	PollingLoop	; Repeat polling loop
Dev2Hdlr
	SWPB	R1		; Data byte in LSB (Dev 2)
	SWAP	R2,R1
	SXT	R2
	SRA	R2
;	...			; Process byte
	BAL	PollingLoop	; Repeat polling loop
;
	END	Mainline