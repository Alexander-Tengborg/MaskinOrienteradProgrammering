start:
@	Initiera prot D och E som utportar
	LDR	R6,=0x55555555
	LDR	R5,=0x40020C00
	STR	R6,[R5]
	LDR	R5,=0x40021000
	STR	R6,[R5]
@	Addressen till port D:s ut-dataregister till R5
	LDR	R5,=0x40020C14
@	Addressen till port E:s ut-dataregister till R6
	LDR	R6,=0x40021014
main:
	@R0 = R0 << R1
	
	