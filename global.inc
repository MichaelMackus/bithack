CHROUT     = $FFD2
SCREEN_RAM = $0400
COLOR_RAM  = $D800

.globalzp tmp

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

