
	.export __ldbyte16

	.setcpu 8080
	.code
__ldbyte16:
	lxi h,16

	mov l,m
	ret