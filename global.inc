CHROUT         = $FFD2
SCREEN_RAM     = $0400
SCREEN_RAM_END = $07e8
COLOR_RAM      = $D800
MAP_COLS       = 40
MAP_ROWS       = 20
MAP_TILES      = MAP_COLS * MAP_ROWS

MAP_WALL     = %00000000
MAP_FLOOR    = %00000001
MAP_DOOR     = %00000011
MAP_STAIR    = %00000101
MAP_CHEST    = %00001001
MAP_FOUNTAIN = %00010001

.globalzp tmp1, tmp2, tmp3

.global map
.global map_end

.macro is_end_of_map addr
.scope
    lda addr + 1
    cmp #>map_end
    bne no
    lda addr
    cmp #<map_end
    bne no
    lda #0
    jmp end
no:
    lda #1
end:
.endscope
.endmacro

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
MAP_SIZE = MAP_TILES * .sizeof(Tile)

.macro add16 addr, word
    clc
    lda addr
    adc #<word
    sta addr
    lda addr + 1
    adc #>word
    sta addr + 1
.endmacro
.macro add16addrs addr, addr2
    clc
    lda addr
    adc addr2
    sta addr
    lda addr + 1
    adc addr2 + 1
    sta addr + 1
.endmacro
