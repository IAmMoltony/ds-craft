
@{{BLOCK(heart_outline)

@=======================================================================
@
@	heart_outline, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2022-08-01, 17:52:39
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global heart_outlineBitmap		@ 128 unsigned chars
	.hidden heart_outlineBitmap
heart_outlineBitmap:
	.word 0x01101100,0x00000000,0x12212210,0x00000000,0x22222221,0x00000001,0x22222221,0x00000001
	.word 0x22222221,0x00000001,0x12222210,0x00000000,0x01222100,0x00000000,0x00121000,0x00000000
	.word 0x00010000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global heart_outlinePal		@ 32 unsigned chars
	.hidden heart_outlinePal
heart_outlinePal:
	.hword 0x0000,0x0000,0x14A5,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(heart_outline)
