
@{{BLOCK(door_small)

@=======================================================================
@
@	door_small, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2022-07-01, 09:40:34
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global door_smallBitmap		@ 32 unsigned chars
	.hidden door_smallBitmap
door_smallBitmap:
	.word 0x00211100,0x00200300,0x00200400,0x00444100,0x00211300,0x00455400,0x00224100,0x00333300

	.section .rodata
	.align	2
	.global door_smallPal		@ 32 unsigned chars
	.hidden door_smallPal
door_smallPal:
	.hword 0x0000,0x154C,0x21D2,0x10EA,0x198F,0x2A36,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(door_small)
