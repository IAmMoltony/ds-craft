
@{{BLOCK(pig_damage)

@=======================================================================
@
@	pig_damage, 32x32@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 512 = 544
@
@	Time-stamp: 2022-08-01, 17:52:48
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global pig_damageBitmap		@ 512 unsigned chars
	.hidden pig_damageBitmap
pig_damageBitmap:
	.word 0x00000000,0x11100000,0x00011111,0x00000000,0x00000000,0x11100000,0x00011111,0x00000000
	.word 0x00000000,0x11100000,0x00011111,0x00000000,0x00000000,0x11100000,0x00011111,0x00000000
	.word 0x11111111,0x11111111,0x00111111,0x00000000,0x11111111,0x11111111,0x00111111,0x00000000
	.word 0x11111111,0x11111111,0x00111111,0x00000000,0x11111111,0x11111111,0x00011111,0x00000000
	.word 0x11111111,0x11111111,0x00000001,0x00000000,0x11111111,0x11111111,0x00000001,0x00000000
	.word 0x11111111,0x11111111,0x00000001,0x00000000,0x11111111,0x11111111,0x00000001,0x00000000
	.word 0x11111111,0x11111111,0x00000001,0x00000000,0x00011110,0x01111000,0x00000000,0x00000000
	.word 0x00011110,0x01111000,0x00000000,0x00000000,0x00011110,0x01111000,0x00000000,0x00000000

	.word 0x00011110,0x01111000,0x00000000,0x00000000,0x00011110,0x01111000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global pig_damagePal		@ 32 unsigned chars
	.hidden pig_damagePal
pig_damagePal:
	.hword 0x0000,0x001F,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(pig_damage)