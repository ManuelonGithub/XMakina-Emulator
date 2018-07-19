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
; Status/control bits
;
DBA	equ	#4	; DBA bit
IE	equ	#1	; IE bit
;
; PSW priorities
;
PRIOR7	equ	#$E0
PRIOR4	equ	#$80
PRIOR3	equ	#$60
; Device memory
;
	ORG	#0
Dev0	BSS	#2
Dev1	BSS	#2
Dev2	BSS	#2
Dev3SCR	byte	IE
Dev3Out	byte	#0
Dev4SCR	byte	IE
Dev4Out	byte	#0
Dev5	BSS #2
;
; Start of code
;
	ORG	#$100
Mainline
	movl	StkTop,SP
	movh	StkTop,SP
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
	LDR	R0,#10,R1	; Read Dev2 Data and S/C reg
	AND.B	#4,R1		; Check DBA
	BNE	Dev5Hdlr	; Char avail -> goto Dev2Hdlr
;
	BAL	PollingLoop	; Nothing found -- repeat
;
Dev0Hdlr
	SWPB	R1		; Data byte in LSB (Dev 0)
	BIS #1,PSW
	RRC.B	R1
	SXT	R1
	ADDC	#2,R1
	STR.B	R1,R0,#7	; Trigger dev3 data process
	BAL	PollingLoop	; Repeat polling loop
;
Dev1Hdlr
	SWPB	R1		; Data byte in LSB (Dev 1)
	MOV.B	R1,R3
	MOVLZ	#$FF,R1
	SUB	R3,R1
	DADD	R1,R2
	BAL	PollingLoop	; Repeat polling loop
;
Dev2Hdlr
	SWPB	R1		; Data byte in LSB (Dev 2)
	XOR	#-1,R1
	BIC	#-1,R2
	MOVH	#$1000,R2
	ST	R1,R2+
	MOVLZ	#'V',R1
	STR.B	R1,R0,#9	; Trigger dev3 data process
	BAL	PollingLoop	; Repeat polling loop
;
Dev5Hdlr
	SWPB	R1		; Data byte in LSB (Dev 2)
	BIT.B	#1,R1
	BEQ	ODD
	CMP	#0,R1
	BAL	PollingLoop	; Repeat polling loop
	
ODD
	SRA	R1
	BAL	PollingLoop	; Repeat polling loop
	
	ORG	#$6000
dev3ISR
	MOVLZ #$FF,R1
	ADD	#2,R1
	mov	LR,PC	
	
dev4ISR
	MOVLZ	#16,R1
	DADD	R1,R1
	mov	LR,PC
	
	org	#$FFC0
StkTop
;Dev3 vector
	org	#$FFCC
	word	PRIOR4
	word	dev3ISR
;
;Dev4 vector
	word	PRIOR4
	word	dev4ISR
	
	END	Mainline