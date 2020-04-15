@	Del 1
@	LDR	R1,=s
@	LDR	R2,=c
@	LDR	R0,[R1]
@	STRB	R0,[R2]
@	.ALIGN
@s:	.SPACE 2
@c:	.SPACE 1


@	Del 2
	LDR	R1,=i
	LDR	R2,=s
	LDR	R0,[R1]
	STRH	R0,[R2]
	.ALIGN
i:	.SPACE 4
s:	.SPACE 2
