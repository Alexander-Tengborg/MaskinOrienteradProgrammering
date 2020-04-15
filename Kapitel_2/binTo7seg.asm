init:
@	Initiera port D som utport
	LDR	R0,=0x55555555
	LDR	R1,=0x40020C00
	STR 	R0,[R1]
@	Addressen till port D:s ut-dataregister till R5
	LDR	R5,=0x40020C14
@	Addressen till port e:s in-dataregister till R6
	LDR	R6,=0x40021010
	LDR	R4,=SegCodes

main:
	LDRB	R0,[R6]
	CMP	R0,#16
	BCS	lzero
	LDRB	R0,[R4,R0]
	STRB	R0,[R5]
	B	main
lzero:	LDRB	R5,=0
	B	main

SegCodes:	.BYTE	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07
		.BYTE	0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71
