
@{{BLOCK(abtn)

@=======================================================================
@
@	abtn, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2022-08-01, 17:52:16
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global abtnBitmap		@ 128 unsigned chars
	.hidden abtnBitmap
abtnBitmap:
	.word 0x11111110,0x00000111,0x22222221,0x00001222,0x33333221,0x00001223,0x22222321,0x00001232
	.word 0x22222321,0x00001232,0x33333321,0x00001233,0x22222321,0x00001232,0x22222321,0x00001232
	.word 0x22222321,0x00001232,0x22222321,0x00001232,0x22222221,0x00001222,0x11111110,0x00000111
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global abtnPal		@ 32 unsigned chars
	.hidden abtnPal
abtnPal:
	.hword 0x0000,0x0C63,0x0421,0x7FFF,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(abtn)
