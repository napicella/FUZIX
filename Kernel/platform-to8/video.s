	.module to9video

	; Methods provided
	.globl _plot_char
	.globl _scroll_up
	.globl _scroll_down
	.globl _clear_across
	.globl _clear_lines
	.globl _cursor_on
	.globl _cursor_off
	.globl _cursor_disable
	.globl _vtattr_notify

	.globl video_init
	;
	; Imports
	;
	.globl _fontdata_8x8
	.globl _vidattr
	.globl map_video

	include "kernel.def"
	include "../kernel09.def"

	.area .text

;
;	Compute the video base address
;	A = X, B = Y, returns an address in X
;
;	40 column for now - 80 is dual bank interleaved so a bit more tricky
;
;	This is a simple bitmap, so chracters are just copied 8x8
;
vidaddr:
	pshs a
	tfr b,a
	clra
	pshs d
	lsra
	rorb
	lsra
	rorb			; x 64
	addd ,s++		; x 320
	tfr d,x
	puls a
	leax VIDEO_BASE,x
	jmp map_video
;
;	plot_char(int8_t y, int8_t x, uint16_t c)
;
_plot_char:
	pshs y
	lda 4,s
	bsr vidaddr		; preserves X (holding the char)
	tfr x,d
	andb #$7F		; no high font bits
	subb #$20		; skip control symbols
	rolb			; multiply by 8
	rola
	rolb
	rola
	rolb
	rola
	tfr d,x
	leax _fontdata_8x8,x		; relative to font
	ldb _vtattr
	andb #0x3F		; drop the bits that don't affect our video
	beq plot_fast

	;
	;	General purpose plot with attributes, we only fastpath
	;	the simple case
	;
	clra
plot_loop:
	sta _vtrow
	ldb _vtattr
	cmpa #7		; Underline only applies on the bottom row
	beq ul_this
	andb #0xFD
ul_this:
	cmpa #3		; italic shift right for < 3
	blt ital_1
	andb #0xFB
	bra maskdone
ital_1:
	cmpa #5		; italic shift right for >= 5
	blt maskdone
	bitb #0x04
	bne maskdone
	orb #0x40		; spare bit borrow for bottom of italic
	andb #0xFB
maskdone:
	lda ,x+			; now throw the row away for a bit
	bitb #0x10
	bne notbold
	lsra
	ora -1,x		; shift and or to make it bold
notbold:
	bitb #0x04		; italic by shifting top and bottom
	beq notital1
	lsra
notital1:
	bitb #0x40
	beq notital2
	lsla
notital2:
	bitb #0x02
	beq notuline
	lda #0xff		; underline by setting bottom row
notuline:
	bitb #0x01		; inverse or not: we are really in inverse
	bne plot_inv		; by default so we complement except if
	coma			; inverted
plot_inv:
	bitb #0x20		; overstrike or plot ?
	bne overstrike
	sta ,y
	bra plotnext
overstrike:
	anda ,y
	sta ,y
plotnext:
	leay 32,y
	lda _vtrow
	inca
	cmpa #8
	bne plot_loop
unmap_video:
	jsr map_kernel
	puls y,pc
;
;	Fast path for normal attributes
;
plot_fast:
	lda ,x+			; simple 8x8 renderer for now
	coma
	sta 0,y
	lda ,x+
	coma
	sta 32,y
	lda ,x+
	coma
	sta 64,y
	lda ,x+
	coma
	sta 96,y
	lda ,x+
	coma
	sta 128,y
	lda ,x+
	coma
	sta 160,y
	lda ,x+
	coma
	sta 192,y
	lda ,x+
	coma
	sta 224,y
	bra unmap_video

;
;	void scroll_up(void)
;
_scroll_up:
	pshs y
	jsr map_video
	ldy #VIDEO_BASE
	leax 320,y
vscrolln:
	; Unrolled line by line copy
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	ldd ,x++
	std ,y++
	cmpx video_endptr
	bne vscrolln
	jmp unmap_video

;
;	void scroll_down(void)
;
_scroll_down:
	pshs y
	jsr map_video
	ldy #VIDEO_END
	leax -320,y
vscrolld:
	; Unrolled line by line loop
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	ldd ,--x
	std ,--y
	cmpx video_startptr
	bne vscrolld
	jmp unmap_video

video_startptr:
	.dw	VIDEO_BASE
video_endptr:
	.dw	VIDEO_END

;
;	clear_across(int8_t y, int8_t x, uint16_t l)
;
_clear_across:
	pshs y
	lda 4,s		; x into A, B already has y
	jsr vidaddr	; Y now holds the address
	tfr x,d		; Shuffle so we are writng to X and the counter
	tfr y,x		; l is in d
	lda #$ff
clearnext:
	sta ,x
	sta 40,x
	sta 80,x
	sta 120,x
	sta 160,x
	sta 200,x
	sta 240,x
	sta 280,x
	leax 1,x
	decb
	bne clearnext
	puls y,pc
;
;	clear_lines(int8_t y, int8_t ct)
;
_clear_lines:
	pshs y
	clra			; b holds Y pos already
	jsr vidaddr		; y now holds ptr to line start
	tfr y,x
	ldd #$ffff
	lsl 4,s
	lsl 4,s
	lsl 4,s
wipel:
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	std ,x++
	dec 4,s			; count of lines
	bne wipel
	puls y,pc

_cursor_on:
	pshs y
	lda  4,s
	jsr vidaddr
	tfr y,x
	puls y
	stx cursor_save
	; Fall through
_cursor_off:
	ldb _vtattr
	bitb #0x80
	bne nocursor
	ldx cursor_save
	com ,x
	com 40,x
	com 80,x
	com 120,x
	com 160,x
	com 200,x
	com 240,x
	com 280,x
nocursor:
_vtattr_notify:
_cursor_disable:
	rts

video_init:
	jsr	map_video
	ldx	#VIDEO_BASE
	ldd	#0xAAAA
vidwipe:
	std	,x++
	cmpx	#VIDEO_END
	bne 	vidwipe
	jmp	map_kernel

	.area .data
cursor_save:
	.dw	0
_vtrow:
	.db	0
