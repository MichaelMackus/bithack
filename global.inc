CHROUT         = $FFD2
SCREEN_RAM     = $0400
SCREEN_RAM_END = $07e8
COLOR_RAM      = $D800
MAP_COLS       = 40
MAP_ROWS       = 23
MAP_SIZE       = MAP_COLS * MAP_ROWS

MAP_WALL     = %00000000
MAP_FLOOR    = %00000001
MAP_DOOR     = %00000011
MAP_STAIR    = %00000101
MAP_CHEST    = %00001001
MAP_FOUNTAIN = %00010001

.globalzp tmp, tmp2

.global map
.struct Tile
    type .byte
    .struct Mob
        type .byte            ; one of MAP_MOB_* masks
        stats .byte
    .endstruct
    .struct Item
        type .byte            ; one of MAP_ITEM_* masks
        .union Specifics
            enchantment .byte ; for weapon/armor
            appearance  .byte ; for scroll/potion/wand
        .endunion
    .endstruct
.endstruct
