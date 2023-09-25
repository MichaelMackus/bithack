MAP_COLS        = 32
MAP_ROWS        = 30
MAP_TILES       = MAP_COLS * MAP_ROWS
PPU_CTRL        = $2000
PPU_MASK        = $2001
PPU_SCROLL      = $2005
PPU_ADDR        = $2006
PPU_DATA        = $2007
PATTERN_TABLE_0 = $0000
NAMETABLE_0     = $2000

.globalzp ptr1, ptr2, ptr3

.include "nes.inc"
.include "../common.inc"
