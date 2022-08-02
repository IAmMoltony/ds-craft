
@{{BLOCK(placeholder_guy)

@=======================================================================
@
@	placeholder_guy, 16x32@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 256 = 288
@
@	Time-stamp: 2022-08-01, 17:52:49
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global placeholder_guyBitmap		@ 256 unsigned chars
	.hidden placeholder_guyBitmap
placeholder_guyBitmap:
	.word 0x11111110,0x00011111,0x22222210,0x00012222,0x22222210,0x00012222,0x22222210,0x00012202
	.word 0x22222210,0x00012212,0x22222210,0x00012222,0x22222210,0x00012222,0x22222210,0x00012212
	.word 0x22222210,0x00011122,0x22222210,0x00012222,0x11111110,0x00011111,0x22210000,0x00000001
	.word 0x22210000,0x00000001,0x22210000,0x00000001,0x22210000,0x00000001,0x22210000,0x00000001
	.word 0x22210000,0x00000001,0x22210000,0x00000001,0x22210000,0x00000001,0x22210000,0x00000001
	.word 0x22210000,0x00000001,0x11110000,0x00000001,0x22210000,0x00000001,0x22210000,0x00000001
	.word 0x22210000,0x00000001,0x22210000,0x00000001,0x22210000,0x00000001,0x22210000,0x00000001
	.word 0x22210000,0x00000001,0x22210000,0x00000001,0x22210000,0x00000001,0x11110000,0x00000001

	.section .rodata
	.align	2
	.global placeholder_guyPal		@ 32 unsigned chars
	.hidden placeholder_guyPal
placeholder_guyPal:
	.hword 0x0000,0x0000,0x7FFF,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(placeholder_guy)
