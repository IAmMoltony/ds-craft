
@{{BLOCK(sand_small)

@=======================================================================
@
@	sand_small, 8x8@16, 
@	+ bitmap not compressed
@	Total size: 128 = 128
@
@	Time-stamp: 2022-06-24, 10:54:05
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.16
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global sand_smallBitmap		@ 128 unsigned chars
	.hidden sand_smallBitmap
sand_smallBitmap:
	.word 0x4B1A533B,0x5B7C4B1A,0x533B4B1A,0x5F9C46FA,0x4B1A5B7C,0x4B1A4B1A,0x5B7C5B7C,0x533B4B1A
	.word 0x533B4B1A,0x533B5B7C,0x4B1A533B,0x533B533B,0x533B4B1A,0x4B1A533B,0x4B1A4B1A,0x4B1A5F9C
	.word 0x533B533B,0x4B1A533B,0x4B1A46FA,0x4B1A533B,0x533B4B1A,0x533B533B,0x4B1A5B7C,0x533B533B
	.word 0x533B533B,0x5B7C46FA,0x5B7C5B7C,0x533B533B,0x533B533B,0x5B7C5B7C,0x533B4B1A,0x533B533B

@}}BLOCK(sand_small)
