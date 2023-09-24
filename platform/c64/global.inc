CHROUT         = $FFD2
SCREEN_RAM     = $C400
CHAR_RAM       = $C800
COLOR_RAM      = $D800
MAP_COLS       = 40
MAP_ROWS       = 20
MAP_TILES      = MAP_COLS * MAP_ROWS

.globalzp ptr1, ptr2, ptr3

.include "../common.inc"
