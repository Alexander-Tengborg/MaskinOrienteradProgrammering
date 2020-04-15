start:
@	Initiera port D som utport
	LDR	R0,=0x55555555
	LDR	R1,=0x40020C00
	STR 	R0,[R1]
@	Addressen till port D:s ut-dataregister till R5
	LDR	R5,=0x40020C14
@	Addressen till port e:s in-dataregister till R6
	LDR	R6,=0x40021010
main:	LDRB	R0,[R6]
	SXTB	R0,R0	@UXTB om ej tal med tecken
	LDRB	R1,[R6,#1]
	SXTB	R1,R1	@UXTB om ej tal med tecken
	CMP	R0,R1
	BGT	main_1
	MOV	R0,#0
	B	main_2
main_1:	MOV	R0,#0xFF
main_2:	STRB	R0,[R5]
	B	main
	
