
@{{BLOCK(grass_small)

@=======================================================================
@
@	grass_small, 8x8@16, 
@	+ bitmap not compressed
@	Total size: 128 = 128
@
@	Time-stamp: 2022-07-02, 18:00:41
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global grass_smallBitmap		@ 128 unsigned chars
	.hidden grass_smallBitmap
grass_smallBitmap:
	.word 0x2AD022AD,0x1E8C22AD,0x26CE3312,0x22AD22AD,0x26CE26CE,0x14EB14EB,0x14EB22AD,0x1D4F26CE
	.word 0x25B214EB,0x14EB25B2,0x14EB1D4F,0x2E171D4F,0x2E171D4F,0x1D4F25B2,0x25B214EB,0x25B21D4F
	.word 0x1D4F25B2,0x1D4F14EB,0x1D4F1D4F,0x25B22E17,0x25B21D4F,0x25B22E17,0x1D4F25B2,0x14EB25B2
	.word 0x1D4F25B2,0x25B21D4F,0x14EB14EB,0x25B22E17,0x25B21D4F,0x1D4F1D4F,0x1D4F25B2,0x14EB25B2

@}}BLOCK(grass_small)
