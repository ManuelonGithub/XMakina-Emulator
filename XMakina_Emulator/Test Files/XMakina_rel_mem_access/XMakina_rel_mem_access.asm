	ORG	#$100
	
MAINLINE
	
	LDR	R2,#$5,R3
	LDR.B	R2,#$5,R3
	STR	R2,R3,#-15
	STR.B	R2,R3,#$5
	
	END MAINLINE
	