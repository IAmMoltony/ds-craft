
@{{BLOCK(gray_circle)

@=======================================================================
@
@	gray_circle, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2022-08-01, 17:52:36
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global gray_circleBitmap		@ 128 unsigned chars
	.hidden gray_circleBitmap
gray_circleBitmap:
	.word 0x11100000,0x00000111,0x11111000,0x00011111,0x11111100,0x00111111,0x11111110,0x01111111
	.word 0x11111110,0x01111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111
	.word 0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111110,0x01111111
	.word 0x11111110,0x01111111,0x11111100,0x00111111,0x11111000,0x00011111,0x11100000,0x00000111

	.section .rodata
	.align	2
	.global gray_circlePal		@ 32 unsigned chars
	.hidden gray_circlePal
gray_circlePal:
	.hword 0x0000,0x18C6,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(gray_circle)
