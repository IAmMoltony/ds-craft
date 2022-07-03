
@{{BLOCK(poppy_small)

@=======================================================================
@
@	poppy_small, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2022-07-03, 08:56:09
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global poppy_smallBitmap		@ 32 unsigned chars
	.hidden poppy_smallBitmap
poppy_smallBitmap:
	.word 0x00000000,0x00000000,0x00021000,0x00013100,0x00004000,0x00004000,0x00540400,0x00044000

	.section .rodata
	.align	2
	.global poppy_smallPal		@ 32 unsigned chars
	.hidden poppy_smallPal
poppy_smallPal:
	.hword 0x0000,0x14DD,0x0C93,0x008E,0x1104,0x1164,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(poppy_small)
