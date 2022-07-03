
@{{BLOCK(cactus_side_small)

@=======================================================================
@
@	cactus_side_small, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2022-07-03, 08:55:50
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global cactus_side_smallBitmap		@ 32 unsigned chars
	.hidden cactus_side_smallBitmap
cactus_side_smallBitmap:
	.word 0x03543210,0x31564240,0x03244543,0x04544540,0x31541540,0x04743213,0x04214540,0x04544540

	.section .rodata
	.align	2
	.global cactus_side_smallPal		@ 32 unsigned chars
	.hidden cactus_side_smallPal
cactus_side_smallPal:
	.hword 0x0000,0x1188,0x162B,0x4AF7,0x0D67,0x1A6C,0x3693,0x11EA
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(cactus_side_small)
