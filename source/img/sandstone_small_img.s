
@{{BLOCK(sandstone_small)

@=======================================================================
@
@	sandstone_small, 8x8@16, 
@	+ bitmap not compressed
@	Total size: 128 = 128
@
@	Time-stamp: 2022-07-02, 18:00:54
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global sandstone_smallBitmap		@ 128 unsigned chars
	.hidden sandstone_smallBitmap
sandstone_smallBitmap:
	.word 0x4B1A535B,0x4B1A4B1A,0x535B535B,0x4B1A535B,0x5B7C3AB8,0x5F9C5F9C,0x5F9C5F9C,0x5F9C3AB8
	.word 0x46FA5F9C,0x3AB85B7C,0x535B535B,0x535B3AB8,0x5F9C535B,0x5F9C5B7C,0x5B7C4B1A,0x3AB85F9C
	.word 0x4B1A3AB8,0x535B535B,0x4B1A3AB8,0x5B7C535B,0x3AB8535B,0x5F9C3AB8,0x46FA4B1A,0x46FA5B7C
	.word 0x5B7C46FA,0x535B46FA,0x5B7C535B,0x4B1A535B,0x46FA535B,0x46FA5B7C,0x46FA5B7C,0x46FA4B1A

@}}BLOCK(sandstone_small)
