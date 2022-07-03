
@{{BLOCK(stone_small)

@=======================================================================
@
@	stone_small, 8x8@16, 
@	+ bitmap not compressed
@	Total size: 128 = 128
@
@	Time-stamp: 2022-07-03, 08:56:17
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global stone_smallBitmap		@ 128 unsigned chars
	.hidden stone_smallBitmap
stone_smallBitmap:
	.word 0x3DEF3DEF,0x3DEF3DEF,0x35AD3DEF,0x39CE39CE,0x46313DEF,0x3DEF4631,0x3DEF4631,0x39CE39CE
	.word 0x3DEF4631,0x3DEF3DEF,0x35AD39CE,0x463135AD,0x35AD39CE,0x39CE35AD,0x39CE3DEF,0x3DEF3DEF
	.word 0x3DEF3DEF,0x46313DEF,0x46313DEF,0x3DEF4631,0x3DEF3DEF,0x3DEF4631,0x3DEF3DEF,0x46314631
	.word 0x39CE4631,0x39CE3DEF,0x3DEF39CE,0x463139CE,0x3DEF3DEF,0x39CE3DEF,0x3DEF4631,0x3DEF39CE

@}}BLOCK(stone_small)
