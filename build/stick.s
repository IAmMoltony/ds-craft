
@{{BLOCK(stick)

@=======================================================================
@
@	stick, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2022-08-01, 17:53:04
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global stickBitmap		@ 128 unsigned chars
	.hidden stickBitmap
stickBitmap:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x02100000,0x00000000,0x02310000
	.word 0x00000000,0x00231000,0x00000000,0x00024100,0x00000000,0x00002310,0x00000000,0x00000241
	.word 0x10000000,0x00000023,0x41000000,0x00000002,0x23100000,0x00000000,0x02410000,0x00000000
	.word 0x00231000,0x00000000,0x00023100,0x00000000,0x00002200,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global stickPal		@ 32 unsigned chars
	.hidden stickPal
stickPal:
	.hword 0x0000,0x08C9,0x0465,0x1191,0x0D2D,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(stick)
