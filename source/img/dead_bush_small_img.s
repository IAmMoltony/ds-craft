
@{{BLOCK(dead_bush_small)

@=======================================================================
@
@	dead_bush_small, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2022-07-02, 18:00:36
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global dead_bush_smallBitmap		@ 32 unsigned chars
	.hidden dead_bush_smallBitmap
dead_bush_smallBitmap:
	.word 0x00100000,0x00010200,0x00002300,0x01332310,0x00023000,0x00030300,0x00331000,0x00011000

	.section .rodata
	.align	2
	.global dead_bush_smallPal		@ 32 unsigned chars
	.hidden dead_bush_smallPal
dead_bush_smallPal:
	.hword 0x0000,0x10EA,0x1592,0x154C,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(dead_bush_small)
