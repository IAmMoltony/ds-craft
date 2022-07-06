# ds-craft

DS-Craft is a Minecraft clone for Nintendo DS. It is not finished. This game is made using libnds.

## Controls

Left and right for horizontal movement, up for jump, select to open inventory, a to place block, b to destroy block

### Inventory manipulation

A selects inventory slot. Pressing a again will cause either stacking the blocks or moving them. Your inventory has 20 slots. To select next hotbar slot press r.

### Crafting

Press r to select next crafting recipe. Press a to craft the item (if you have the requirements to craft it).

## Dependencies

 - devkitPro libnds installed
 - &lt;devkitPro folder&gt;/tools/bin in PATH variable.

## How to build

1. `makegfx` (if building for the first time or if images in gfx folder were updated)
2. `make`

## Credits

Mojang - textures, font and sounds
The libnds example authors
[This tutorial](https://www.youtube.com/watch?v=yb6QJl6mqf4)
