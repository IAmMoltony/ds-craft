
@{{BLOCK(oak_sapling)

@=======================================================================
@
@	oak_sapling, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ bitmap not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2022-08-01, 17:52:46
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global oak_saplingBitmap		@ 128 unsigned chars
	.hidden oak_saplingBitmap
oak_saplingBitmap:
	.word 0x00000000,0x00000000,0x00000000,0x00001000,0x20000000,0x00003300,0x21040000,0x00465400
	.word 0x13660000,0x00776544,0x06321200,0x00237664,0x75612000,0x02120660,0x66000000,0x02200068
	.word 0x64300000,0x00034012,0x23767000,0x00876321,0x67633100,0x01387685,0x62701110,0x01108786
	.word 0x31200000,0x01000216,0x82100000,0x00000126,0x70000000,0x00000286,0x70000000,0x00000087

	.section .rodata
	.align	2
	.global oak_saplingPal		@ 32 unsigned chars
	.hidden oak_saplingPal
oak_saplingPal:
	.hword 0x0000,0x1628,0x1EAA,0x0D83,0x0942,0x1D8F,0x154E,0x0CEB
	.hword 0x08C9,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(oak_sapling)
