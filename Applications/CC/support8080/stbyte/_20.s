
	.export __stbyte20

	.setcpu 8080
	.code
__stbyte20:
	mov a,l
	lxi h,20

	mov m,a
	mov l,a
	ret