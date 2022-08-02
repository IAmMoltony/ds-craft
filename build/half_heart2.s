
@{{BLOCK(half_heart2)

@=======================================================================
@
@	half_heart2, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2022-08-01, 17:52:38
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global half_heart2Bitmap		@ 32 unsigned chars
	.hidden half_heart2Bitmap
half_heart2Bitmap:
	.word 0x00000000,0x00001100,0x00011210,0x00011110,0x00011130,0x00011300,0x00013000,0x00030000

	.section .rodata
	.align	2
	.global half_heart2Pal		@ 32 unsigned chars
	.hidden half_heart2Pal
half_heart2Pal:
	.hword 0x0000,0x085F,0x001F,0x0857,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(half_heart2)
